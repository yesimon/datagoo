/*
DataGoo Firmware
Written by Ravi Sankar, Simon Ye, and Nathan Hall-Snyder
3/21/12

Description: This code is primarily designed for an open source energy-monitoring board, which
you can find details for at http://datagoo.readthedocs.org. However, it
can also run on a Cellular Shield from SparkFun Electronics, an SD Card Shield from Adafruit, and
an Arduino board. Details on those parts are also available at the readthedocs link.

An activated SIM card must be inserted into the SIM card holder on the board in order to use the device!
The device also requires a writeable SD card.

This sketch utilizes the following external libraries:
The EmonLib library written by Trystan Lea of OpenEnergyMonitor. The library can be downloaded at this URL:
https://github.com/openenergymonitor/EmonLib
The MsTimer2 library written by Javier Valencia. The library can be downloaded at this URL:
http://arduino.cc/playground/Main/MsTimer2
The JeeLib libraries written by the folks at JeeLabs.net. The libraries can be found from:
http://jeelabs.net/projects/cafe/wiki

and the following libraries provided with the Arduino platform:
The SoftwareSerial library: http://arduino.cc/en/Reference/SoftwareSerial
The SD library: http://arduino.cc/en/Reference/SD
The string library: http://arduino.cc/en/Reference/string
The Arduino Watchdog library 

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use our code freely! Please just remember to give us credit where it's due. Thanks!)
*/

#include <Arduino.h>
#include <string.h>         //Used for string manipulations
#include <SD.h>             //SD card interface
#include <SoftwareSerial.h> //Include the SoftwareSerial library to send serial commands to the cellular module.
//#include <avr/wdt.h>
#include "EmonLib.h"
#include "MsTimer2.h"
#include "LedDisplay.h"

#define sdPin 10
#define currentPin 0
#define voltPin 1

#define SERIAL_ON 0 //set this to 1 if you want serial printing on (for debug)
#define NUM_CAL_CONSTS 4 //the number of values in the calibration file
#define MAX_PHONE_NUMS 5

#define TEXT_INTERVAL 60000 //ms: text every minute (for demo; deploy should probably be daily)
#define LOG_INTERVAL  10000 //ms: log every 10s
#define MILLIS_PER_DAY 86400000

SoftwareSerial cell(2,3);  //Create a 'fake' serial port. Pin 2 is the Rx pin, pin 3 is the Tx pin.

String mobileNumber = ""; //phone number to text status updates to
String inputString  = ""; //used to read input from the cell phone module
String timeString   = ""; //time string pulled from GSM module

int numTextsSent = 0;

float sumPower = 0.0;
int numPowerSamples = 0;

float calibrationConstants[NUM_CAL_CONSTS] = {238.5, 1.7, 75.71, 7}; //default calibration (volt, phase shift, current, gsm sband)
File logFile; //the file on the SD card that we are writing to
boolean sdAvailable = true;

long lastLoggedTime = 0; //in millis
long lastTextedTime = 0; //in millis
float lastVoltReading = 0; //to check for voltage drop

EnergyMonitor emon1; //used to do the actual energy calculations

//ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void parseCalibrationString(String calibrationString);
void startSMS(String mobileNumber);
void endSMS();
void sendText(String msg);
void cellReadLine();
void writeLogHeader(); 
void writeLogEntry(long time);
void setup();
void loop();

void setup()
{
  display_init();  // Initialize LED Display
  MsTimer2::set(10, display_switch_digit); // 10ms period on displaying digits
  MsTimer2::start();
  
  //Initialize serial ports for communication.
  Serial.begin(9600);
  cell.begin(9600);

  //Wait until network registration is complete
  delay(25000);

  //Initialize SD card
  pinMode(sdPin, OUTPUT);
  if (!SD.begin(sdPin)) {
    sdAvailable = false;
    if (SERIAL_ON) Serial.println("initialization failed!");
  }
  
  if (sdAvailable) {
    if (SERIAL_ON) Serial.println("initialization done.");
  
    //read the cell number to text off the SD card
    File cellFile = SD.open("cell.txt");
    if (cellFile) {
      while (cellFile.available()) {
        char c = (char) cellFile.read();
        if (c == '#') break;
        mobileNumber += c;
      }
      cellFile.close();
      mobileNumber.trim(); //get rid of whitespace
    } else {
      if (SERIAL_ON) Serial.println("Please make a file called CELL.TXT on the SD card containing the phone number you would like the power logger to text with daily stats");
    }
    
    //create the log file if necessary, with a header
    if (!SD.exists("log.csv")) {
      logFile = SD.open("log.csv", FILE_WRITE);
      if (logFile) writeLogHeader();
      logFile.close();
    }
    
    //setup power calibration numbers
    File calibrationFile = SD.open("cal.txt");
    String calibrationInput = "";
    if (calibrationFile) {
      while (calibrationFile.available()) {
        char c = (char) calibrationFile.read();
        if (c == '#') break;
        calibrationInput += c;
      }
      calibrationFile.close();
      calibrationInput.trim(); //get rid of whitespace
      parseCalibrationString(calibrationInput);
    }
  }

  int sband = (int) calibrationConstants[3]; //default: 7
  cell.println("AT+SBAND=" + String(sband)); //for Guatemala, we think SBAND=3 (GSM850) but double check the carrier frequency against the AT command set
  cell.println("AT+CMGF=1"); //Set GSM module on text (as opposed to voice) mode
  //cell.println("AT+CNMI=3,3,0,0"); //Set text messages to output to serial if you want to be able to use texts as input to DataGoo

  sendText("Hi, I'm DataGoo. Your number has been registered with me to receive updates on generator statistics.");

  // Voltage/Current calibration
  emon1.voltage(voltPin, calibrationConstants[0], calibrationConstants[1]);  // Voltage: input pin, volt_calibration, phase_shift
  emon1.current(currentPin, calibrationConstants[2]);       // Current: input pin, current_calibration
  
  display_write(0); //override the "88" that shows up on the display at startup
  emon1.calcVI(20,2000); //the very first reading after reset is always junk, so just take one reading and toss the result
}

void loop() {
  emon1.calcVI(100,20000); // measure 100 wavelengths, waiting for at 20secs (20*1000ms)
  display_write((int) emon1.apparentPower / 1000); //convert W to kW and display

  long now = millis();
  sumPower += emon1.apparentPower;
  numPowerSamples++;
  
  //check for and alert on voltage drops
  if ((emon1.Vrms < lastVoltReading / 2) && (lastVoltReading - emon1.Vrms > 10)) {
    String curVoltage = String((long)emon1.Vrms);
    sendText("There appears to be an error with the generator. Its voltage has dropped to " + curVoltage);
  }
  lastVoltReading = emon1.Vrms;

  //send a text on power readings
  if (now - lastTextedTime > TEXT_INTERVAL) {
    if (numTextsSent < 5) { //for demo purposes
      float powerAvg = sumPower / numPowerSamples;
      int truncatedAvg = (long) powerAvg;
      sendText("Power generated (watts): " + String(truncatedAvg));
      numTextsSent++;
      sumPower = 0.0;
      numPowerSamples = 0;
      lastTextedTime = now;
    }
  }
  //log power readings to the SD card
  if (now - lastLoggedTime > LOG_INTERVAL) { //24 hrs * 60 mins * 60 secs * 1000 ms = 86,400,000 millisecs/day
    logFile = SD.open("log.csv", FILE_WRITE); //Open the log file on SD card
    if (logFile) {
      writeLogEntry(now);
      logFile.close();
      lastLoggedTime = now;
    }
  }

  //sample code to read texts to the device as input
  /*if (cell.available()) {
    cellReadLine();
    if (inputString.startsWith("datagootext")) {
      //dosomething
    }
    inputString = "";
  }*/
} 
  
/*-------------------------------------------------------------------
 * Helper Functions
 * ------------------------------------------------------------------ */

/* Converts the text from CAL.TXT into an array of 4 floats
 * (volt_calibration, phase_shift, current_calibration, and SBAND).
 * This last one is actually an int but is grouped her for convenience.
 */
void parseCalibrationString(String calibrationString) {
  calibrationString += '\n';
  int calStringLen = calibrationString.length();
  int i = 0;
  int j = 0;
  String calibrationConstantTemp = "";

  for (i=0; i < calStringLen; i++) {
    char calArray[15];
    char c = calibrationString[i]; //read a character at a time from the input string
    if (c == '\n') { //separate inputs based on newlines
      calibrationConstantTemp.toCharArray(calArray, 14);
      calibrationConstants[j] = atof(calArray); //and convert the strings to floats
      calibrationConstantTemp = "";
      j++;
      if (j >= NUM_CAL_CONSTS) return; //don't read more lines than expected
    }
    else if (c == ' ') { //ignore this whitespace
      continue;
    }
    else {
      calibrationConstantTemp += c;
    }
  } 
}

/* Converts the text from CELL.TXT into an array of up to 5 phone numbers
 * to send text updates to.
 */
//void parseMobileNumbers(String cellString) {
//  cellString += '\n';
//  int cellStringLen = cellString.length();
//  int i = 0;
//  int j = 0;
//  String temp = "";
//
//  //initialize the array
//  for (int k=0; k < MAX_PHONE_NUMS; k++) {
//    mobileNumbers[k] = "";
//  }
//
//  //now populate it
//  for (i=0; i < cellStringLen; i++) {
//    char c = cellString[i]; //read a character at a time from the input string
//    if (c == '\n') { //separate inputs based on newlines
//      mobileNumbers[j] += temp;
//      temp = "";
//      j++;
//      if (j >= MAX_PHONE_NUMS) return; //don't read more lines than expected
//    }
//    else if (c == ' ') { //ignore this whitespace
//      continue;
//    }
//    else {
//      temp += c;
//    }
//  }
//}

/* These functions basically print some magic strings to the cell module
 * to send texts. The information on what is expected is found here:
 * http://www.sparkfun.com/datasheets/CellularShield/SM5100B%20AT%20Command%20Set.pdf
 */
void startSMS(String mobileNumber)
{
  if (SERIAL_ON) Serial.println("starting SMS");
  //cell.println("AT+CMGF=1"); // set SMS mode to text
  cell.print("AT+CMGS=");
  cell.write(34); // ASCII equivalent of "
  cell.print(mobileNumber);
  cell.write(34);  // ASCII equivalent of "
  cell.println();
  delay(500); // give the module some thinking time
}
void endSMS()
{
  if (SERIAL_ON) Serial.println("ending SMS");
  cell.write(26);  // ASCII equivalent of Ctrl-Z
  delay(5000); //give the module some thinking time
}

/*
 * Use this function to text (over GSM) the string 'msg' to all the cell
 * phone numbers in mobileNumbers.
 *
 * startSMS and endSMS are helper functions for this function and shouldn't
 * be required by anything external (like the main loop)
 */
void sendText(String msg) {
  startSMS(mobileNumber);
  cell.print(msg);
  endSMS();
}


/*
 * Repeatedly tries to read a character from the cell phone's serial output
 * until it sees an end-of-line character (\r) or the serial output overflows.
 * This allows us to deal with lines and responses to our commands, rather than
 * the individual characters that the Serial interface returns
 */
void cellReadLine() {
  long lastReadT = millis(); //keep track of when we started waiting for input

  while (1) {
    if (cell.available()) { //if there is available input from the cell
      char c = (char) cell.read(); //read it
      lastReadT = millis(); //and update our timer

      if (c == -1 || c == '\n') { //ignore these characters
        continue;
      }
      if (c == '\r' || cell.overflow()) { //and return in these cases
        return;
      }

      inputString += c; //add the character we just read to our string
    }

    //don't loop waiting for a complete line for more than a minute
    if (millis() - lastReadT > 60000) {
      return;
    }
  }
}

/* Write CSV header to SD card if the logFile is open
 */
void writeLogHeader() {
  logFile.print("Time (relative to startup)");
  logFile.print(",");
  logFile.print("Voltage");
  logFile.print(",");
  logFile.print("Current");
  logFile.print(",");
  logFile.print("Power");
  logFile.println();
}

/* Write CSV entry to SD card if the logFile is open
 */
void writeLogEntry(long time) {
  int dayNum = ((int) time/MILLIS_PER_DAY) + 1; //day 1 is the day of startup
  int secsOfDay = (long) (time % MILLIS_PER_DAY) / 1000; //number of seconds into this day
  String timeString = "day " + String(dayNum) + " and " + String(secsOfDay) + " seconds";
  logFile.print(timeString);
  logFile.print(",");
  logFile.print(emon1.Vrms);
  logFile.print(",");
  logFile.print(emon1.Irms);
  logFile.print(",");
  logFile.print(emon1.apparentPower);
  logFile.println();
}

