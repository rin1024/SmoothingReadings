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
#define DEBUG_TYPE_NONE  0
#define DEBUG_TYPE_PRINT 1
#define DEBUG_TYPE_PLOT  2

/**************************************************************************************
   Smoothing readings
 **************************************************************************************/
class SmoothingReadings {
  public:
    SmoothingReadings(int _numReadings = 5);
    ~SmoothingReadings();

    void setup(int _numReadings);
    void setup(int _numReadings, int _numOffsetReadings);
    bool calcOffset(int _rawVal);
    bool update(int _rawVal);

    int getNumReadings();
    void reallocReadings(int _numReadings);
    int getReading(int _index);
    int getReadingIndex();
    
    int getOffset();
    int getAverage();
    int calcAccel();
    int getAccel();
    int getMin();
    int getMax();

    void enableDebug(int _debugType);
    void disableDebug();

    void debugPrint();
    void debugPrint(int _rawVal);
    void debugPlot();
    void debugPlot(int _rawVal);
    void showPlotLabel();

    void dumpReadings();

  private:
    bool firstLoop;

    int numReadings;
    int *readings;
    int readingIndex;
    long total;

    int offsetCount;
    long totalOffsetVal;
    int numOffsetReadings;

    int offsetVal;
    int averageVal;
    int accelVal;
    int minVal;
    int maxVal;

    int debugType;
};

#endif
