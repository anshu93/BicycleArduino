// Reed switch repetition counter

// Wheel
int sense0 = 2;
volatile byte counter0 = 0;
unsigned long lastDebounce0 = 0;

//// Pedal
int sense1 = 3;
volatile byte counter1 = 0;
unsigned long lastDebounce1 = 0;

unsigned long debounceDelay = 250;    // Ignore bounces under 50 ms
float rpm = 0;
float rpm1 = 0;
unsigned long timeold0 = 0;
unsigned long timeold1 = 0;

boolean pedalFlag = true;
boolean wheelFlag = true;

void setup() {
  
  Serial.begin(9600);
  pinMode(sense0, INPUT);
  pinMode(sense1, INPUT);
  digitalWrite(sense0, HIGH);
  digitalWrite(sense1, HIGH);
  
  attachInterrupt(0, magnet, FALLING);
  attachInterrupt(1, magnet2, FALLING);
  
  Serial.println("Starting to measure");
}

void loop() {
  if(counter0 > 0){
    float time_elapsed = 1000/ (float) (millis() - timeold0);
    rpm = time_elapsed * 60 * (float) counter0;
    timeold0 = millis();
    counter0 = 0;
    wheelFlag = false;
    Serial.println("Wheel RPM:");
    Serial.println(rpm);
    Serial.println("--------------");
  }
  
  if(counter1 > 9){
    float time_elapsed1 = 1000/ (float) (millis() - timeold1);
    rpm1 = time_elapsed1 * 60 * (float) counter1;
    timeold1 = millis();
    counter1 = 0;
    pedalFlag = false;
    Serial.println("Pedal RPM:");
    Serial.println(rpm1);
    Serial.println("--------------");
  }
//  
//  if(pedalFlag == false && wheelFlag == false){
//    
//    Serial.print("rpm of pedal: ");
//    Serial.println(rpm, 4);  
//    
//    Serial.print("rpm of wheel: ");
//    Serial.println(rpm1, 4);  
//    
//    float ratio = rpm / rpm1;
//    Serial.print("Ratio (pedal/wheel): ");
//    Serial.println(ratio, 4);
//    
//    Serial.println("Counters: ");
//    Serial.println(counter0);
//    Serial.println(counter1);
//    
//    Serial.println("-------------------");
//    
//    pedalFlag = true;
//    wheelFlag = true;
//    //counter0 = 0;
//    //counter1 = 0;
//    //timeold0 = 0;
//    //timeold1 = 0;
//  }
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
