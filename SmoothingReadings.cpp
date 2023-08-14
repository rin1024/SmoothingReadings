/**
   Smoothing readings
*/
#include "SmoothingReadings.h"

/**
   コンストラクタ
*/
SmoothingReadings::SmoothingReadings() {
  numReadings = 0;
  total = 0;
  readingIndex = 0;

  offsetCount = 0;
  totalOffsetVal = 0;
  numOffsetReadings = 0;

  offsetVal = 0;
  averageVal = 0;
  minVal = 0;
  maxVal = 0;

  debugType = DEBUG_TYPE_NONE;
}

/**
   デストラクタ
*/
SmoothingReadings::~SmoothingReadings() {
  //delete[] readings;
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
  // TODO: implement here
  if (_numReadings > SIZE_OF_READINGS) {
    _numReadings = SIZE_OF_READINGS;
  }
  numReadings = _numReadings;
  //readings = new int[numReadings];
  for (int i=0;i<numReadings;i++) {
    readings[i] = 0;
  }
  total = 0;
  readingIndex = 0;

  numOffsetReadings = _numOffsetReadings;
}

/**
 * オフセットキャリブレーション
 *
 * @return true if finished
 */
bool SmoothingReadings::calcOffset(int _rawVal) {
  if (numOffsetReadings == 0) {
    offsetVal = 0;

    return true;
  }
  else if (offsetCount < numOffsetReadings) {
    totalOffsetVal += long(_rawVal);

    return false;
  }

  offsetVal = int(totalOffsetVal / long(numOffsetReadings));

  return true;
}

/**
   アップデート関数
*/
bool SmoothingReadings::update(int _rawVal) {
  boolean updated = false;
  
  total = total - long(readings[readingIndex]);
  readings[readingIndex] = _rawVal - offsetVal;
  total = total + long(readings[readingIndex]);

  // TODO: 最大値、最小値の処理を追加する予定

  averageVal = int(total / long(numReadings));
  if (++readingIndex >= numReadings) {
    if (debugType == DEBUG_TYPE_PRINT) {
      debugPrint();
    }
    else if (debugType == DEBUG_TYPE_PLOT) {
      debugPlot();
    }
    
    readingIndex = 0;
    updated = true;
  }

  return updated;
}

/**
   平均値を返す
*/
int SmoothingReadings::getAverage() {
  return averageVal;
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
  Serial.print("average: ");
  Serial.print(averageVal);
  Serial.print("\t");

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
  Serial.print(averageVal);
  Serial.print("\t");

  Serial.print(offsetVal);
  Serial.print("\t");

  Serial.print(minVal);
  Serial.print("\t");

  Serial.print(maxVal);

  Serial.println();
}
