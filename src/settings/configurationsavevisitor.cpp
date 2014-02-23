#include "configurationsavevisitor.h"

#include "settings/configurationstate.h"
#include "settings/controlsettings.h"

#include <iostream>


ConfigurationSaveVisitor::ConfigurationSaveVisitor()
{
}


void
ConfigurationSaveVisitor::saveMe(const ControlSettings& controlSettings)
{
  std::cout << "ConfigurationSaveVisitor::saveMe(ControlSettings) - volume: " << controlSettings.volumeTotal << std::endl;

  QSettings settings("test.ini", QSettings::NativeFormat);

  settings.beginGroup("ControlSettings");

  // volume settings
  settings.setValue("volumeTotal", controlSettings.volumeTotal);
  settings.setValue("volumeRed", controlSettings.volumeRed);
  settings.setValue("volumeGreen", controlSettings.volumeGreen);
  settings.setValue("volumeBlue", controlSettings.volumeBlue);

  // frequency settings
  settings.setValue("freqRedMin", controlSettings.freqRedMin);
  settings.setValue("freqRedMax", controlSettings.freqRedMax);
  settings.setValue("freqGreenMin", controlSettings.freqGreenMin);
  settings.setValue("freqGreenMax", controlSettings.freqGreenMax);
  settings.setValue("freqBlueMin", controlSettings.freqBlueMin);
  settings.setValue("freqBlueMax", controlSettings.freqBlueMax);

  settings.endGroup();
}
