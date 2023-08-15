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
bool SmoothingReadings::calcOffset(int _rawVal) {
  if (offsetCount++ < numOffsetReadings) {
    totalOffsetVal += long(_rawVal);

    return false;
  }

  offsetVal = numOffsetReadings == 0 ? 
    0 : 
    int(totalOffsetVal / long(numOffsetReadings));

  return true;
}

/**
   アップデート関数
*/
bool SmoothingReadings::update(int _rawVal) {
  bool updated = false;

  int formattedVal = _rawVal - offsetVal;
  
  total = total - long(readings[readingIndex]);
  
  readings[readingIndex] = formattedVal;
  
  total = total + long(readings[readingIndex]);

  if (minVal > formattedVal) {
    minVal = formattedVal;
  }

  if (maxVal < formattedVal) {
    maxVal = formattedVal;
  }

  averageVal = int(total / long(numReadings));
  if (++readingIndex >= numReadings) {
    // calcAccel
    accelVal = 0;
    for (int i=0;i<numReadings - 1;i++) {
      accelVal += readings[i + 1] - readings[i];
    }

    if (debugType == DEBUG_TYPE_PRINT) {
      debugPrint(_rawVal);
    }
    else if (debugType == DEBUG_TYPE_PLOT) {
      debugPlot(_rawVal);
    }
    
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

  readings = new int[numReadings];
  for (int i=0;i<numReadings;i++) {
    readings[i] = 0;
  }
  readingIndex = 0;
  
  total = 0;
}

/**
   任意のindexのreadingsをかえす
*/
int SmoothingReadings::getReading(int _index) {
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
int SmoothingReadings::getOffset() {
  return offsetVal;
}

/**
   平均値を返す
*/
int SmoothingReadings::getAverage() {
  return averageVal;
}

/**
   加速度を返す
*/
int SmoothingReadings::getAccel() {
  return accelVal;
}

/**
   最小値を返す
*/
int SmoothingReadings::getMin() {
  return minVal;
}

/**
   最大値を返す
*/
int SmoothingReadings::getMax() {
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
void SmoothingReadings::debugPrint(int _rawVal) {
  Serial.print("rawVal: ");
  Serial.print(_rawVal);
  Serial.print("\t");

  Serial.print("average: ");
  Serial.print(averageVal);
  Serial.print("\t");

  Serial.print("accelVal: ");
  Serial.print(accelVal);

  Serial.print("offset: ");
  Serial.print(offsetVal);
  Serial.print("\t");

  Serial.print("minVal: ");
  Serial.print(minVal);
  Serial.print("\t");

  Serial.print("maxVal: ");
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
void SmoothingReadings::debugPlot(int _rawVal) {
  Serial.print(_rawVal);
  Serial.print("\t");

  Serial.print(averageVal);
  Serial.print("\t");

  Serial.print(accelVal);
  Serial.print("\t");

  Serial.print(offsetVal);
  Serial.print("\t");

  Serial.print(minVal);
  Serial.print("\t");

  Serial.print(maxVal);

  Serial.println();
}

/**
   現在のreadingsをhead(readingIndex)から表示する
   */
void SmoothingReadings::dumpReadings() {
  int headIndex = this->getReadingIndex();
  int numReadings = this->getNumReadings();
  Serial.print("[");
  Serial.print(headIndex);
  Serial.print(" / ");
  Serial.print(numReadings);
  Serial.print("] ");
  for (int i=0;i<numReadings;i++) {
    int val = this->getReading((headIndex + i) % numReadings);
    Serial.print(val);
    Serial.print(" ");
  }
  Serial.println();
}
