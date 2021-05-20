/*------------------------------------------------------------------------
  Example sketch for Adafruit Thermal Printer library for Arduino.
  Demonstrates a few text styles & layouts, bitmap printing, etc.

  IMPORTANT: DECLARATIONS DIFFER FROM PRIOR VERSIONS OF THIS LIBRARY.
  This is to support newer & more board types, especially ones that don't
  support SoftwareSerial (e.g. Arduino Due).  You can pass any Stream
  (e.g. Serial1) to the printer constructor.  See notes below.

  You may need to edit the PRINTER_FIRMWARE value in Adafruit_Thermal.h
  to match your printer (hold feed button on powerup for test page).
  ------------------------------------------------------------------------*/

#include "Adafruit_Thermal.h"
#include "logoV4.4.h"

String x;

String date = "";
String currentTime = "";
int transactionNo = -1;
String accountNo = "";
int cardNo = -1;
int amount = -1;


// Here's the new syntax when using SoftwareSerial (e.g. Arduino Uno) ----
// If using hardware serial instead, comment out or remove these lines:

#include "SoftwareSerial.h"
#define TX_PIN 5 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 6 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor
// Then see setup() function regarding serial & printer begin() calls.

// Here's the syntax for hardware serial (e.g. Arduino Due) --------------
// Un-comment the following line if using hardware serial:

//Adafruit_Thermal printer(&Serial1);      // Or Serial2, Serial3, etc.

// -----------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1000);
  getInfo();
  mySerial.begin(9600);
  printer.begin();
  
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
  printer.feed(1);
  

  printer.sleep();      // Tell printer to sleep
  delay(3000L);         // Sleep for 3 seconds
  printer.wake();       // MUST wake() before printing again, even if reset
  printer.setDefault(); // Restore printer to defaults
}

void loop() {
}

void getInfo(){
  int i = 0;
  while(!(date != "" && currentTime != "" && transactionNo != -1 && accountNo != "" && cardNo != -1 && amount != -1)){
    while (!Serial.available());
    x = Serial.readString();
    x = String(x);
    Serial.println(i);
    if(x != ""){
      if(x.indexOf("d") > -1){
        String temp = x.substring(2);
        date = temp;
        i++;
      } else if (x.indexOf("t") > -1){
        String temp = x.substring(2);
        currentTime = temp;
        i++;
      } else if (x.indexOf("o") > -1){
        int temp = x.substring(2).toInt();
        transactionNo = temp;
        i++;
      } else if (x.indexOf("a") > -1){
        String temp = x.substring(2);
        accountNo = temp;
        i++;
      } else if (x.indexOf("c") > -1){
        int temp = x.substring(2).toInt();
        cardNo = temp;
        i++;
      } else if (x.indexOf("h") > -1){
        int temp = x.substring(2).toInt();
        amount = temp;
        i++;
      }
    }
  }
}
