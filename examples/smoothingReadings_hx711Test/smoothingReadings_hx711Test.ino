#include <SmoothingReadings.h>
#include "HX711.h"

#define __M_COMPILE_DATE__ __DATE__ " " __TIME__
#define __M_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/////////////// serial
#define BAUD_RATE 115200
#define NUM_BUF 10

char messageValue[NUM_BUF];
int messageValueIndex = 0;

/////////////// sensor
#define NUM_READINGS 3

#define LOADCELL_DOUT_PIN A4
#define LOADCELL_SCK_PIN  A5

HX711 scale;
SmoothingReadings sensorValues;

////////////////////////////////////////
void setup() {
  Serial.begin(BAUD_RATE);
  delay(1000);

  Serial.println();
  Serial.print("[");
  Serial.print(__M_COMPILE_DATE__);
  Serial.print("]");
  Serial.println(__M_FILENAME__);

  sensorValues.setup(NUM_READINGS);
  //sensorValues.enableDebug(DEBUG_TYPE_PRINT);
  sensorValues.enableDebug(DEBUG_TYPE_PLOT);
  sensorValues.showPlotLabel();

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.tare();
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.read();
    boolean updated = sensorValues.update(int(reading));

    // readings一覧を表示
    //sensorValues.dumpReadings();
    
    //if (updated) {
      /*int average = sensorValues.getAverage();
      int accel = sensorValues.getAccel();
      //Serial.print("average: ");
      Serial.print(average);
      Serial.print("\t");
      //Serial.print("accel: ");
      Serial.print(accel);
      Serial.println();*/
    //}
  }
  else {
    //Serial.println("HX711 not found.");
  }

  //delay(20);
}
