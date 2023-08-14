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
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*******************************************
   const
 *******************************************/
#define SIZE_OF_READINGS 32 // TODO: fix me

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

    int getMin();
    int getMax();
    int getAverage();

    void enableDebug(int _debugType);
    void disableDebug();

  private:
    int numReadings;
    int readings[SIZE_OF_READINGS]; // TODO: fix me
    long total;
    int readingIndex;

    int offsetCount;
    long totalOffsetVal;
    int numOffsetReadings;

    int offsetVal;
    int averageVal;
    int minVal;
    int maxVal;

    int debugType;

    void debugPrint();
    void debugPlot();
};

#endif
