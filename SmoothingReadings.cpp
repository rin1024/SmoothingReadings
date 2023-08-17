/**
   Smoothing readings
*/
#include "SmoothingReadings.h"

/**
   コンストラクタ
*/
SmoothingReadings::SmoothingReadings(int _numReadings) {  
  reallocReadings(_numReadings);

  offsetCount = 0;
  totalOffsetVal = 0;
  numOffsetReadings = 0;

  offsetVal = 0;
  averageVal = 0;
  accelVal = 0;
  minVal = 0;
  maxVal = 0;

  debugType = DEBUG_TYPE_NONE;
}

/**
   デストラクタ
*/
SmoothingReadings::~SmoothingReadings() {
  delete[] readings;
  delete[] readingAverages;
}

/**
   初期化
*/
void SmoothingReadings::setup(int _numReadings) {
  setup(_numReadings, 0);
}

/**
   初期化
*/
void SmoothingReadings::setup(int _numReadings, int _numOffsetReadings) {
  reallocReadings(_numReadings);

  numOffsetReadings = _numOffsetReadings;
}

/**
 * オフセットキャリブレーション
 *
 * @return true if finished
 */
bool SmoothingReadings::calcOffset(long _rawVal) {
  if (offsetCount++ < numOffsetReadings) {
    totalOffsetVal += long(_rawVal);

    return false;
  }

  offsetVal = numOffsetReadings == 0 ? 
    0 : 
    totalOffsetVal / long(numOffsetReadings);

  return true;
}

/**
   アップデート関数
*/
bool SmoothingReadings::update(long _rawVal) {
  bool updated = false;

  long formattedVal = _rawVal - offsetVal;
  
  total = total - readings[readingIndex];
  readings[readingIndex] = formattedVal;
  total = total + readings[readingIndex];

  if (minVal > formattedVal) {
    minVal = formattedVal;
  }

  if (maxVal < formattedVal) {
    maxVal = formattedVal;
  }

  averageVal = total / long(numReadings);

  totalAverage = totalAverage - readingAverages[readingIndex];
  readingAverages[readingIndex] = averageVal;
  totalAverage = totalAverage + readingAverages[readingIndex];

  calcAccel();

  if (debugType == DEBUG_TYPE_PRINT) {
    debugPrint(_rawVal);
  }
  else if (debugType == DEBUG_TYPE_PLOT) {
    debugPlot(_rawVal);
  }

  if (++readingIndex >= numReadings) {
    readingIndex = 0;
    updated = true;
    firstLoop = false;
  }

  return updated;
}

/**
   総readingsを返す
*/
int SmoothingReadings::getNumReadings() {
  return numReadings;
}

/**
   再度配列を確保
*/
void SmoothingReadings::reallocReadings(int _numReadings) {
  firstLoop = true;

  numReadings = _numReadings;

  readings = new long[numReadings];
  readingAverages = new long[numReadings];
  for (int i=0;i<numReadings;i++) {
    readings[i] = 0;
    readingAverages[i] = 0;
  }
  readingIndex = 0;
  
  total = 0;
  totalAverage = 0;
}

/**
   任意のindexのreadingsをかえす
*/
long SmoothingReadings::getReading(int _index) {
  return readings[_index];
}

/**
   任意のindexのreadingAveragesをかえす
*/
long SmoothingReadings::getReadingAverage(int _index) {
  return readingAverages[_index];
}

/**
   現在位置を返す
*/
int SmoothingReadings::getReadingIndex() {
  return readingIndex;
}

/**
   オフセット値を返す
*/
long SmoothingReadings::getOffset() {
  return offsetVal;
}

/**
   平均値を返す
*/
long SmoothingReadings::getAverage() {
  return averageVal;
}

/**
   加速度をその場で計算して返す
*/
long SmoothingReadings::calcAccel() {
  accelVal = 0;

  for (int i=1;i<numReadings;i++) {
    int currentIndex = (readingIndex + i    ) % numReadings;
    int prevIndex    = (readingIndex + i - 1) % numReadings;

    // readingIndexが0のときに発生するはず
    if (currentIndex == 0 && prevIndex == -1) {
      if (firstLoop) {
        continue;
      }
      currentIndex = 0;
      prevIndex = numReadings - 1;
    }

    // NOTE: readingAveragesで計算するばあい
    //accelVal += readingAverages[currentIndex] - readingAverages[prevIndex];

    // NOTE: readingsでの加速度にする場合。こちらのほうが瞬間的なスパイクとりやすいかも。
    accelVal += readings[currentIndex] - readingAverages[prevIndex];
  }

  return accelVal;
}

/**
   加速度を返す
*/
long SmoothingReadings::getAccel() {
  return accelVal;
}

/**
   最小値を返す
*/
long SmoothingReadings::getMin() {
  return minVal;
}

/**
   最大値を返す
*/
long SmoothingReadings::getMax() {
  return maxVal;
}

/**
   デバッグ表示を有効化
*/
void SmoothingReadings::enableDebug(int _debugType) {
  debugType = _debugType;
}

/**
   デバッグ表示を無効化
*/
void SmoothingReadings::disableDebug() {
  debugType = DEBUG_TYPE_NONE;
}

/**
   デバッグ表示をする
*/
void SmoothingReadings::debugPrint() {
  debugPrint(0);
}

/**
   デバッグ表示をする
*/
void SmoothingReadings::debugPrint(long _rawVal) {
  Serial.print(F("rawVal: "));
  Serial.print(_rawVal);

  Serial.print(F("\t"));
  Serial.print(F("average: "));
  Serial.print(averageVal);

  Serial.print(F("\t"));
  Serial.print(F("accelVal: "));
  Serial.print(accelVal);

  Serial.print(F("\t"));
  Serial.print(F("offset: "));
  Serial.print(offsetVal);

  Serial.print(F("\t"));
  Serial.print(F("minVal: "));
  Serial.print(minVal);

  Serial.print(F("\t"));
  Serial.print(F("maxVal: "));
  Serial.print(maxVal);

  //Serial.print(F("\t"));
  //Serial.print(F("totalAverage: "));
  //Serial.print(totalAverage);

  Serial.println();
}

/**
   プロット用のデバッグ表示をする
*/
void SmoothingReadings::debugPlot() {
  debugPlot(0);
}

/**
   プロット用のデバッグ表示をする
*/
void SmoothingReadings::debugPlot(long _rawVal) {
  Serial.print(_rawVal);
  
  Serial.print(F("\t"));
  Serial.print(averageVal);
  
  Serial.print(F("\t"));
  Serial.print(accelVal);

  Serial.print(F("\t"));
  Serial.print(offsetVal);

  Serial.print(F("\t"));
  Serial.print(minVal);

  Serial.print(F("\t"));
  Serial.print(maxVal);

  //Serial.print(F("\t"));
  //Serial.print(totalAverage);

  Serial.println();
}

/**
   プロット用のラベル表示をする
*/
void SmoothingReadings::showPlotLabel() {
  Serial.print(F("raw"));

  Serial.print(F("\t"));
  Serial.print(F("average"));

  Serial.print(F("\t"));
  Serial.print(F("accel"));

  Serial.print(F("\t"));
  Serial.print(F("offset"));

  Serial.print(F("\t"));
  Serial.print(F("min"));

  Serial.print(F("\t"));
  Serial.print(F("max"));

  //Serial.print(F("\t"));
  //Serial.print(F("totalAverage"));

  Serial.println();
}

/**
   現在のreadingsをhead(readingIndex)から表示する
   */
void SmoothingReadings::dumpReadings() {
  //Serial.print(F("["));
  //Serial.print(readingIndex);
  //Serial.print(F(" / "));
  //Serial.print(numReadings);
  //Serial.print(F("] "));

  for (int i=0;i<numReadings;i++) {
    char buff[10];
    sprintf(buff, "%-4ld", readings[(readingIndex + i) % numReadings]);
    Serial.print(buff);

    Serial.print(F(" "));
  }

  Serial.println();
}

/**
   現在のreadingAveragesをhead(readingIndex)から逆順？に表示する
   */
void SmoothingReadings::dumpReadingAverages() {
  //Serial.print(F("["));
  //Serial.print(readingIndex);
  //Serial.print(F(" / "));
  //Serial.print(numReadings);
  //Serial.print(F("] "));

  for (int i=0;i<numReadings;i++) {
    int index = readingIndex - i;
    if (index < 0) {
      index += numReadings;
    }

    char buff[10];
    sprintf(buff, "%-4ld", readingAverages[index]);
    Serial.print(buff);
    
    Serial.print(F(" "));
  }

  Serial.println();
}
