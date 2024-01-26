#pragma once

// Main configuration file - all config settings are changed here


// Loop Timer config

// This is the master timer control.  The main loop will pause for this value
// on every cycle.  This adds some roughness to the steering channel but helps 
// maintain consistent operation when altering operational settings.  Note that
// altering this setting will greatly impact any inertia values.

#define MASTER_DELAY 10000

// Input config

// Input pin for PPM from receiver
// Can be 18, 19, 20 or 21 (or 2 or 3 if not using lighting output on those pins)
// Note: when tested on Elegoo Mega2560, pins 18 and 19 caused glitching in all channels
// rendering the system unusable.  Pin 21 was found to have no such problems.  This is
// probably due to a conflict with some other internal operation or library (such as Servo)
#define PPM_IN 21                   // PPM input pin
#define PPM_INPUT_MAX_ERROR 10      // Max error leeway for PPM reader (see https://github.com/Nikkilae/PPM-reader#fine-tuning)
#define PPM_BLANK_TIME 2100         // Blank time for PPM reader (see above link)

// Radio channel config


// Max Failsafe Count.  Number of times the Rx must receive identical data on all channels
// before activating the failsafe.
#define MAX_FAILSAFE_COUNT 30				//TODO: M: test that this is a valid number

//// MODE channel.  Always operates.
//#define CHN_MODE_CH 5                       // Channel should not be shared

// STEERING channel.  Operates only in DRIVE mode.
#define CHN_STEERING_CH 1                   // Input channel number
#define CHN_STEERING_MIN 1000               // Minimum input  value
#define CHN_STEERING_MAX 2000               // Maximum input value
#define CHN_STEERING_DEADBAND_MIN 1486      // Deadband (center position) range start
#define CHN_STEERING_DEADBAND_MAX 1504      // Deadband (center position) range end

// THROTTLE/BRAKE channel.  Operates only in DRIVE mode.
#define CHN_THROTTLE_CH 3                   // Input channel number
#define CHN_THROTTLE_MIN 1000               // Minimum input  value
#define CHN_THROTTLE_MAX 2000               // Maximum input value
#define CHN_THROTTLE_DEADBAND_MIN 1488      // Deadband (center position) range start
#define CHN_THROTTLE_DEADBAND_MAX 1506      // Deadband (center position) range end

//// LIGHTS operation channel.  Operates only in DRIVE mode.
//// If set to 0, no lights will function.
//#define CHN_LIGHTS      4                   // Input channel number.
//
//// GEAR operation channel.  Operates only in DRIVE mode.
//#define CHN_GEAR        6                   // Input channel number
//
//// INDICATOR operation channel.  Operates only in DRIVE mode.
//// If set to 0, indicators will not function.
//#define CHN_INDICATOR   7                   // Input channel number
//
////TODO: S: these are not yet used...
//#define CHN_AUX1        1           // Output this channel to Aux1 servo when mode is Park
//#define CHN_AUX2        2           // Output this channel to Aux2 servo when mode is Park
//#define CHN_AUX3        3           // Output this channel to Aux3 servo when mode is Park
//#define CHN_AUX4        4           // Output this channel to Aux4 servo when mode is Park
//
//// Control configuration for radio, ESC and servo
//#define E_BRAKE_THRESHOLD 1100      // Drag brake comes on with input below this value
//
//#define SWITCH_HIGH 1650            // Value for "high" or "up" on a Tx switch or knob
//#define SWITCH_LOW 1350             // Value for "low" or "down" on a Tx switch or knob

// STEERING output settings
#define SVO_STEERING_PIN 8          // Output pin for steering servo
#define SVO_STEERING_MIN 1000       // Adjusts steering LH endpoint
#define SVO_STEERING_MAX 2000       // Adjusts steering RH endpoint
#define SVO_STEERING_CENTER 1500    // Adjusts steering neutral position

// ESC output settings
#define SVO_ESC_PIN 7               // Output pin for ESC
#define SVO_ESC_MIN 1000            // Adjusts ESC maximum reverse position
#define SVO_ESC_MAX 2000            // Adjusts ESC maximum throttle position
#define SVO_ESC_CENTER 1500         // Adjusts ESC neutral / drag brake position

//#define FWD_ACCEL_INERTIA 6         // Accelerator inertia (forward)
//#define FWD_DECEL_INERTIA 2         // Decelerator inertia (forward)
//#define FWD_BRAKE_INERTIA 8         // Brake interia (forward)
//
//#define REV_ACCEL_INERTIA 6         // Accelerator inertia (reverse)
//#define REV_DECEL_INERTIA 2         // Decelerator inertia (reverse)
//#define REV_BRAKE_INERTIA 8         // Brake interia (reverse)
//
//#define AUX_OUT_1 10                // Outputs aux1 channel value when in Park mode
//#define AUX_OUT_2 11                // Outputs aux2 channel value when in Park mode
//#define AUX_OUT_3 12                // Outputs aux3 channel value when in Park mode
//#define AUX_OUT_4 13                // Outputs aux4 channel value when in Park mode
//
//// Light configuration
//
//// LED output pins
//#define HEADLIGHT_OUT 2             // front headlights (PWM)
//#define BRAKELIGHT_OUT 3            // brake / tail lights (PWM)
//#define FOGLIGHT_F_OUT 22           // front fog lights (digital)
//#define FOGLIGHT_R_OUT 23           // rear fog lights (digital)
//#define INDICATOR_L_OUT 24          // left indicators (digital)
//#define INDICATOR_R_OUT 25          // right indicators (digital)
//#define ROOFLIGHT_OUT 26            // roof / aux lights (digital)
//#define REVERSE_OUT 27              // reverse lights (digital)
//#define FAILSAFE_OUT 28				// check valid LED output (digital)
//
//// Timing settings
//#define INDICATOR_TIME 400			// Indicator on/off time (approx)
//#define FAILSAFE_FLASH_TIME 100		// Indicator on/off time in failsafe
//
//// LED output settings
//// Intensity values range from 0 (off) to 255 (max, 5V)
//#define BRAKE_MAX_PWM 255           // Brakes on
//#define BRAKE_MED_PWM 100           // Sidelights/headlights on
//
//#define HEADLIGHT_MAX_PWM 255       // Main beam
//#define HEADLIGHT_MED_PWM 100       // Dipped beam
//#define HEADLIGHT_MIN_PWM 50        // Sidelights
