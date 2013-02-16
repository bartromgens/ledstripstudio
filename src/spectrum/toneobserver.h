#ifndef TONEOBSERVER_H
#define TONEOBSERVER_H

#include <map>
#include <string>

class ToneObserver
{
public:
  ToneObserver();

  virtual void notifyTone(std::map<std::string, double> toneAmplitudes) = 0;
};

#endif // TONEOBSERVER_H
