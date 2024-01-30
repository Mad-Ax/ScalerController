#include "outputlights.h"
#include "arduino.h"

OutputLights::OutputLights(LightModeConfig modeConfig, LightOutputConfig outputConfig)
{
  this->modeConfig = modeConfig;
  this->outputConfig = outputConfig;
//  
//  pinMode(outputConfig.indicatorLeftPin, OUTPUT);
//  pinMode(outputConfig.indicatorRightPin, OUTPUT);
  pinMode(outputConfig.brakePin, OUTPUT);
  pinMode(outputConfig.reversePin, OUTPUT);
  //  pinMode(outputConfig.headLightPin, OUTPUT);
//  pinMode(outputConfig.fogLightFrontPin, OUTPUT);
//  pinMode(outputConfig.fogLightRearPin, OUTPUT);
//  pinMode(outputConfig.roofLightPin, OUTPUT);
}

void OutputLights::send(LightSetting setting)
{
//  digitalWrite(this->cfg.indicatorLeftPin, setting.indicatorLeftIntensity);
//  digitalWrite(this->cfg.indicatorRightPin, setting.indicatorRightIntensity);
  analogWrite(this->outputConfig.brakePin, setting.brakeIntensity);
  analogWrite(this->outputConfig.reversePin, setting.reverseIntensity);
//  analogWrite(this->cfg.headLightPin, setting.headLightIntensity);
//  digitalWrite(this->cfg.fogLightFrontPin, setting.frontFogIntensity);
//  digitalWrite(this->cfg.fogLightRearPin, setting.rearFogIntensity);
//  digitalWrite(this->cfg.roofLightPin, setting.roofLightIntensity);

}