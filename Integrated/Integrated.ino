#include <SPI.h>
#include <SD.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// GPS
HardwareSerial mySerial = Serial1;  // Attach the GPS to hardware port 18,19
Adafruit_GPS GPS(&mySerial);

#define GPSECHO  false  // Don't reflect GPS actions on screen

// SD Card
#define CS_PIN  53    // Card select pin for SD card

// BT initialization
char data;
SoftwareSerial BTSerial(12, 13); // RX, TX for BT module
File myFile;  // For SD card
char* data_file = "GPSDATA.TXT";

void setup() {
  Serial.begin(115200);  // Start the computer talking
  BTSerial.begin(9600);  // Start the BT talking
  delay(5000);
  
  Serial.println(F("Adafruit GPS library SD Data Logging test!"));
  Serial.println(F("-----------------------------------------------"));
  GPS.begin(9600);
  delay(1000);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  mySerial.println(PMTK_Q_RELEASE);
  Serial.println(F("Initializing SD card..."));
  if (!SD.begin(CS_PIN)) {
    Serial.println(F("Card failed, or not present"));
    Serial.println(F("Try again....  "));
    return;
  } 
  Serial.println(F("Card Initialized, Your ready to ROLL..."));
  Serial.println(F("Writting data to the SD Card.... STANDBY"));
}
uint32_t timer = millis();
void loop(){
  
  // SCAN FOR INCOMING GPS DATA
  char c = GPS.read();
  if ((c) && (GPSECHO)){
    Serial.write(c); 
  }
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())){
      return;
    }
    if (timer > millis()){  
      timer = millis();
    }
  if (millis() - timer > 2000) { 
    timer = millis();
    File gpsFile = SD.open(data_file, FILE_WRITE);
    if (gpsFile) {
      if((int)GPS.fixquality != 0){
      gpsFile.print("\n");
      gpsFile.print(GPS.hour, DEC); 
      gpsFile.print(':');
      gpsFile.print(GPS.minute, DEC); 
      gpsFile.print(':');
      gpsFile.print(GPS.seconds, DEC);
      gpsFile.print(',');
      gpsFile.print((int)GPS.fixquality); 
      gpsFile.print(',');
      gpsFile.print(GPS.latitude, 4); 
      gpsFile.print(GPS.lat);
      gpsFile.print(","); 
      gpsFile.print(GPS.longitude, 4); 
      gpsFile.print          (GPS.lon);
      gpsFile.print(","); 
      gpsFile.print(GPS.altitude);
      gpsFile.print(","); 
      gpsFile.print((int)GPS.satellites);
      gpsFile.close();
      Serial.println(F("Data written to file..."));
      }else{
        Serial.println(F("No Fix obtained...")); 
      }
    }
    else{
      BTSerial.println(F("error writing the data to the SD Card, try again..."));
    }
    } 
  }
  
  // SCAN FOR BLUETOOTH DATA
    if(BTSerial.available()){
    data = BTSerial.read();
    if(data == 't'){     
        sendSDdata();
    }
  }
}

void sendSDdata(){
  
  BTSerial.print("Data Received...");
        
  BTSerial.print("#");
  
  myFile = SD.open(data_file);
  if(myFile){
    while(myFile.available()){
      char c = myFile.read();
      if(c == '\n'){
        c = ';';  
      }
      BTSerial.print(c);  
    }
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    BTSerial.println("error opening test.txt");
  }  

  BTSerial.print("#");
    
    // Speed
  BTSerial.print("5.67");
  BTSerial.print(",");
    
    // Distance
  BTSerial.print("3.45");
  BTSerial.print("!");
}
