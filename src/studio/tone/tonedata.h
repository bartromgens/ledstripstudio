#ifndef TONEDATA_H
#define TONEDATA_H

#include "basic/tone.h"

#include <deque>


struct ToneData
{
  ToneData() :
    maxToneHistory(),
    minToneHistory(),
    toneMaxAverage(0.0),
    toneMinAverage(0.0),
    maxTone(Tone::A),
    maxToneAmplitude(0.0),
    minToneAmplitude(0.0)
  {}

  std::deque<double> maxToneHistory;
  std::deque<double> minToneHistory;
  double toneMaxAverage;
  double toneMinAverage;

  Tone maxTone;
  double maxToneAmplitude;
  double minToneAmplitude;
};

#endif // TONEDATA_H
