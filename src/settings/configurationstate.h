#ifndef CONFIGURATIONSTATE_H
#define CONFIGURATIONSTATE_H

class ConfigurationSaveVisitor;

class ConfigurationState
{
public:
  ConfigurationState();

  virtual void acceptSaver(ConfigurationSaveVisitor* vistor) = 0;
};

#endif // CONFIGURATIONSTATE_H
