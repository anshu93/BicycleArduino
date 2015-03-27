#include <SPI.h>
#include <SD.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

HardwareSerial mySerial = Serial1;  // Attach the GPS to hardware port 18,19
Adafruit_GPS GPS(&mySerial);

#define GPSECHO  false  // Don't reflect GPS actions on screen
#define CS_PIN  53    // Card select pin for SD card

// BT initialization
char data;
SoftwareSerial BTSerial(12, 13); // RX, TX for BT module
File myFile;  // For SD card

#define SD_CARD_OFF false  // True = debugging with SD card

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
    File gpsFile = SD.open("GPSRMC2.txt", FILE_WRITE);
    if (gpsFile) {
      gpsFile.print("\nTime: ");
      gpsFile.print(GPS.hour, DEC); 
      gpsFile.print(':');
      gpsFile.print(GPS.minute, DEC); 
      gpsFile.print(':');
      gpsFile.println(GPS.seconds, DEC);
      gpsFile.print("Date: ");
      gpsFile.print(GPS.day, DEC); 
      gpsFile.print('/');
      gpsFile.print(GPS.month, DEC); 
      gpsFile.print("/20");
      gpsFile.println(GPS.year, DEC);
      gpsFile.print("Fix: "); 
      gpsFile.print((int)GPS.fix);
      gpsFile.print(" quality: "); 
      gpsFile.println((int)GPS.fixquality); 
      gpsFile.print("Location: ");
      gpsFile.print(GPS.latitude, 4); 
      gpsFile.print(GPS.lat);
      gpsFile.print(", "); 
      gpsFile.print(GPS.longitude, 4); 
      gpsFile.println(GPS.lon);
      gpsFile.print("Speed (knots): "); 
      gpsFile.println(GPS.speed);
      gpsFile.print("Angle: "); 
      gpsFile.println(GPS.angle);
      gpsFile.print("Altitude: "); 
      gpsFile.println(GPS.altitude);
      gpsFile.print("Satellites: "); 
      gpsFile.println((int)GPS.satellites);
      gpsFile.println("-------------------------------------------------------- "); 
      gpsFile.close();
      Serial.println(F("Data written to file..."));
    }
    else{
      Serial.println(F("error writing the data to the SD Card, try again..."));
    }
    } 
  }
  
    if(BTSerial.available()){
    data = BTSerial.read();
    if(data == 't'){     
      if(!SD_CARD_OFF){
        sendSDdata();
      }else{
      
      // Welcome
        mySerial.print("Data Received...");
        
        mySerial.print("#");
        
        // GPS
        mySerial.print("36.001031, -78.939034");
        mySerial.print(";");
        mySerial.print("35.999035, -78.933841");
        mySerial.print(";");
        mySerial.print("35.999741, -78.931698");
        mySerial.print(";");
        mySerial.print("35.999862, -78.928914");
        mySerial.print(";");
        mySerial.print("36.000319, -78.926159");  
        mySerial.print(";");
        mySerial.print("35.999628, -78.921404");
        mySerial.print(";");
        mySerial.print("36.000036, -78.919934");
        mySerial.print(";");
        mySerial.print("36.001841, -78.918169");
        mySerial.print(";");
        mySerial.print("36.005096, -78.915948");      
        mySerial.print(";");
        mySerial.print("36.005252, -78.915326");      
        mySerial.print(";");
        mySerial.print("36.005291, -78.915133");      
        mySerial.print(";");
        mySerial.print("36.005918, -78.915092");      
        mySerial.print(";");
        mySerial.print("36.006007, -78.914754");      
        mySerial.print(";");
        mySerial.print("36.005914, -78.914376");      
        mySerial.print(";");
        mySerial.print("36.005313, -78.914403");      
        mySerial.print(";");
        mySerial.print("36.005157, -78.914752");      
        mySerial.print(";");
        mySerial.print("36.005298, -78.915143");      
        
        mySerial.print("#");
        
        // Speed
        mySerial.print("5.67");
        
        mySerial.print(",");
        
        // Distance
        mySerial.print("3.45");
        mySerial.print("!");
      }
    }
  }
}

void sendSDdata(){
  
  BTSerial.print("Data Received...");
        
  BTSerial.print("#");
  
  myFile = SD.open("GPSRMC.TXT");
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
    Serial.println("error opening test.txt");
  }  

  BTSerial.print("#");
    
    // Speed
  BTSerial.print("5.67");
  BTSerial.print(",");
    
    // Distance
  BTSerial.print("3.45");
  BTSerial.print("!");
}
