#ifndef POWERSPECTRUM_H
#define POWERSPECTRUM_H

#include <map>
#include <vector>


class PowerSpectrum
{
public:
  PowerSpectrum(int nBins, double minFrequency, double maxFrequency);

  double getBinSize() const;
  void pushBack(double frequency, double value);

  const std::vector<std::pair<double, double>>& getSpectrum() const;
  std::vector<std::pair<double, double>> getSpectrum(double maxFrequency) const;

  static std::vector<std::pair<double, double> > resizeSpectrum(std::vector<std::pair<double, double>> spectrum,
                                                                std::size_t nBins,
                                                                double maxFrequency);

private:
  int m_nBins;
  double m_minFrequency;
  double m_maxFrequency;
  std::vector<std::pair<double, double>> m_spectrum;
};

#endif // POWERSPECTRUM_H
