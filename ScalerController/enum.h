#pragma once

#include "Arduino.h"

enum class Mode
{
	Controler = LOW,
	PassThru = HIGH
};

enum class Gear
{
	Forward = 0,
	Reverse = 1,
};

enum class Cruise
{
	Off = 0,
	On = 1
};

enum class LightMode
{
	Off = 0,
	Dipped = 1,
	Rooflights = 2,
	Full = 3
};

//enum class MainBeam
//{
//	Off,
//	Flash,
//	On
//};

enum class BrakeLight
{
	Off,
	Sidelight,
	Braking
};

enum class ThreeWayPosition
{
	PosA = 0,
	PosB = 1,
	PosC = 2
};
//
//enum class TwoWayPosition
//{
//	PosA = 0,
//	PosB = 1,
//};