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
}

void loop() {
  //long sensorVal = analogRead(0);
  //long sensorVal = cnt++ % 75;
  //long sensorVal = random(0, 1024);
  long sensorVal = long(sin(millis()) * 1000.0);
  if (sensorValues.update(sensorVal)) {
    // readings一覧を表示
    //sensorValues.dumpReadings();

    // 平均値と加速度を表示
    long average = sensorValues.getAverage();
    long accel = sensorValues.getAccel();
    //Serial.print("average: ");
    Serial.print(average);
    Serial.print("\t");
    //Serial.print("accel: ");
    Serial.print(accel);
    Serial.println();
  }

  //delay(25);
}
