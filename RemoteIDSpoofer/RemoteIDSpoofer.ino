
// ESP8266 RemoteID spoofer

#include "frontend.h"
#include "spoofer.h"
#include <TinyGPS++.h> // library for GPS module
#include <SoftwareSerial.h>

TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device

float latitude , longitude;
int year, month, day, hour, minute, second;
int gotGPS = 0;
int isNewPilotLocation = 1;
int setTakeOffLocation = 0;
const int num_spoofers = 4;
int gpsLock = 1;
int satelliteCount = 0;
Spoofer spoofers[num_spoofers];

void setup() {
  Serial.begin(115200);
  ss.begin(9600);  

  // instantiate the spoofers and change locations
  Serial.println("Waiting on GPS");
  for (int i = 0; i < num_spoofers; i++) {
    spoofers[i].init();
  }
}

void loop() {
  // do the spoofing
  while (ss.available() > 0) //while data is available
  {
    if (gps.encode(ss.read())) //read gps data
    {
      if (gps.location.isValid()) //check whether gps location is valid
      {
        float newLatitude = gps.location.lat();
        float newLongitude = gps.location.lng();

        float kmDistance = gps.distanceBetween(
          newLatitude,
          newLongitude,
          latitude,
          longitude) / 1000.0;

        isNewPilotLocation = (kmDistance > 0.25 ? 1: 0);

        if (isNewPilotLocation) {
          latitude = newLatitude;
          longitude = newLongitude;
          Serial.println("New Pilot Location");
          if (!setTakeOffLocation) {
            Serial.println("Setting Take Off Location");
            for (int i = 0; i < num_spoofers; i++) {
              spoofers[i].updateLocation(latitude, longitude);
            }
          }
          else {
            for (int i = 0; i < num_spoofers; i++) {
              spoofers[i].updatePilotLocation(latitude, longitude);
            }
          }
          isNewPilotLocation = 0;
        }

        if (gps.time.isValid())
        {
          hour = gps.time.hour();
          minute = gps.time.minute();
          second = gps.time.second();
          if (minute > 59)
          {
            minute = minute - 60;
            hour = hour + 1;
          }
          for (int i = 0; i < num_spoofers; i++) {
            spoofers[i].updateTime(hour, minute, second);
          }
        }
        if (gps.date.isValid())
        {
          year = gps.date.year();
          month = gps.date.month();
          day = gps.date.day();
          for (int i = 0; i < num_spoofers; i++) {
            spoofers[i].updateDate(year, month, day);
          }
        }
        if (gps.satellites.isValid())
        {
          satelliteCount = gps.satellites.value();
          if (satelliteCount > 7) {
            gotGPS = 1;
          }
          for (int i = 0; i < num_spoofers; i++) {
            spoofers[i].updateSatellites(satelliteCount);
          }
        }
      }      
    }
  }

  if (gotGPS && gpsLock)
  {
    Serial.println("GPS Locked, Starting Spoofers");
    gpsLock = 0;
  }

  if (satelliteCount > 7) {
    for (int i = 0; i < num_spoofers; i++) {
      spoofers[i].update();
      delay(200 / num_spoofers);
    }
  }
  else if (!gpsLock) {
    Serial.println("Waiting on Satellites");
    gpsLock = 1;
    gotGPS = 0;
  }
}