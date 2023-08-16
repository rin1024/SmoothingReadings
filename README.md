# SmoothingReadings

センサー等の値を平滑化する

## Usage

```
#include <SmoothingReadings.h>

#define NUM_READINGS 32
SmoothingReadings sensorValues;

void setup() {
  Serial.begin(115200);
  delay(1000);

  sensorValues.setup(NUM_READINGS);
  //sensorValues.enableDebug(DEBUG_TYPE_PRINT);
}

void loop() {
  int sensorVal = analogRead(0);
  if (sensorValues.update(long(sensorVal))) {
    long average = sensorValues.getAverage();
    Serial.print("average: ");
    Serial.print(average);
    Serial.println();
  }
}
```

## Requirement

- Arduino IDE

## Usage

## Author

[@rin1024](https://twitter.com/rin1024)

## License

[MIT](http://b4b4r07.mit-license.org)

