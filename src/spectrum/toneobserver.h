#ifndef TONEOBSERVER_H
#define TONEOBSERVER_H

#include "basic/tone.h"

#include <map>
#include <string>

class ToneObserver
{
public:
  ToneObserver();

  virtual void notifyTone(const std::map<Tone, double>& toneAmplitudes) = 0;
};

#endif // TONEOBSERVER_H
