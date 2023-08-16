#include <SmoothingReadings.h>
#include <HX711_ADC.h>

#define __M_COMPILE_DATE__ __DATE__ " " __TIME__
#define __M_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/////////////// serial
#define BAUD_RATE 115200
#define NUM_BUF 10

char messageValue[NUM_BUF];
int messageValueIndex = 0;

/////////////// sensor
#define STABILIZING_TIME 2000 // unsigned long
#define CALIBRATION_FACTOR 696.0
#define NUM_READINGS 3

#define LOADCELL_DOUT_PIN A4
#define LOADCELL_SCK_PIN  A5

HX711_ADC scale(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

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

  // init HX711
  scale.begin();
  scale.start(STABILIZING_TIME, true);
  if (scale.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    scale.setCalFactor(CALIBRATION_FACTOR);
  }
}

void loop() {
  if (scale.update()) {
    long scaleLongVal = long(scale.getData() * 100.0);
    
    if (sensorValues.update(scaleLongVal)) {
      // something todo
    }

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
