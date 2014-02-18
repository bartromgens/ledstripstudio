#include "configurationsavevisitor.h"

#include "settings/configurationstate.h"
#include "settings/controlsettings.h"

#include <iostream>


ConfigurationSaveVisitor::ConfigurationSaveVisitor()
{
}


void
ConfigurationSaveVisitor::saveMe(ControlSettings* controlSettings)
{
  std::cout << "ConfigurationSaveVisitor::saveMe(ControlSettings) - volume: " << controlSettings->volumeTotal << std::endl;
}
