#ifndef POWERSPECTRUM_H
#define POWERSPECTRUM_H

#include <map>
#include <vector>


struct FrequencyBand
{
  double getBandWidth() const
  {
    return maxFrequency - minFrequency;
  }

  double value;
  double minFrequency;
  double maxFrequency;
};


class PowerSpectrum
{
public:
  PowerSpectrum(int nBins, double minFrequency, double maxFrequency);

  double getBinSize() const;
  void pushBack(double frequency, double value);

  const std::vector<std::pair<double, double>>& getSpectrum() const;
  std::vector<std::pair<double, double>> getSpectrum(double maxFrequency) const;

private:
  int m_nBins;
  double m_minFrequency;
  double m_maxFrequency;
  std::vector<std::pair<double, double>> m_spectrum;
};

#endif // POWERSPECTRUM_H
