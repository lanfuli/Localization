#include <IO.h>
#include <Arduino.h>
#include <Common.h>

#define MEGA 1
waypoint GPS_reading;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  bool GPS_available = GPS_reading.AcquireGPGGA(300);
}
