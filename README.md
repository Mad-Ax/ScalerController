# ScalerController
An Arduino-based MFU for RC scale crawlers and hillwalking rigs

## Outline

This controller acts as an interface between the receiver and the speed controller, servos, lights and winches to allow the operator to drive the rig from a multi-channel transmitter and access various functions without having to touch the rig.  Planned functionality will include:

- full rig lights (multiple lighting options)
- working brake lights
- working reverse lights
- independent front and rear winch operation
- cruise control
- reverse lockout / reverse gear

## Requirements

To install this controller you will need:

- An Arduino Mega (other boards may be supported later)
- A radio system that supports PPM (TBC - other protocols maybe supported later)
- A speed controller with instant reverse and 100% drag brake (tested with Hobbywing WP-1080)
- Steering servo
- LEDs and resistors for lighting (optional)
- Front and rear winches (optional)
- An understanding of soldering techniques and how to connect microcontrollers to RC equipment and how to correctly wire LEDs
- Futher requirements to be confirmed as the project progresses

## Roadmap

### Version 0.0.1

This is the base iteration from which all further work will be built upon.  There is an existing solution (SCRAPSpeed TruckController) which was built for big rigs, which has much of the code required to get this project off the ground.  ScalerController will be built on this codebase, changing it where necessary to get the required functionality.

- ✔Import all code from the TruckController solution
- ✔Delete any code that relates to unrequired functionality (such as sound)
- ✔Rename all objects as appropriate to meet the ScalerController naming convention
- ✔Comment out all code apart from the basic loop
- ✔Open a single input for PPM
- ✔Implement basic pass-thru for throttle and steering
  - ✔Throttle channel to pass-thru to ESC output
  - ✔Steering channel to pass-thru to steering output
  - ✔No other outputs defined
  
### Version 0.0.2

Basic controller functionality with selectable reverse

- Create a new flag to store Mode (Off or On)
- When in Off mode, the controller continues to work in pass-thru mode as per 0.0.1
- When in On mode, the controller uses selectable reverse by pulling full back on right stick
  - When in Forward mode, the truck can only go forward.  Back-stick does nothing
  - When in Reverse mode, forward-stick causes the truck to go backward.  Back-stick does nothing
  - Steering continues to work normally

### Version 0.0.3

Inertia - the truck will take time to accelerate and brake depending on how far the stick is pushed

- Create a new variable to store the inertia value in (use existing value from TruckController)
- When in On mode
  - Pushing forward on the stick causes the truck to accelerate slowly to the value indicated by the stick
  - Pulling backward on the stick causes the truck to slow down at a rate indicated by the stick
  - Pulling full back on the stick causes the truck to instantly stop
  - When the stick is released, the truck will slow eventually to a stop
  - This works in forward and reverse modes
- When in Off mode, the controller is in full pass-thru mode

### Version 0.0.4
 
Cruise control - the truck's speed will remain constant when cruise is on and can be adjusted 

- Cruise is toggled on/off by pushing full-forward on the right stick and releasing
- When cruise is on, the truck will maintain the current speed when the throttle is released
- The farther forward the stick is pushed, the faster the speed will increase
- When the stick is pulled back, the truck will slow down until the stick is released, after which the current speed is retained
- The farther back the stick is pulled, the faster the truck slows down
- If the stick is pulled full-back, the truck comes instantly to a full stop
- When cruise is off, the truck will operate in Inertia mode as per 0.0.3

### Version 0.0.5

Operational lights - brake and reverse lights illuminate according to settings

- Brake and reverse lights will operate through PWM pins
- There will be setting values for brake and reverse light intensity
- When the truck is in reverse mode, the reverse lights will come on
- When not in reverse mode, the reverse lights will be off
- When the left stick is below centre, the brake lights will come on
- When the left stick is centre or above centre, the brake lights will be off

### Version 0.0.6

Driving lights - can be toggled through settings using the left stick to the full left and right positions

- Full left will advance the light setting
- Full right will retard the light setting
- All lights will operate through PWM pins
- There will be setting values for intensity of all lights
- In all light modes, brake and reverse continue to operate as in version 0.0.5
- Dipped and main beam use different intensity settings on the same pins
- Tail lights use the same pin as brake lights, but with a lower PWM value
- Light modes as follows
  - All off
  - Dipped beams - headlamps are low intensity, tail lights are on
  - Roof lamps low - roof lamps are low intensity
  - Main beams - roof lamps and headlamps are high intensity
  
### Version 0.0.7
 
Rear floodlight - a bright lamp that shines out the back of the truck, can be toggled on and off via a 2-way switch on the controller
 
- Floodlight will use a PWM pin
- There will be a setting value for floodlight intensity
- When the switch is down, the floodlight will be on
- When the switch is up, the floodlight will be off

### Version 0.0.8

Switch Off (pass-thru) - when a 2-way switch inside the truck is on the open position, the controller will revert to pass-thru mode

- When the switch is closed, all functionality from previous versions works as expected
- When the switch is open, the inputted throttle and steering channel data is output unchanged to the ESC and steering servo
- All other servo outputs are set to the centre position
- All lights are off

### Version 0.0.9

Winch controller - the use of a 3-way switch on the transmitter can enable either the front or rear winch to be operated by the right vertical axis

- When the switch is in the up position, the right vertical channel continues to control the cruise control setting, as before
- When the switch is in the middle position, the right vertical channel controls the front winch
- When the switch is in the bottom position, the right vertical channel controls the rear winch
- It is not possible to change cruise mode or select / deselect reverse while winch operation is taking place

### Version 1.0.0

Boot sequence - when the controller is powered on or set to On mode, all the light outputs will operate once in sequence

- All the light outputs will be flashed for half a second each in sequence when the Arduino is powered on, or if the mode switch is moved from Off to On
- For the duration of the boot sequence, all servos receive a centre signal

## Schematics / wiring diagram

To be provided

## Functionality not listed on roadmap / Wishlist / Gripe listed
