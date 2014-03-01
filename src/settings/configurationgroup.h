#ifndef CONFIGURATIONGROUP_H
#define CONFIGURATIONGROUP_H

class QSettings;

class ConfigurationGroup
{
public:
  ConfigurationGroup();

  virtual void saveConfiguration(QSettings& configuration) = 0;
  virtual void loadConfiguration(QSettings& configuration) = 0;
};

#endif // CONFIGURATIONGROUP_H
