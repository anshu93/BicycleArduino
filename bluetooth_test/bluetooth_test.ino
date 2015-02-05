#include <SoftwareSerial.h>

#define reed_switch 2    // Pin on which reed switch is connected

char data;
int  LED = 5; // LED on pin 13
SoftwareSerial mySerial(10, 11); // RX, TX

volatile byte counter0 = 0;
unsigned long lastDebounce0 = 0;
unsigned long debounceDelay = 250;    // Ignore bounces under 50 ms
float rpm = 0;
unsigned long timeold = 0;

boolean send_cur_RPM = false;

void setup()
{
  // Open serial communications and wait for port to open:
  pinMode(LED, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  
  pinMode(reed_switch, INPUT);
  digitalWrite(reed_switch, HIGH);
  attachInterrupt(0, magnet, RISING);
}

void loop(){ 
  if(counter0 > 5){
    float time_elapsed = 1000/ (float) (millis() - timeold);s
    rpm = time_elapsed * 60 * (float) counter0;
    timeold = millis();
    counter0 = 0;
    Serial.print("rpm: ");
    Serial.println(rpm);  
    if(send_cur_RPM){
      mySerial.print("Current RPM: ");
      mySerial.println(rpm);
    }
  }
  
  if(mySerial.available()){
    data = mySerial.read();
    if(data == '1'){
      digitalWrite(LED, HIGH);
      send_cur_RPM = true;
    }
    if(data == '2'){
      digitalWrite(LED, LOW);
      send_cur_RPM = false;  
    }
  }
}

void magnet() {
  if( (millis() - lastDebounce0) > debounceDelay){
    counter0++;
    lastDebounce0 = millis();
  }
}


