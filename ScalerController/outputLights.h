#pragma once

#include "ioutputlights.h"
//#include "enum.h"
//#include "structs.h"

// OutputLights class
// Sends required voltage values to required light pins
class OutputLights : public IOutputLights
{
public: 
	OutputLights(LightOutputConfig& outputConfig);
    void send(LightSetting& lightSetting);

private:
	LightOutputConfig& outputConfig;
};
