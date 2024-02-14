#include "outputlights.h"
#include "arduino.h"

OutputLights::OutputLights(LightModeConfig modeConfig, LightOutputConfig outputConfig)
{
  this->modeConfig = modeConfig;
  this->outputConfig = outputConfig;
//  
  pinMode(outputConfig.brakePin, OUTPUT);
  pinMode(outputConfig.reversePin, OUTPUT);
  pinMode(outputConfig.headLightPin, OUTPUT);
  pinMode(outputConfig.roofLightPin, OUTPUT);
}

void OutputLights::send(LightSetting setting)
{
	analogWrite(this->outputConfig.brakePin, setting.brakeIntensity);
	analogWrite(this->outputConfig.reversePin, setting.reverseIntensity);
	analogWrite(this->outputConfig.headLightPin, setting.headLightIntensity);
	analogWrite(this->outputConfig.roofLightPin, setting.roofLightIntensity);
	analogWrite(this->outputConfig.floodlightPin, setting.floodlightIntensity);
}