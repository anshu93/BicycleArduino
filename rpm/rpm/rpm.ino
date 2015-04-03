// Reed switch repetition counter

// Wheel
int sense0 = 2;
volatile byte counter0 = 0;
unsigned long lastDebounce0 = 0;

//// Pedal
int sense1 = 3;
volatile byte counter1 = 0;
unsigned long lastDebounce1 = 0;

unsigned long debounceDelay = 50;    // Ignore bounces under 50 ms
float rpm_wheel = 0;
float rpm_pedal = 0;
unsigned long timeold0 = 0;
unsigned long timeold1 = 0;

boolean pedalFlag = true;
boolean wheelFlag = true;

int count = 0;

#define CALCULATION_INTERVAL 5

#define HIGH_THRESH 1.05
#define LOW_THRESH 0.98

float ratio = 0;
boolean ratio_flag = false;

void setup() {
  
  Serial.begin(9600);
  pinMode(sense0, INPUT);
  pinMode(sense1, INPUT);
  digitalWrite(sense0, HIGH);
  digitalWrite(sense1, HIGH);
  
  attachInterrupt(0, magnet, FALLING);
  attachInterrupt(1, magnet2, FALLING);
  
  // Set up timers
  
   noInterrupts();           // disable all interrupts
   TCCR1A = 0;
   TCCR1B = 0;

   TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
   TCCR1B |= (1 << CS12);    // 256 prescaler 
   TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
   interrupts();

  
  Serial.println("Starting to measure");
}

ISR(TIMER1_OVF_vect){
 count++;
 //Serial.println("1 s elapsed");
 //Serial.println(count);
 //Serial.print("\n");
 if(count == CALCULATION_INTERVAL){
   if(rpm_wheel != 0 && rpm_pedal != 0){
     ratio = rpm_wheel/rpm_pedal;
     Serial.println("RATIO: WHEEL/PEDAL");
     Serial.println(ratio, 6);
     Serial.println("-----------");
     ratio_flag = true;
     
     
   }else{
     Serial.println("NO MOVT DETECTED");
     Serial.println("-----------------"); 
   }
   count = 0;
 }
  TCNT1 = 34286;
}

void find_ratio(float ratio){
  
  // M
  if(ratio < HIGH_THRESH*1.36 && ratio > LOW_THRESH*1.36){
    Serial.println("CURRENT GEAR IS M1");  
  }   
  else if(ratio < HIGH_THRESH*1.58 && ratio > LOW_THRESH*1.58){
    Serial.println("CURRENT GEAR IS M2");  
  }   
  else if(ratio < HIGH_THRESH*1.81 && ratio > LOW_THRESH*1.81){
    Serial.println("CURRENT GEAR IS M3");  
  }   
  else if(ratio < HIGH_THRESH*2.12 && ratio > LOW_THRESH*2.12){
    Serial.println("CURRENT GEAR IS M4");  
  }   
  else if(ratio < HIGH_THRESH*2.36 && ratio > LOW_THRESH*2.36){
    Serial.println("CURRENT GEAR IS M5");  
  }   
  else if(ratio < HIGH_THRESH*2.72 && ratio > LOW_THRESH*2.72){
    Serial.println("CURRENT GEAR IS M6");  
  }
  
  // H
  
  else if(ratio < HIGH_THRESH*2 && ratio > LOW_THRESH*2){
    Serial.println("CURRENT GEAR IS H2");  
  }
  else if(ratio < HIGH_THRESH*2.31 && ratio > LOW_THRESH*2.31){
    Serial.println("CURRENT GEAR IS H3");  
  }
  else if(ratio < HIGH_THRESH*2.72 && ratio > LOW_THRESH*2.72){
    Serial.println("CURRENT GEAR IS H4");  
  }
  else if(ratio < HIGH_THRESH*2.94 && ratio > LOW_THRESH*2.94){
    Serial.println("CURRENT GEAR IS H5");  
  }
  else if(ratio < HIGH_THRESH*3.5 && ratio > LOW_THRESH*3.5){
    Serial.println("CURRENT GEAR IS H6");  
  }
  
  // L
  
  else if(ratio < HIGH_THRESH*1 && ratio > LOW_THRESH*1){
    Serial.println("CURRENT GEAR IS L1");  
  }
  else if(ratio < HIGH_THRESH*1.16 && ratio > LOW_THRESH*1.16){
    Serial.println("CURRENT GEAR IS L2");  
  }
  else if(ratio < HIGH_THRESH*1.34 && ratio > LOW_THRESH*1.34){
    Serial.println("CURRENT GEAR IS L3");  
  }
  else if(ratio < HIGH_THRESH*1.75 && ratio > LOW_THRESH*1.75){
    Serial.println("CURRENT GEAR IS L4");  
  }
  
  else{
   Serial.println("GEAR NOT RECOGNIZED");
  }   
  return;
}

void loop() {
  if(ratio_flag == true){
    ratio_flag = false;
    find_ratio(ratio);
  }
  
  if(counter0 > 5){
    float time_elapsed = 1000/ (float) (millis() - timeold0);
    rpm_wheel = time_elapsed * 60 * (float) counter0;
    timeold0 = millis();
    counter0 = 0;
    wheelFlag = false;
    Serial.println("Wheel RPM:");
    Serial.println(rpm_wheel);
    Serial.println("--------------");
  }
  
  if(counter1 > 5){
    float time_elapsed1 = 1000/ (float) (millis() - timeold1);
    rpm_pedal = time_elapsed1 * 60 * (float) counter1;
    timeold1 = millis();
    counter1 = 0;
    pedalFlag = false;
    Serial.println("Pedal RPM:");
    Serial.println(rpm_pedal);
    Serial.println("--------------");
  }
}

// Wheel Interrupt
void magnet() {
  if( (millis() - lastDebounce0) > debounceDelay){
    counter0++;
    lastDebounce0 = millis();
  }
}

// Pedal Interrupt
void magnet2() {
  if( (millis() - lastDebounce1) > debounceDelay){
    counter1++;
    lastDebounce1 = millis();
  }
}
