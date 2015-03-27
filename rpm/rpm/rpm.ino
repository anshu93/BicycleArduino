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
 Serial.println(count);
 //Serial.print("\n");
 if(count == CALCULATION_INTERVAL){
   Serial.print("5s elapsed");
   if(rpm_wheel != 0 && rpm_pedal != 0){
     ratio = rpm_wheel/rpm_pedal;
     Serial.print("RATIO: WHEEL/PEDAL");
     Serial.print(ratio, 6);
     Serial.print("-----------");
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
  if(ratio < 1.1*1.36 && ratio > 0.9*1.36){
    Serial.println("CURRENT GEAR IS M1");  
  }   
    if(ratio < 1.1*1.58 && ratio > 0.9*1.58){
    Serial.println("CURRENT GEAR IS M2");  
  }   
    if(ratio < 1.1*1.81 && ratio > 0.9*1.81){
    Serial.println("CURRENT GEAR IS M3");  
  }   
    if(ratio < 1.1*2.12 && ratio > 0.9*2.12){
    Serial.println("CURRENT GEAR IS M4");  
  }   
    if(ratio < 1.1*2.36 && ratio > 0.9*2.36){
    Serial.println("CURRENT GEAR IS M5");  
  }   
    if(ratio < 1.1*2.72 && ratio > 0.9*2.72){
    Serial.println("CURRENT GEAR IS M6");  
  }else{
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
