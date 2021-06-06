#include<Stepper.h>
#include "Adafruit_Thermal.h"
#include "logoV4.4.h"

#include "SoftwareSerial.h"
#define TX_PIN 13 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 12 // Arduino receive   GREEN WIRE   labeled TX on printer

const int stepsPerRotation = 2048;
const int stepperSpeed = 14;

//Gezien vanaf de voorkant (IN1, IN3, IN2, IN4)
Stepper stepperMotorLeft = Stepper(stepsPerRotation, 14, 16, 15, 17); //pins from Mega
Stepper stepperMotorCenter = Stepper(stepsPerRotation, 7, 5, 6, 4); //pins from Mega
Stepper stepperMotorRight = Stepper(stepsPerRotation, 11, 9, 10, 8); //pins from Mega

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

//Functies initialiseren
void leftMotorOneBill();
void centerMotorOneBill();
void rightMotorOneBill();

String x;
int notes50 = -1;
int notes20 = -1;
int notes10 = -1;
void moneyDispenser();

String date = "";
String currentTime = "";
int transactionNo = -1;
String accountNo = "";
int cardNo = -1;
int amount = -1;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1000);
  mySerial.begin(9600);
  stepperMotorLeft.setSpeed(stepperSpeed); //10 euro
  stepperMotorCenter.setSpeed(stepperSpeed); //20 euro
  stepperMotorRight.setSpeed(stepperSpeed); //50 euro
  printer.begin();
  checkReceiptOrDispenser();
}

void loop() {
}

void checkReceiptOrDispenser(){
  int i = 9;
  while(true){
    x = Serial.readString();
    x = String(x);
    Serial.println(i);
    if(x != ""){
      if(x.indexOf("m") > -1){//m is money dispenser
        Serial.println(8);
        delay(100);
        moneyDispenser();
        //break;
      } else if (x.indexOf("r")){//r is receipt print
        Serial.println(8);
        receipt();
      }
    }
  }
}

void receipt(){
  int i = 0;
  boolean allData = false;
  while(!(allData)){
    while (!Serial.available());
    x = Serial.readString();
    x = String(x);
    Serial.println(i);
    if(x != ""){
      if(x.indexOf("d") > -1){ //d is date
        String temp = x.substring(2);
        date = temp;
        i++;
      } else if (x.indexOf("t") > -1){ //t is time
        String temp = x.substring(2);
        currentTime = temp;
        i++;
      } else if (x.indexOf("o") > -1){ // o is transaction nO
        int temp = x.substring(2).toInt();
        transactionNo = temp;
        i++;
      } else if (x.indexOf("a") > -1){ //a is AccountNo
        String temp = x.substring(2);
        accountNo = temp;
        i++;
      } else if (x.indexOf("c") > -1){ //c is cardNo
        int temp = x.substring(2).toInt();
        cardNo = temp;
        i++;
      } else if (x.indexOf("h") > -1){ //h 
        int temp = x.substring(2).toInt();
        amount = temp;
        i++;
        allData = true;
      }
    }
  }
  printReceipt();
}

void printReceipt(){
  //Prints header (Logo and name)
  printer.printBitmap(320, 97, logoV4);
  printer.justify('C');
  printer.boldOn();
  printer.setSize('L');
  printer.println(F("ABN - MANBRO"));
  printer.boldOff();
  printer.setSize('S');
  printer.underlineOn();
  printer.println(F("                                "));
  printer.underlineOff();
  
  //Date and time information
  printer.justify('L');
  printer.print("Datum: ");
  printer.print(date);
  printer.print("   Tijd: ");
  printer.println(currentTime);
  printer.justify('C');
  printer.underlineOn();
  printer.println(F("                                "));
  printer.underlineOff();
  
  //Machine, transaction number information
  printer.justify('L');
  printer.println("Automaat #:    88");
  printer.print("Transactie #:  ");
  printer.println(transactionNo);
  //Account information
  printer.print("Rekening #:    XXXX-XX");
  printer.println(accountNo);
  
  //Card number
  printer.print("Pas #:         ");
  printer.println(cardNo);
  printer.print("Bedrag:        ");
  printer.print(amount);
  printer.println(",-");
  printer.justify('C');
  printer.underlineOn();
  printer.println(F("                                "));
  printer.underlineOff();
  printer.justify('L');
  //Ending with a greet
  printer.println("Bedankt voor het gebruiken van  de services van ABN-MANBRO");
  printer.println("GRAAG TOT ZIENS");
  //Rolls out 
  printer.feed(2);
  

  printer.sleep();      // Tell printer to sleep
  delay(3000L);         // Sleep for 3 seconds
  printer.wake();       // MUST wake() before printing again, even if reset
  printer.setDefault(); // Restore printer to defaults
}



void moneyDispenser(){
  int i = 0;
  boolean allBills = false;
  while(!allBills){
    while (!Serial.available());
    x = Serial.readString();
    x = String(x);
    Serial.println(i);
    if(x != ""){
      if(x.indexOf("f") > -1){ //f = notes50
        String temp = x.substring(2);
        if(notes50 != -1){
          notes50 = temp.toInt();
        }
        else {
          notes50 = 0;
        }
        i++;
        for(int j = 0; j < notes50; j++){
          rightMotorOneBill();
        }
      } else if (x.indexOf("s") > -1){ //s = notes20
        String temp = x.substring(2);
        notes20 = temp.toInt();
        i++;
        for(int j = 0; j < notes20; j++){
          centerMotorOneBill();
        }
      } else if (x.indexOf("t") > -1){ //t = notes10
        String temp = x.substring(2);
        notes10 = temp.toInt();
        i++;
        for(int j = 0; j < notes10; j++){
          leftMotorOneBill();
        }
        allBills = true;
      }
    }
  }
}


void leftMotorOneBill() {
  stepperMotorLeft.step(-stepsPerRotation);
}

void centerMotorOneBill() {
  stepperMotorCenter.step(-stepsPerRotation);  
}

void rightMotorOneBill() {
  stepperMotorRight.step(stepsPerRotation);
}
