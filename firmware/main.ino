/*
 * MINTS
 * Written by Daniel Kiv
 * Version 0.1
 */

#include <math.h>
#include <stdio.h>
#include "Seeed_BMP280.h"
#include <Wire.h>
#include <LoRaWan.h>
#include "TinyGPS++.h"
#include <avr/dtostrf.h>
//#include "MutichannelGasSensor.h"

// gas sensors intializations
int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 1000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

// bmp280 initializations
float pressure;
BMP280 bmp280;

#define USE_GPS 1
#ifdef USE_GPS
#include "TinyGPS++.h"
TinyGPSPlus gps;
#endif

const int analogInPin = A0;  // Analog input pin that the VBAT pin is attached to
int BatteryValue = 0;        // value read from the VBAT pin
float outputValue = 0;        // variable for voltage calculation

void setup() {

  char buffer[256];

  delay(1000);
  SerialUSB.println("Starting...");
  delay(1000);

   char c;
#ifdef USE_GPS
   bool locked;
#endif

  Serial.begin(9600);  // for gps readings
  Serial1.begin(9600);
  SerialUSB.begin(115200);
  lora.init();
  lora.setDeviceReset();

  #ifdef USE_GPS
    Serial.begin(9600);     // open the GPS
    locked = false;

    // For S&G, let's get the GPS fix now, before we start running arbitary
    // delays for the LoRa section

    while (!gps.location.isValid()) {
      while (Serial.available() > 0) {
        if (gps.encode(c=Serial.read())) {
//          displayInfo();
          if (gps.location.isValid()) {
//            locked = true;
            break;
          }
        }
//        SerialUSB.print(c);
      }

//      if (locked)
//        break;

      if (millis() > 15000 && gps.charsProcessed() < 10)
      {
        SerialUSB.println(F("No GPS detected: check wiring."));
        SerialUSB.println(gps.charsProcessed());
        while(true);
      }
      else if (millis() > 20000) {
        SerialUSB.println(F("Not able to get a fix in alloted time."));
        break;
      }
    }
#endif

  pinMode(pin,INPUT);
  if(!bmp280.init()) {
    SerialUSB.println("Device error!");
  }
  starttime = millis();//get the current time;

  // LoRaWan initializations
  memset(buffer, 0, 256);
  lora.getVersion(buffer, 256, 1);
  SerialUSB.print(buffer);

  memset(buffer, 0, 256);
  lora.getId(buffer, 256, 1);
  SerialUSB.print(buffer);

  lora.setKey("2B7E151628AED2A6ABF7158809CF4F3C", "2B7E151628AED2A6ABF7158809CF4F3C", NULL);

  lora.setDeciveMode(LWABP);
  lora.setDataRate(DR0, US915HYBRID);
//    sendCommand("AT+DR=US915HYBRID\r\n");

  lora.setChannel(0, 902.3);
  lora.setChannel(1, 902.5);
  lora.setChannel(2, 902.7);

//    lora.setChannel(0, 902.3, DR0, DR3);
//    lora.setChannel(1, 902.5, DR0, DR3);
//    lora.setChannel(2, 902.7, DR0, DR3);
//    lora.setChannel(3, 902.9, DR0, DR3);
//    lora.setChannel(4, 903.1, DR0, DR3);
//    lora.setChannel(5, 903.3, DR0, DR3);
//    lora.setChannel(6, 903.5, DR0, DR3);
//    lora.setChannel(7, 903.7, DR0, DR3);

//    lora.setReceiceWindowFirst(false);
//    lora.setReceiceWindowFirst(0, 923.3);
//    lora.setReceiceWindowFirst(1, 923.9);
//    lora.setReceiceWindowFirst(2, 924.5);
//    lora.setReceiceWindowFirst(3, 925.1);
//    lora.setReceiceWindowFirst(4, 925.7);
//    lora.setReceiceWindowFirst(5, 926.3);
//    lora.setReceiceWindowFirst(6, 926.9);
//    lora.setReceiceWindowFirst(7, 927.5);

  lora.setReceiceWindowFirst(0, 902.3);
  lora.setReceiceWindowSecond(923.3, DR8);

  lora.setDutyCycle(false);
  lora.setJoinDutyCycle(false);

  lora.setPower(14);

  SerialUSB.println("System booted!");

}

void loop() {
  bool result = false;
  char cat[15];
  char com[2] = ",";
  char payload[128] = "";

  dtostrf(bmp280.getTemperature(), 0, 2, cat);
  strcat(payload,cat);
  strcat(payload,com);
  dtostrf(bmp280.getPressure(), 0, 2, cat);
  strcat(payload,cat);
  strcat(payload,com);
  dtostrf(bmp280.calcAltitude(bmp280.getPressure()), 0, 2, cat);
  strcat(payload,cat);
  strcat(payload,com);

//    // read the analog in value:
//  BatteryValue = analogRead(analogInPin);
//  // Calculate the battery voltage value
//  outputValue = (float(BatteryValue)*5)/1023*2 - 2.75;
//  // print the results to the SerialUSB monitor:
//  SerialUSB.print("Analog value = " );
//  SerialUSB.print(BatteryValue);
//  SerialUSB.println(outputValue);
//  strcat(payload,cat);
//  strcat(payload,com);
//  dtostrf(gps.altitude.meters(), 0, 6, cat);

  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  // SerialUSB.print("test");
  if ((millis()-starttime) > sampletime_ms)
  {
      ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
      concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
      dtostrf(lowpulseoccupancy, 0, 0, cat);
      strcat(payload,cat);
      strcat(payload,com);
      dtostrf(ratio, 0, 2, cat);
      strcat(payload,cat);
      strcat(payload,com);
      dtostrf(concentration, 0, 2, cat);
      strcat(payload,cat);
      strcat(payload,com);
      lowpulseoccupancy = 0;
      starttime = millis();
  }

  if (gps.location.isValid())
  {
    dtostrf(gps.location.lat(), 0, 6, cat);
    strcat(payload,cat);
    strcat(payload,com);
    dtostrf(gps.location.lng(), 0, 6, cat);
    strcat(payload,cat);
    strcat(payload,com);
    dtostrf(gps.altitude.meters(), 0, 6, cat);
    strcat(payload,cat);
//    strcat(payload,"\r\n");
  }
  else
  {
    strcat(payload,"INVALIDGPSDATA");
//    strcat(payload,com);
  }

// send payloads
  SerialUSB.println(payload);
//  sendCommand(payload);
  result = lora.transferPacket(payload);

  delay(500);

//    memset(payload, '\0', 200);
}

void barometer() {

  //sprintf (intStr, "%d", int1);
  //get and print temperatures
  SerialUSB.print("Temp: ");
  SerialUSB.print(bmp280.getTemperature());
  SerialUSB.println("C");

  // ---------------- WORK -------------

  //sendATcommand("AT+CMSG=Pressure,","Done",2000);

  //get and print atmospheric pressure data
  SerialUSB.print("Pressure: ");
  pressure = bmp280.getPressure();
  SerialUSB.print(pressure);
  SerialUSB.println("Pa");

  //get and print altitude data
  SerialUSB.print("Altitude: ");
  SerialUSB.print(bmp280.calcAltitude(pressure));
  SerialUSB.println("m");

  SerialUSB.println("\n");//add a line between output of different times.

  //delay(1000);
}

void dustSensor() {
  duration = pulseIn(pin, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;
    // SerialUSB.print("test");
    if ((millis()-starttime) > sampletime_ms)
    {
        ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
        SerialUSB.print(lowpulseoccupancy);
        SerialUSB.print(",");
        SerialUSB.print(ratio);
        SerialUSB.print(",");
        SerialUSB.println(concentration);
        lowpulseoccupancy = 0;
        starttime = millis();
    }
}
