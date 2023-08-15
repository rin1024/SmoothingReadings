/**
   Smoothing readings
*/
#include "SmoothingReadings.h"

/**
   コンストラクタ
*/
SmoothingReadings::SmoothingReadings() {
  numReadings = 0;
  readingIndex = 0;

  offsetIndex = 0;
  numOffsetReadings = 0;

  offsetVal = 0;
  averageVal = 0;
  minVal = 0;
  maxVal = 0;
  mediVal = 0;

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
    readings[i] = 0.0;
  }
  readingIndex = 0;

  if (_numOffsetReadings > SIZE_OF_READINGS) {
    _numOffsetReadings = SIZE_OF_READINGS;
  }
  numOffsetReadings = _numOffsetReadings;
  for (int i=0;i<numOffsetReadings;i++) {
    offsetReadings[i] = 0.0;
  }
  offsetIndex = 0;
}

/**
 * オフセットキャリブレーション
 *
 * @return true if finished
 */
bool SmoothingReadings::calcOffset(int _rawVal) {
  if (offsetIndex < numOffsetReadings) {
    //if (_rawVal < MIN_VAL || _rawVal > MAX_VAL) {
    //  Serial.print("rawVal seems overflow? ");
    //  Serial.println(_rawVal);
    //}

    offsetReadings[offsetIndex] = float(_rawVal) / READINGS_SCALE;
    //Serial.println(offsetReadings[offsetIndex]);

    offsetIndex++;

    return false;
  }

  Serial.print("stats: ");
  Serial.println(stats.average(offsetReadings, numOffsetReadings));
  offsetVal = numOffsetReadings == 0 ? 0 : 
    int(stats.average(offsetReadings, numOffsetReadings) * READINGS_SCALE);

  return true;
}

/**
   アップデート関数
*/
bool SmoothingReadings::update(int _rawVal) {
  boolean updated = false;
  
  //if (_rawVal < MIN_VAL || _rawVal > MAX_VAL) {
  //  Serial.print("rawVal seems overflow? ");
  //  Serial.println(_rawVal);
  //}
  
  // TODO: 3回サンプリングして良い値を選ぶなどするか？
  readings[readingIndex] = float(_rawVal - offsetVal) / READINGS_SCALE;

  if (++readingIndex >= numReadings) {
    averageVal = int(stats.average(readings, numReadings) * READINGS_SCALE);
    minVal = int(stats.minimum(readings, numReadings) * READINGS_SCALE);
    maxVal = int(stats.maximum(readings, numReadings) * READINGS_SCALE);
    mediVal = int(stats.median(readings, numReadings) * READINGS_SCALE);

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
   中央値を返す
*/
int SmoothingReadings::getMedi() {
  return mediVal;
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
  Serial.print("\t");

  Serial.print("mediVal: ");
  Serial.print(mediVal);

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
  Serial.print("\t");

  Serial.print(mediVal);

  Serial.println();
}
