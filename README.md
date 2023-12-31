# RIDS - Remote ID Spoofer

An ESP8266/NodeMCU Drone RemoteID Spoofer.
Built based on work done by 
* [jjshoots](https://github.com/jjshoots/RemoteIDSpoofer)
* [sxjack](https://github.com/sxjack/uav_electronic_ids)
* [SpacehuhnTech](https://github.com/SpacehuhnTech/esp8266_deauther).

I stand on the shoulders of giants.

This spawns 4 different fake drones broadcasting RemoteID, with them all flying in random directions around the current location.

## Installation

1. You need the [Arduino IDE](https://www.arduino.cc/en/software).
2. Open the file `RemoteIDSpoofer/RemoteIDSpoofer.ino`.
3. In Arduino IDE, go to `File` > `Preferences`, then add this URL to the `Additional Boards Manager URLs`:
	- https://raw.githubusercontent.com/SpacehuhnTech/arduino/main/package_spacehuhn_index.json
4. Now go to `Tools` > `Boards` > `Boards Manager`, search `deauther` and install `Deauther ESP8266 Boards`.
5. Select your board at `Tools` > `Board` > and be sure it is at `Deauther ESP8266 Boards` (and not at `ESP8266 Modules`).
6. Solder GPS to NodeMCU <br/><img src="./images/wiring.png"  width="400">
7. Install TinyGPSPlus library using Arduino IDE Library Manager
8. Plug in your device, I used a NodeMCU v2, and select its COM port at `Tools` > `Port`.
9. Press `upload`, or use Ctrl+U.
10. The device should start broadcasting RemoteID packets generated for random flying machines.

## Changes from jjroots version

1. Updated the faked operator ID to properly set country of origin to US
2. Updated to generate fake UAS ID for each drone
3. Ensures that you have a proper GPS lock (> 7 satellites) before spoofing
4. Gets time/date from GPS
5. Limits Altitude to less than 400 feet
6. Updates pilot location if spoofer is moved

## OpenDroneID App Notes

1. Best performance and results on Android when you disable Wifi Scan Throttling. It will eat your battery though.
2. Allow plenty of time (5 - 10 minutes) for the applicationt to get a fix on all of the drones

## Disclaimer

This repository and its code are intended for educational purposes only.
Neither the ESP8266, nor its SDK were meant or built for such purposes.
Bugs can occur!

It is also illegal to be broadcasting fake RemoteID packets in public airspace, in the same way that spoofing ADS-B packets is illegal.
Whatever manner you wish to use this on is at your own discretion, we don't take any responsibility for what you do with this software.
