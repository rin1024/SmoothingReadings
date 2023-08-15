#include <SmoothingReadings.h>

#define __M_COMPILE_DATE__ __DATE__ " " __TIME__
#define __M_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/////////////// serial
#define BAUD_RATE 115200

/////////////// readings
#define NUM_READINGS 64
#define NUM_OFFSET_READINGS 10

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

  sensorValues.setup(NUM_READINGS, NUM_OFFSET_READINGS);
  //sensorValues.enableDebug(DEBUG_TYPE_PLOT);
  while (sensorValues.calcOffset(analogRead(0))) {
    ;
  }
}

void loop() {
  //int sensorVal = analogRead(0);
  //int sensorVal = cnt++ % 75;
  //int sensorVal = random(0, 1024);
  int sensorVal = int(sin(millis()) * 1000.0);
  if (sensorValues.update(sensorVal)) {
    // readings一覧を表示
    //sensorValues.dumpReadings();

    // 平均値と加速度を表示
    int average = sensorValues.getAverage();
    int accel = sensorValues.getAccel();
    //Serial.print("average: ");
    Serial.print(average);
    Serial.print("\t");
    //Serial.print("accel: ");
    Serial.print(accel);
    Serial.println();
  }

  //delay(25);
}
