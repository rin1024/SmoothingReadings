/**
   Smoothing readings
*/
#ifndef SMOOTHING_READINGS_H
#define SMOOTHING_READINGS_H

/*******************************************
   include
 *******************************************/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <QuickStats.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*******************************************
   const
 *******************************************/
#define SIZE_OF_READINGS 32   // TODO: fix me
#define READINGS_SCALE 10000.0 // float型に適応するためにスケールを変換している、floatは6から7桁の精度なので100000.0くらいがいいところ？

#define MIN_VAL 0
#define MAX_VAL 32767

#define DEBUG_TYPE_NONE  0
#define DEBUG_TYPE_PRINT 1
#define DEBUG_TYPE_PLOT  2

/**************************************************************************************
   Smoothing readings
 **************************************************************************************/
class SmoothingReadings {
  public:
    SmoothingReadings();
    ~SmoothingReadings();

    void setup(int _numReadings);
    void setup(int _numReadings, int _numOffsetReadings);
    bool calcOffset(int _rawVal);
    bool update(int _rawVal);

    int getOffset();
    int getAverage();
    int getMin();
    int getMax();
    int getMedi();

    void enableDebug(int _debugType);
    void disableDebug();

  private:
    QuickStats stats;

    int numReadings;
    float readings[SIZE_OF_READINGS]; // TODO: fix me
    int readingIndex;

    int numOffsetReadings;
    float offsetReadings[SIZE_OF_READINGS];
    int offsetIndex;

    int offsetVal;
    int averageVal;
    int minVal;
    int maxVal;
    int mediVal; // 中央値

    int debugType;

    void debugPrint();
    void debugPlot();
};

#endif
