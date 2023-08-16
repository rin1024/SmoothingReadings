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
  lastAverageVal = 0;
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
  if (++readingIndex >= numReadings) {
    calcAccel();

    if (debugType == DEBUG_TYPE_PRINT) {
      debugPrint(_rawVal);
    }
    else if (debugType == DEBUG_TYPE_PLOT) {
      debugPlot(_rawVal);
    }
    
    readingIndex = 0;
    updated = true;
    firstLoop = false;

    lastAverageVal = averageVal;
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
  for (int i=0;i<numReadings;i++) {
    readings[i] = 0;
  }
  readingIndex = 0;
  
  total = 0;
}

/**
   任意のindexのreadingsをかえす
*/
long SmoothingReadings::getReading(int _index) {
  return readings[_index];
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
   前回の平均値を返す
*/
long SmoothingReadings::getLastAverage() {
  return lastAverageVal;
}

/**
   加速度をその場で計算して返す
*/
long SmoothingReadings::calcAccel() {
  // readingsからの差分でちゃんと計算する場合
  /*accelVal = 0;

  for (int i=1;i<numReadings;i++) {
    accelVal += readings[i] - readings[i - 1];
  }

  accelVal += readings[numReadings - 1] - readings[0];*/

  // avgの比較のみで加速度を取る場合
  accelVal = lastAverageVal - averageVal;

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

  Serial.print(F("lastAverage: "));
  Serial.print(lastAverageVal);
  Serial.print(F("\t"));

  Serial.print(F("accelVal: "));
  Serial.print(accelVal);

  Serial.print(F("offset: "));
  Serial.print(offsetVal);
  Serial.print(F("\t"));

  Serial.print(F("minVal: "));
  Serial.print(minVal);
  Serial.print(F("\t"));

  Serial.print(F("maxVal: "));
  Serial.print(maxVal);

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

  Serial.print(lastAverageVal);
  Serial.print(F("\t"));

  Serial.print(accelVal);
  Serial.print(F("\t"));

  Serial.print(offsetVal);
  Serial.print(F("\t"));

  Serial.print(minVal);
  Serial.print(F("\t"));

  Serial.print(maxVal);

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

  Serial.print(F("lastAverage"));
  Serial.print(F("\t"));

  Serial.print(F("accel"));
  Serial.print(F("\t"));

  Serial.print(F("offset"));
  Serial.print(F("\t"));

  Serial.print(F("min"));
  Serial.print(F("\t"));

  Serial.print(F("max"));

  Serial.println();
}

/**
   現在のreadingsをhead(readingIndex)から表示する
   */
void SmoothingReadings::dumpReadings() {
  int headIndex = this->getReadingIndex();
  int numReadings = this->getNumReadings();
  Serial.print(F("["));
  Serial.print(headIndex);
  Serial.print(F(" / "));
  Serial.print(numReadings);
  Serial.print(F("] "));
  for (int i=0;i<numReadings;i++) {
    int val = this->getReading((headIndex + i) % numReadings);
    Serial.print(val);
    Serial.print(F(" "));
  }
  Serial.println();
}
