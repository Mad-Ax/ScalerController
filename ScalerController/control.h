#pragma once

//#include "enum.h"
//#include "input.h"
//#include "inertia.h"
#include "icontroltranslator.h"
//#include "ilightingtranslator.h"
//#include "Arduino.h"
#include "HardwareSerial.h"

// Control class
// Translates raw inputted values from the transmitter to output values that will be sent to the outputs
class Control
{
public:
	ControlSetting setting;

	// Constructor
//	Control(IControlTranslator* controlTranslator, ILightingTranslator* lightingTranslator, ControlConfig controlConfig);//TODO: remove config when not needed
	Control(IControlTranslator* controlTranslator, ControlConfig controlConfig);

	// Translate method
	// Translates the requested Input object into a set of values that can be sent to the Output
	void translate(InputSetting input, HardwareSerial& ser);

private:
	IControlTranslator* controlTranslator;
//	ILightingTranslator* lightingTranslator;
//	InputSetting* lastInput;
//	bool failsafe = false;
//	int failsafeCount;
//	Mode mode;
//	Gear gear;
//	LightMode lightMode = LightMode::Off;
//	MainBeam mainBeam = MainBeam::Off;
	ControlConfig config;//TODO: remove config when not needed
//	Inertia* forwardAccel;//TODO: remove these when not needed
//	Inertia* forwardDecel;
//	Inertia* forwardBrake;
//	Inertia* reverseAccel;
//	Inertia* reverseDecel;
//	Inertia* reverseBrake;
//	bool indicatorOn = false;
//	Indicator indicator = Indicator::Off;
//	unsigned long indicatorTime = 0;
//
//	//TODO: M: sort casing on all these badboys
//	//TODO: M: check where each method uses local var
//
//	// Translates the requested mode
//	void translateMode(int input);
//
//	// Translates the requested gear with the current mode to determine the selected gear
//	void translateGear(int input);

	// Translates the input into a motor speed
	void translateMotorSpeed(int input);

	// Translates the steering position input to desired output depending on mode
	void translateSteering(int input);

//	// Translates the indicator input to the desired output depending on mode
//	void translateIndicator(int input);
//
//	// Translate the light mode based on the switch setting
//	void translateLightMode(int input);
//
//	// Translates the main beam lamp setting based on lightmode and input
//	void translateMainBeam(int input);
//
//	// Translates the brake light setting based on lightmode and braking input
//	void translateBrakeLight(int input);
//
//	// Translates the reverse light setting based on mode and gear
//	void translateReverseLight();
//
//	// Toggles the indicator on/off flash based on timing settings
//	int indicatorToggle(int delay);
};
