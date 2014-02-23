#ifndef CONFIGURATIONSAVEVISITOR_H
#define CONFIGURATIONSAVEVISITOR_H

class ControlSettings;

class ConfigurationSaveVisitor
{
public:
  ConfigurationSaveVisitor();

  void saveMe(const ControlSettings& controlSettings);
};

#endif // CONFIGURATIONSAVEVISITOR_H
