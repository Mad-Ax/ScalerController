//#include "outputlights.h"
//#include "arduino.h"
//
//OutputLights::OutputLights(LightModeConfig modeConfig, LightOutputConfig outputConfig)
//{
//  this->modeConfig = modeConfig;
//  this->cfg = outputConfig;
//  
//  pinMode(outputConfig.indicatorLeftPin, OUTPUT);
//  pinMode(outputConfig.indicatorRightPin, OUTPUT);
//  pinMode(outputConfig.brakeLightPin, OUTPUT);
//  pinMode(outputConfig.headLightPin, OUTPUT);
//  pinMode(outputConfig.fogLightFrontPin, OUTPUT);
//  pinMode(outputConfig.fogLightRearPin, OUTPUT);
//  pinMode(outputConfig.roofLightPin, OUTPUT);
//  pinMode(outputConfig.reverseLightPin, OUTPUT);
//}
//
//void OutputLights::send(LightSetting setting)
//{
//  digitalWrite(this->cfg.indicatorLeftPin, setting.indicatorLeftIntensity);
//  digitalWrite(this->cfg.indicatorRightPin, setting.indicatorRightIntensity);
//  analogWrite(this->cfg.brakeLightPin, setting.brakeLightIntensity);
//  analogWrite(this->cfg.headLightPin, setting.headLightIntensity);
//  digitalWrite(this->cfg.fogLightFrontPin, setting.frontFogIntensity);
//  digitalWrite(this->cfg.fogLightRearPin, setting.rearFogIntensity);
//  digitalWrite(this->cfg.roofLightPin, setting.roofLightIntensity);
//  digitalWrite(this->cfg.reverseLightPin, setting.reverseLightIntensity);
//}