// Reed switch repetition counter

int sense0 = 2;
volatile byte counter0 = 0;
unsigned long lastDebounce0 = 0;
unsigned long debounceDelay = 250;    // Ignore bounces under 50 ms
float rpm = 0;
unsigned long timeold = 0;

void setup() {
  
  Serial.begin(9600);
  pinMode(sense0, INPUT);
  digitalWrite(sense0, HIGH);
  
  attachInterrupt(0, magnet, FALLING);
  //attachInterrupt(1, trigger1, FALLING);
  
  Serial.println("Repetition counter");
  Serial.print("Start");
  Serial.print(":");
  Serial.println("End");
}

void loop() {
  if(counter0 > 5){
    float time_elapsed = 1000/ (float) (millis() - timeold);
    rpm = time_elapsed * 60 * (float) counter0;
    timeold = millis();
    counter0 = 0;
    Serial.print("rpm: ");
    Serial.println(rpm);  
  }
}

void magnet() {
  if( (millis() - lastDebounce0) > debounceDelay){
    counter0++;Bicycle
    lastDebounce0 = millis();
  }
}
