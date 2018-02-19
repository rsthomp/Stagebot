// This program receives and parses commands from an android device through bluetooth
// to control the motion of the Stagebot
// Author: Rachael T

#include <SoftwareSerial.h>

int maxSeconds = 10; // send status message every maxSeconds

int txPin = 3; // LED connected to pin 2 (on-board LED)
int rxPin =  2; 
int connectedPin = 4; //Reads high when bluetooth is connected
int statusLED = 13;

volatile int seconds = 0;
volatile int lmotor;
volatile int rmotor;

String inputString = "";
String command = "";
String value = "";
boolean stringComplete = false; //Verifies that all bytes have been recieved

volatile int lSpeed;
volatile int rSpeed;

SoftwareSerial SaberSerial = SoftwareSerial( rxPin, txPin );

void setup(){
  //Start serial connection
  pinMode(txPin, OUTPUT);
  pinMode(statusLED, OUTPUT);
  pinMode(connectedPin, INPUT);
  SaberSerial.begin(9600);
  
  Serial.begin(9600);
  Serial.print("Starting...");

  inputString.reserve(50);
  command.reserve(50);
  value.reserve(50);  
}

void loop(){

     int cnct = digitalRead(connectedPin);
     if(cnct == LOW){
        SaberSerial.write(byte(0));
      }
      //Send command to motor driver
      SaberSerial.write(lSpeed);
      SaberSerial.write(rSpeed);
}

// Runs whenever bytes are recieved
void serialEvent() {
  while (Serial.available()) {
    
    // get the new byte:
    char inChar = (char)Serial.read();
    
    if (inChar == '\n' || inChar == '\r') {
      stringComplete = true;  //message is over 
      break;
    }  

    // add it to the inputString:
     inputString += inChar; 
  }

  if (stringComplete) {

   //command structure is "CMD{Lmotor}z{Rmotor}z"
   if (inputString.startsWith("CMD") && inputString.endsWith("z")) {
      String inputL = "";
      int pos = 0;
      for(int i = 3; i < inputString.length(); i++){
        if (inputString.charAt(i) != 'z'){
          inputL += inputString.charAt(i);
        }
        else{
          pos = i;
          break;
        }
      }

      String inputR = "";
      for (int i = pos + 1; i < inputString.length(); i++) {
        if (inputString.charAt(i) != 'z'){
          inputR += inputString.charAt(i);
        }
        else{
          pos = i;
          break;
        }
      }

      lSpeed = inputL.toInt();
      rSpeed = inputR.toInt();

      inputString = "";
      stringComplete = false;
    } 
  }
   
}


