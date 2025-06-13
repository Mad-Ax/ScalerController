#pragma once

// Main configuration file - all config settings are changed here


// Loop Timer config

// This is the master timer control.  The main loop will pause for this value
// on every cycle.  This adds some roughness to the steering channel but helps 
// maintain consistent operation when altering operational settings.  Note that
// altering this setting will greatly impact any inertia values.

#define MASTER_DELAY 10000

// Input config

// Servo config

// This is static config data and generally should not be changed

#define CHN_MIN 1000						// Minimum input value for any radio channel
#define CHN_MAX 2000						// Maximum input value for any radio channel
#define CHN_DEADBAND_MIN 1486				// Deadband (center position) range start
#define CHN_DEADBAND_MAX 1504				// Deadband (center position) range end

#define SWITCH_HIGH 1650					// Value for "high" or "up" on a Tx switch or knob
#define SWITCH_LOW 1350						// Value for "low" or "down" on a Tx switch or knob

#define SVO_MIN 1000						// Minimum output value for any ESC / servo channel
#define SVO_MAX	2000						// Maximum output value for any ESC / servo channel
#define SVO_CENTER 1500						// Center value for any ESC / servo channel

// Mode switch (physical switch - connect to GND to enable controller; leave open for pass-thru mode)
#define MODE_PIN 13							// Digital input pin for pass-thru switch
											// When closed (connected to 0V), ScalerController mode is active
											// When open (internal pullup to 5V), pass-thru mode is active and only basic steering and throttle will work

// Input pin for PPM from receiver
// Can be 18, 19, 20 or 21 (or 2 or 3 if not using lighting output on those pins)
// Note: when tested on Elegoo Mega2560, pins 18 and 19 caused glitching in all channels
// rendering the system unusable.  Pin 21 was found to have no such problems.  This is
// probably due to a conflict with some other internal operation or library (such as Servo)
#define PPM_IN 21							// PPM input pin
#define PPM_CHANNELS 8						// Total number of channels transmitted
#define PPM_INPUT_MAX_ERROR 10				// Max error leeway for PPM reader (see https://github.com/Nikkilae/PPM-reader#fine-tuning)
#define PPM_BLANK_TIME 2250					// Blank time for PPM reader (see above link)
#define PPM_MIN_CHANNEL_VALUE 850			// Minimum PPM value for the channel
#define PPM_MAX_CHANNEL_VALUE 2150			// Maximum PPM value for the channel

// Radio channel config

// Max Failsafe Count.  Number of times the Rx must receive identical data on all channels
// before activating the failsafe.
#define MAX_FAILSAFE_COUNT 300				//TODO: M: test that this is a valid number

// STEERING channel
#define CHN_STEERING_CH 1                   // Input channel number
#define CHN_STEERING_MIN 1000               // Minimum input  value
#define CHN_STEERING_MAX 2000               // Maximum input value
#define CHN_STEERING_DEADBAND_MIN 1500      // Deadband (center position) range start
#define CHN_STEERING_DEADBAND_MAX 1500      // Deadband (center position) range end

// THROTTLE/BRAKE channel
#define CHN_THROTTLE_CH 3                   // Input channel number
#define CHN_THROTTLE_MIN 1000               // Minimum input  value
#define CHN_THROTTLE_MAX 2000               // Maximum input value
#define CHN_THROTTLE_DEADBAND_MIN 1480      // Deadband (center position) range start
#define CHN_THROTTLE_DEADBAND_MAX 1520      // Deadband (center position) range end

#define CHN_THROTTLE_EBRAKE_THRESHOLD 1100	// Brake value below which the ESC all-stop signal will be given

// LIGHTS operation channel
// If set to 0, no lights will function.
#define CHN_LIGHTS      4                   // Input channel number.
#define CHN_LIGHTS_ON_SELECT_MIN 800		// Band range (min) for selecting lights on
#define CHN_LIGHTS_ON_SELECT_MAX 1250		// Band range (max) for selecting lights on
#define CHN_LIGHTS_OFF_SELECT_MIN 1750		// Band range (min) for selecting lights off
#define CHN_LIGHTS_OFF_SELECT_MAX 2200		// Band range (max) for selecting lights off

// GEAR operation channel.  Operates only in Controller mode.
#define CHN_GEAR_CH			2               // Input channel number
#define CHN_GEAR_SELECT_MIN	800             // Band range (min) for selecting forward / reverse gear
#define CHN_GEAR_SELECT_MAX 1250            // Band range (max) for selecting forward / reverse gear

// CRUISE operation channel.  Operates only in Controller mode.
#define CHN_CRUISE_CH		  2				// Input channel number
#define CHN_CRUISE_SELECT_MIN 1750			// Band range (min) for selecting cruise on / off
#define CHN_CRUISE_SELECT_MAX 2200          // Band range (max) for selecting cruise on / off

// FLOODLIGHT operation channel.  Operates only in Controller mode.
#define CHN_FLOOD_CH			5			// Input channel number

// WINCH channels.  Operates only in Controller mode.
#define CHN_WINCH_SELECT		6			// Input channel number.  Determines which winch should be operated
#define CHN_WINCH_OPERATE		2			// Input channel number.  Determines input value for winch

// STEERING output settings
#define SVO_STEERING_PIN 9					// Output pin for steering servo
#define SVO_STEERING_MIN 1000				// Adjusts steering LH endpoint
#define SVO_STEERING_MAX 2000				// Adjusts steering RH endpoint
#define SVO_STEERING_CENTER 1500			// Adjusts steering neutral position

// ESC output settings
#define SVO_ESC_PIN 8						// Output pin for ESC
#define SVO_ESC_MIN 1000					// Adjusts ESC maximum reverse position
#define SVO_ESC_MAX 2000					// Adjusts ESC maximum throttle position
#define SVO_ESC_CENTER 1500					// Adjusts ESC neutral / drag brake position

// Inertia settings - use higher values for faster operation
#define FWD_ACCEL_INERTIA 6					// Accelerator inertia (forward)
#define FWD_DECEL_INERTIA 2					// Decelerator inertia (forward)
#define FWD_BRAKE_INERTIA 8					// Brake interia (forward)

#define REV_ACCEL_INERTIA 6					// Accelerator inertia (reverse)
#define REV_DECEL_INERTIA 2					// Decelerator inertia (reverse)
#define REV_BRAKE_INERTIA 8					// Brake interia (reverse)

#define CRUISE_INERTIA    4					// Cruise inertia

#define STEERING_INERTIA  40				// Steering inertia

// Aux Servo Channel

#define AUX_1_CHN	8						// Radio channel for aux 1 servo
#define AUX_1_PIN	10						// Output pin for aux 1 servo

// Light configuration

// LED output pins
#define HEADLIGHT_OUT_PIN 7					// front headlights (PWM)
#define BRAKELIGHT_OUT_PIN 5				// brake / tail lights (PWM)
#define REVERSE_OUT_PIN 53					// reverse lights (digital)
#define ROOFLIGHT_OUT_PIN 6					// roof lights (PWM)
#define FLOODLIGHT_OUT_PIN 52				// floodlight (digital)
#define DASHLIGHT_R_PIN 2					// red channel for dash light (PWM)
#define DASHLIGHT_G_PIN 3					// green channel for dash light (PWM)
#define DASHLIGHT_B_PIN 4					// blue channel for dash light (PWM)

// LED output settings
// Intensity values range from 0 (off) to 255 (max, 5V)
#define BRAKE_MAX_PWM 255					// Brakes on
#define BRAKE_LOW_PWM 50					// Sidelights/headlights on
#define REVERSE_PWM 255						// Reverse light on
#define HEADLIGHT_MAX_PWM 255				// Main beams
#define HEADLIGHT_LOW_PWM 100				// Dipped beams
#define ROOFLIGHT_MAX_PWM 255				// Full rooflight
#define ROOFLIGHT_LOW_PWM 100				// Low rooflight
#define FLOODLIGHT_PWM 255					// Floodlight on

// Dash lights
#define DASHLIGHT_FAILSAFE_PWM_RED 255		// Failsafe PWM value for red channel
#define DASHLIGHT_FAILSAFE_PWM_GREEN 0		// Failsafe PWM value for green channel
#define DASHLIGHT_FAILSAFE_PWM_BLUE 0		// Failsafe PWM value for blue channel

#define DASHLIGHT_CRUISE_PWM_RED 0			// Cruise PWM value for red channel
#define DASHLIGHT_CRUISE_PWM_GREEN 0		// Cruise PWM value for green channel
#define DASHLIGHT_CRUISE_PWM_BLUE 255		// Cruise PWM value for blue channel

#define DASHLIGHT_INERTIA_PWM_RED 0			// Inertia PWM value for red channel
#define DASHLIGHT_INERTIA_PWM_GREEN 255		// Inertia PWM value for green channel
#define DASHLIGHT_INERTIA_PWM_BLUE 0		// Inertia PWM value for blue channel

#define DASHLIGHT_DIRECT_PWM_RED 255		// Direct PWM value for red channel
#define DASHLIGHT_DIRECT_PWM_GREEN 0		// Direct PWM value for green channel
#define DASHLIGHT_DIRECT_PWM_BLUE 200		// Direct PWM value for blue channel

// Winch configuration
#define SVO_WINCH_1_PIN		11				// Servo output for winch 1
#define SVO_WINCH_2_PIN		12				// Servo output for winch 2
#define LOGIC_WINCH_1_A_PIN	45				// H-bridge pin A for winch 1
#define LOGIC_WINCH_1_B_PIN	47				// H-bridge pin B for winch 1
#define LOGIC_WINCH_2_A_PIN	49				// H-bridge pin A for winch 2
#define LOGIC_WINCH_2_B_PIN	51				// H-bridge pin B for winch 2