#include <SmoothingReadings.h>

#define __M_COMPILE_DATE__ __DATE__ " " __TIME__
#define __M_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/////////////// serial
#define BAUD_RATE 115200
#define NUM_BUF 10

char messageValue[NUM_BUF];
int messageValueIndex = 0;

/////////////// sensor
#define NUM_READINGS 10

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
}

void loop() {
  int receivedVal = getSerialData();
  
  if (receivedVal > -1) {
    boolean updated = sensorValues.update(receivedVal);

    // readings一覧を表示
    sensorValues.dumpReadings();
    
    //if (updated) {
      int average = sensorValues.getAverage();
      int accel = sensorValues.getAccel();
      Serial.print("average: ");
      Serial.print(average);
      Serial.print("\t");
      Serial.print("accel: ");
      Serial.print(accel);
      Serial.println();
    //}
  }
}

int getSerialData() {
  while (Serial.available() > 0) {
    int serialData = Serial.read();
    if (serialData < 48 || serialData > 66) {
      return -1;
    }
    // 0-9
    else if (serialData >= 48 && serialData <= 57) {
      if (messageValueIndex < NUM_BUF) {
        messageValue[messageValueIndex++] = serialData;
      }
    }
    // :
    else if(serialData == 58) {
      int receivedVal = atoi(messageValue);

      for(int i=0;i<NUM_BUF;i++) {
        messageValue[i] = 0;
      }
      messageValueIndex = 0;

      return receivedVal;
    }
  }

  return -1;
}
