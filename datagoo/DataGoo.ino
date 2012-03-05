/*
SparkFun Cellular Shield - Pass-Through Sample Sketch
SparkFun Electronics
Written by Ryan Owens
3/8/10

Description: This sketch is written to interface an Arduino Duemillanove to a  Cellular Shield from SparkFun Electronics.
The cellular shield can be purchased here: http://www.sparkfun.com/commerce/product_info.php?products_id=9607
In this sketch serial commands are passed from a terminal program to the SM5100B cellular module; and responses from the cellular
module are posted in the terminal. More information is found in the sketch comments.

An activated SIM card must be inserted into the SIM card holder on the board in order to use the device!

This sketch utilizes the SoftwareSerial library written by Mikal Hart of Arduiniana. The library can be downloaded at this URL:
http://arduiniana.org/libraries/SoftwareSerial/

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use our code freely! Please just remember to give us credit where it's due. Thanks!)
*/

#include <SoftwareSerial.h> //Include the SoftwareSerial library to send serial commands to the cellular module.
#include <string.h>         //Used for string manipulations
#include <SD.h>             //SD card interface
#include <Wire.h>           //Used to interact with Real Time Clock
#include <RTClib.h>         //Real Time Clock
//#include <JeeLib.h>
//#include <avr/wdt.h>
#include "EmonLib.h"
EnergyMonitor emon1;

String inputString = "";

int numTextsSent = 0;
char ctrlZ = (char)0x1A;

SoftwareSerial cell(2,3);  //Create a 'fake' serial port. Pin 2 is the Rx pin, pin 3 is the Tx pin.

File logFile;
long lastLoggedTime = 0;
RTC_DS1307 RTC;

//ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void setup()
{
  //Initialize serial ports for communication.
  Serial.begin(9600);
  //cell.begin(9600);
  
  //Let's get started!
  Serial.println("Starting SM5100B Communication...");
  
  //Wait until network registration before entering main loop
  //delay(25000);
  //NetworkSetup();
  //Serial.println("Network connection found!");
  
  //Initialize SD card
  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  //Initialize RTC
  Wire.begin();
  RTC.begin();
  
  //Set system on text mode
  //cell.println("AT+CMGF=1");
}

void loop() {
  
  //------------------------------------------------
  // MEASURE FROM CT'S
  // Calibration notes:
  // 212.6 is a suitable calibration value for the Euro AC-AC adapter http://uk.rs-online.com/web/p/products/459-853/
  // 237.3 is a suitable calibration value for the UK AC-AC adapter http://uk.rs-online.com/web/p/products/400-6484/
  //------------------------------------------------
  // Example Calibration code - deprecated
  //   emon1.setPins(2,0);                    //emonTX AC-AC voltage (ADC2), current pin (CT1 - ADC3) 
  //   emon1.calibration(238.5, 138.8,1.7);   //voltage calibration , current calibration, power factor calibration. See: http://openenergymonitor.org/emon/emontx/acac
  // Calibrate instead according to https://github.com/openenergymonitor/EmonLib/blob/master/EmonLib.cpp
  emon1.calcVI(20,2000);               //No.of wavelengths, time-out , emonTx supply voltage 
  emon1.serialprint();
  //String logEntry = String(emon1.realPower) + ' ' + String(emon1.apparentPower) + ' ' + String(emon1.Vrms) + ' ' + String(emon1.Irms) + ' ' + String(emon1.powerFactor);
  

  
  
  DateTime now = RTC.now();
  long timeDiff = now.unixtime() - lastLoggedTime;
  Serial.println("timeDiff is " + String(timeDiff));
  if (timeDiff > 10) { //24 hrs * 60 mins * 60 secs = 86400 secs/day
    //Open or create log file on SD card
    logFile = SD.open("log.csv", FILE_WRITE);
    if (logFile) {
      Serial.println("writing entry to log");
      writeLogEntry("Seconds are " + String(now.second()));
      logFile.close();
      lastLoggedTime = now.unixtime();
    } else {
      Serial.println("error openening log.csv");
    }
  }
  
  //repeatedly read any input from the cellphone
  /*cellReadLine();
  Serial.println(inputString);
  inputString = "";
  
  //send some test texts
  if (numTextsSent < 2) {
    cell.println("AT+CCLK?"); //clocktime
    cellReadLine();
    SendText("6503845765", inputString);
    numTextsSent++;
    inputString = "";
  }*/
}


/*-------------------------------------------------------------------
 * Helper Functions
 * ------------------------------------------------------------------ */

void startSMS(String mobileNumber)
// function to send a text message
{
  Serial.println("starting SMS");
  cell.println("AT+CMGF=1"); // set SMS mode to text
  cell.print("AT+CMGS=");
  cell.write(34); // ASCII equivalent of "
  cell.print(mobileNumber);
  cell.write(34);  // ASCII equivalent of "
  cell.println();
  delay(500); // give the module some thinking time
}
void endSMS()
{
  Serial.println("ending SMS");
  cell.write(26);  // ASCII equivalent of Ctrl-Z
  delay(15000); // the SMS module needs time to return to OK status
}

void SendText(String mobileNumber, String msg) {
  startSMS(mobileNumber);
  cell.print(msg);
  endSMS();
}

void NetworkSetup() {
  while(1) {
    cell.println("AT+CREG?");
    cellReadLine();
    Serial.println(inputString);
    if (inputString == "+CREG: 0,1") {
      return;
    }
    inputString = "";
    delay(5000);
  }
}

void cellReadLine() {
  while (1) {
    if (cell.available()) {
      char c = cell.read();
      
      if (c == -1 || c == '\n') {
        continue;
      }
      if (cell.overflow()) {
        Serial.println("cell overflow");
      }
      if (c == '\r' || cell.overflow()) {
        return;
      }
      
      inputString += c;
    }
  }
}

void writeLogEntry(String logEntry) {
  logFile.println(logEntry);
}


/* SM5100B Quck Reference for AT Command Set
*Unless otherwise noted AT commands are ended by pressing the 'enter' key.

1.) Make sure the proper GSM band has been selected for your country. For the US the band must be set to 7.
To set the band, use this command: AT+SBAND=7

2.) After powering on the Arduino with the shield installed, verify that the module reads and recognizes the SIM card.
With a terimal window open and set to Arduino port and 9600 buad, power on the Arduino. The startup sequence should look something
like this:

Starting SM5100B Communication...
    
+SIND: 1
+SIND: 10,"SM",1,"FD",1,"LD",1,"MC",1,"RC",1,"ME",1

Communication with the module starts after the first line is displayed. The second line of communication, +SIND: 10, tells us if the module
can see a SIM card. If the SIM card is detected every other field is a 1; if the SIM card is not detected every other field is a 0.

3.) Wait for a network connection before you start sending commands. After the +SIND: 10 response the module will automatically start trying
to connect to a network. Wait until you receive the following repsones:

+SIND: 11
+SIND: 3
+SIND: 4

The +SIND response from the cellular module tells the the modules status. Here's a quick run-down of the response meanings:
0 SIM card removed
1 SIM card inserted
2 Ring melody
3 AT module is partially ready
4 AT module is totally ready
5 ID of released calls
6 Released call whose ID=<idx>
7 The network service is available for an emergency call
8 The network is lost
9 Audio ON
10 Show the status of each phonebook after init phrase
11 Registered to network

After registering on the network you can begin interaction. Here are a few simple and useful commands to get started:

To make a call:
AT command - ATDxxxyyyzzzz
Phone number with the format: (xxx)yyy-zzz

If you make a phone call make sure to reference the devices datasheet to hook up a microphone and speaker to the shield.

To send a txt message:
AT command - AT+CMGF=1
This command sets the text message mode to 'text.'
AT command = AT+CMGS="xxxyyyzzzz"(carriage return)'Text to send'(CTRL+Z)
This command is slightly confusing to describe. The phone number, in the format (xxx)yyy-zzzz goes inside double quotations. Press 'enter' after closing the quotations.
Next enter the text to be send. End the AT command by sending CTRL+Z. This character can't be sent from Arduino's terminal. Use an alternate terminal program like Hyperterminal,
Tera Term, Bray Terminal or X-CTU.

The SM5100B module can do much more than this! Check out the datasheets on the product page to learn more about the module.
*/
