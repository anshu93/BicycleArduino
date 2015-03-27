#include <SoftwareSerial.h>
#include <SD.h>

#define SD_CARD_OFF false  // True = debugging with SD card

#define CS_PIN 53  // CS pin of SD card

char data;
SoftwareSerial mySerial(12, 13); // RX, TX for BT module
File myFile;  // For SD card

void setup()
{
  // Open serial communications and wait for port to open:
  
  mySerial.begin(9600);
  Serial.begin(9600);
  

  // Set up SD card
  
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(CS_PIN, OUTPUT);
   
  if (!SD.begin(CS_PIN)) {
    mySerial.println("initialization failed!");
    return;
  }
  mySerial.println("initialization done.");
}

void loop(){ 
  
  
  if(mySerial.available()){
    data = mySerial.read();

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
}

void sendSDdata(){
  
  mySerial.print("Data Received...");
        
  mySerial.print("\n");
  
  myFile = SD.open("GPSRMC.TXT");
  if(myFile){
    while(myFile.available()){
      char c = myFile.read();
      if(c == '\n'){
        c = ';';  
      }
      mySerial.print(c);  
    }
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    mySerial.println("error opening test.txt");
  }  

  mySerial.print("#");
    
    // Speed
  mySerial.print("5.67");
  mySerial.print(",");
    
    // Distance
  mySerial.print("3.45");
  mySerial.print("!");
}


