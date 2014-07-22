#ifndef TONEOBSERVER_H
#define TONEOBSERVER_H

#include <map>
#include <string>

class ToneObserver
{
public:
  ToneObserver();

  virtual void notifyTone(const std::map<std::string, double>& toneAmplitudes) = 0;
};

#endif // TONEOBSERVER_H
