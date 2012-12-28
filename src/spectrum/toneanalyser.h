#ifndef TONEANALYSER_H
#define TONEANALYSER_H

#include <iostream>
#include <map>

class ToneAnalyser
{
public:
  ToneAnalyser();

  std::map<std::string, double> computeToneAmplitude(std::map<double, double> spectrum);
private:
  std::map<std::string, double> m_baseTones;
};

#endif // TONEANALYSER_H
