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
#define SR_DEBUG_TYPE_NONE  0
#define SR_DEBUG_TYPE_PRINT 1
#define SR_DEBUG_TYPE_PLOT  2

/**************************************************************************************
   Smoothing readings
 **************************************************************************************/
class SmoothingReadings {
  public:
    SmoothingReadings(int _numReadings = 5);
    ~SmoothingReadings();

    void setup(int _numReadings);
    void setup(int _numReadings, int _numOffsetReadings);
    bool calcOffset(long _rawVal);
    bool update(long _rawVal);

    int getNumReadings();
    void reallocReadings(int _numReadings);
    long getReading(int _index);
    long getReadingAverage(int _index);
    int getReadingIndex();
    
    long getOffset();
    long getAverage();
    long calcAccel();
    long getAccel();
    long getMin();
    long getMax();

    void enableDebug(int _debugType);
    void disableDebug();

    void debugPrint();
    void debugPrint(long _rawVal);
    void debugPlot();
    void debugPlot(long _rawVal);
    void showPlotLabel();

    void dumpReadings();
    void dumpReadingAverages();

  private:
    bool firstLoop;

    int numReadings;
    long *readings;
    long *readingAverages;
    int readingIndex;
    long total;
    long totalAverage;

    int offsetCount;
    long totalOffsetVal;
    int numOffsetReadings;

    long offsetVal;
    long averageVal;
    long accelVal;
    long minVal;
    long maxVal;

    int debugType;
};

#endif
