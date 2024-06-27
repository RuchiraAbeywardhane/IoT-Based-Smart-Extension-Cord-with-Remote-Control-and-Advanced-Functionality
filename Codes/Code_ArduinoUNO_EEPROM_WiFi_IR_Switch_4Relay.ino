/**********************************************************************************
Tronic Pros
 **********************************************************************************/

#include <EEPROM.h>
#include <IRremote.h>
#include <SoftwareSerial.h>
#include <arduino-timer.h>

auto timer = timer_create_default(); // create a timer with default settings

SoftwareSerial mySerial(2, 3); // RX, TX D2, D3

// define the GPIO connected with Relays and switches
#define RelayPin1 4  //D4
#define RelayPin2 5  //D5
#define RelayPin3 6  //D6
#define RelayPin4 7  //D7

#define SwitchPin1 10  //D10
#define SwitchPin2 11  //D11
#define SwitchPin3 12  //D12
#define SwitchPin4 13  //D13

#define IR_RECV_PIN A0  //A0
#define WIFI_LED A3  //A3

//Update the HEX code of IR Remote 0x<HEX CODE>
#define IR_Button_1   0x80BF49B6
#define IR_Button_2   0x80BFC936
#define IR_Button_3   0x80BF33CC
#define IR_Button_4   0x80BF718E
#define IR_All_Off    0xFFFFFFFF
#define IR_All_On     0x80BFF10E

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

// Switch State
bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;
bool SwitchState_3 = LOW;
bool SwitchState_4 = LOW;

String bt_data; // variable for storing bluetooth data
String pinStatus = "0000";

void relayOnOff(int relay){
 switch(relay){
      case 1:
            digitalWrite(RelayPin1, !digitalRead(RelayPin1)); // change state for relay-1
            EEPROM.update(0,digitalRead(RelayPin1));
            delay(100);
      break;
      case 2:
            digitalWrite(RelayPin2, !digitalRead(RelayPin2)); // change state for relay-2
            EEPROM.update(1,digitalRead(RelayPin2));
            delay(100);
      break;
      case 3:
            digitalWrite(RelayPin3, !digitalRead(RelayPin3)); // change state for relay-3
            EEPROM.update(2,digitalRead(RelayPin3));
            delay(100);
      break;
      case 4:
            digitalWrite(RelayPin4, !digitalRead(RelayPin4)); // change state for relay-4
            EEPROM.update(3,digitalRead(RelayPin4));
            delay(100);
      break;
      default : break;      
      }  
}

void eepromState()
{
  digitalWrite(RelayPin1, EEPROM.read(0)); delay(200);
  digitalWrite(RelayPin2, EEPROM.read(1)); delay(200);
  digitalWrite(RelayPin3, EEPROM.read(2)); delay(200);
  digitalWrite(RelayPin4, EEPROM.read(3)); delay(200);
}  

void wifi_control()
{
  if(mySerial.available()) {
    bt_data = mySerial.readString(
      
    );
    Serial.println(bt_data);
        if(bt_data == "A1") { digitalWrite(RelayPin1, LOW);  EEPROM.update(0,LOW); } // if 'A1' received Turn on Relay1
        if(bt_data == "A0") { digitalWrite(RelayPin1, HIGH); EEPROM.update(0,HIGH); } // if 'A0' received Turn off Relay1
        if(bt_data == "B1") { digitalWrite(RelayPin2, LOW);  EEPROM.update(1,LOW); } // if 'B1' received Turn on Relay2
        if(bt_data == "B0") { digitalWrite(RelayPin2, HIGH); EEPROM.update(1,HIGH); } // if 'B0' received Turn off Relay2
        if(bt_data == "C1") { digitalWrite(RelayPin3, LOW);  EEPROM.update(2,LOW); } // if 'C1' received Turn on Relay3
        if(bt_data == "C0") { digitalWrite(RelayPin3, HIGH); EEPROM.update(2,HIGH); } // if 'C0' received Turn off Relay3
        if(bt_data == "D1") { digitalWrite(RelayPin4, LOW);  EEPROM.update(3,LOW); } // if 'D1' received Turn on Relay4
        if(bt_data == "D0") { digitalWrite(RelayPin4, HIGH); EEPROM.update(3,HIGH); } // if 'D0' received Turn off Relay4
        if(bt_data == "W1") { digitalWrite(WIFI_LED, HIGH); } // if 'W1' Turn on WiFi LED
        if(bt_data == "W0") { digitalWrite(WIFI_LED, LOW); } // if 'W0' Turn off WiFi LED
        delay(20);
  }
}

void ir_remote(){
  if (irrecv.decode(&results)) {
    
      switch(results.value){
          case IR_Button_1:  relayOnOff(1);     break;
          case IR_Button_2:  relayOnOff(2);     break;
          case IR_Button_3:  relayOnOff(3);     break;
          case IR_Button_4:  relayOnOff(4);     break;
          case IR_All_Off:   all_Switch_OFF();  break;
          case IR_All_On:    all_Switch_ON();   break;
          default : break;         
        }   
        //Serial.println(results.value, HEX);    
        irrecv.resume();   
  } 
}

void all_Switch_ON(){
  digitalWrite(RelayPin1, LOW); EEPROM.update(0,LOW); delay(100);
  digitalWrite(RelayPin2, LOW); EEPROM.update(1,LOW); delay(100);
  digitalWrite(RelayPin3, LOW); EEPROM.update(2,LOW); delay(100);
  digitalWrite(RelayPin4, LOW); EEPROM.update(3,LOW); delay(100);
}

void all_Switch_OFF(){
  digitalWrite(RelayPin1, HIGH); EEPROM.update(0,HIGH); delay(100);
  digitalWrite(RelayPin2, HIGH); EEPROM.update(1,HIGH); delay(100);
  digitalWrite(RelayPin3, HIGH); EEPROM.update(2,HIGH); delay(100);
  digitalWrite(RelayPin4, HIGH); EEPROM.update(3,HIGH); delay(100);
}

void manual_control()
{
  if (digitalRead(SwitchPin1) == LOW && SwitchState_1 == LOW) {
    digitalWrite(RelayPin1, LOW);
    EEPROM.update(0,LOW);
    SwitchState_1 = HIGH;
  }
  if (digitalRead(SwitchPin1) == HIGH && SwitchState_1 == HIGH) {
    digitalWrite(RelayPin1, HIGH);
    EEPROM.update(0,HIGH);
    SwitchState_1 = LOW;
  }
  if (digitalRead(SwitchPin2) == LOW && SwitchState_2 == LOW) {
    digitalWrite(RelayPin2, LOW);
    EEPROM.update(1,LOW);
    SwitchState_2 = HIGH;
  }
  if (digitalRead(SwitchPin2) == HIGH && SwitchState_2 == HIGH) {
    digitalWrite(RelayPin2, HIGH);
    EEPROM.update(1,HIGH);
    SwitchState_2 = LOW;
  }
  if (digitalRead(SwitchPin3) == LOW && SwitchState_3 == LOW) {
    digitalWrite(RelayPin3, LOW);
    EEPROM.update(2,LOW);
    SwitchState_3 = HIGH;
  }
  if (digitalRead(SwitchPin3) == HIGH && SwitchState_3 == HIGH) {
    digitalWrite(RelayPin3, HIGH);
    EEPROM.update(2,HIGH);
    SwitchState_3 = LOW;
  }
  if (digitalRead(SwitchPin4) == LOW && SwitchState_4 == LOW) {
    digitalWrite(RelayPin4, LOW);
    EEPROM.update(3,LOW);
    SwitchState_4 = HIGH;
  }
  if (digitalRead(SwitchPin4) == HIGH && SwitchState_4 == HIGH) {
    digitalWrite(RelayPin4, HIGH);
    EEPROM.update(3,HIGH);
    SwitchState_4 = LOW;
  }
}

void sendStatus(){  
  pinStatus = String(!digitalRead(RelayPin1)) + String(!digitalRead(RelayPin2)) + String(!digitalRead(RelayPin3)) + String(!digitalRead(RelayPin4));
  mySerial.print(pinStatus);
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);
  digitalWrite(WIFI_LED, LOW);
  
  delay(500);
  eepromState();
  // call the toggle_led function every 2000 millis (2 seconds)
  timer.every(2000, sendStatus);  
}

void loop() {

  ir_remote();
  wifi_control();
  manual_control();

  timer.tick(); // tick the timer
}
