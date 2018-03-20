/*
  user_config_override.h - user configuration overrides user_config.h for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

/*****************************************************************************************************\
 * ATTENTION: - Changes to most PARAMETER defines will only override flash settings if you change
 *              define CFG_HOLDER.
 *            - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *            - You still need to update user_config.h for major defines MODULE and USE_MQTT_TLS.
 *            - Changing MODULE defines are not being tested for validity as they are in user_config.h.
 *            - Most parameters can be changed online using commands via MQTT, WebConsole or serial.
 *            - So I see no use in this but anyway, your on your own.
\*****************************************************************************************************/

#ifdef CFG_HOLDER
#undef CFG_HOLDER
#endif
#define CFG_HOLDER           0x18031405   // yymmddnn  change this to force changes to be used immediately

#define USE_PID         // include the pid feature (+?k)
  #define PID_SETPOINT                  9.5    // setpoint
  #define PID_PROPBAND                  1.0     // proportional band in process units (eg degrees)
  #define PID_INTEGRAL_TIME             600     // integral time seconds
  #define PID_DERIVATIVE_TIME           0       // derivative time seconds
  #define PID_INITIAL_INT               0.5     // initial integral value (0:1)
  #define PID_MAX_INTERVAL              300     // max expected time between pv updates (used to fall back to safe power)
  #define PID_DERIV_SMOOTH_FACTOR       3       // derivative smoothing factor
  #define PID_AUTO                      1       // initial state enabled (1) or disabled (2)
  #define PID_MANUAL_POWER              0       // power output when loop is disabled
  #define PID_UPDATE_SECS               5       // how often to run the pid algorithm (integer secs)
  #define PID_USE_TIMPROP               1       // which timeprop settings to use (1 up), leave undefined if timeprop o/p not required
  #define PID_USE_LOCAL_SENSOR                  // if defined then the local sensor will be used for pv. Comment this out if not required


#define USE_TIMEPROP    //  include the timeprop feature (+1.2k)
  // for single output
  #define TIMEPROP_NUM_OUTPUTS          1       // how many outputs to control (with separate alogorithm for each)
  #define TIMEPROP_CYCLETIMES           60      // cycle time seconds
  #define TIMEPROP_DEADTIMES            0       // actuator action time seconds
  #define TIMEPROP_OPINVERTS            false   // whether to invert the output
  #define TIMEPROP_FALLBACK_POWERS      0       // falls back to this if too long betwen updates
  #define TIMEPROP_MAX_UPDATE_INTERVALS 120     // max no secs that are allowed between power updates (0 to disable)
  #define TIMEPROP_RELAYS               1       // which relay to control 1:8

  /* example for multiple outputs*/
  /*
  #define TIMEPROP_NUM_OUTPUTS          2               // how many outputs to control (with separate alogorithm for each)
  #define TIMEPROP_CYCLETIMES           60,     10      // cycle time seconds
  #define TIMEPROP_DEADTIMES            0,      0       // actuator action time seconds
  #define TIMEPROP_OPINVERTS            false,  false   // whether to invert the output
  #define TIMEPROP_FALLBACK_POWERS      0,      0       // falls back to this if too long betwen updates
  #define TIMEPROP_MAX_UPDATE_INTERVALS 120,    120     // max no secs that are allowed between power updates (0 to disable)
  #define TIMEPROP_RELAYS               1,      2       // which relay to control 1:8
  */

#ifdef WIFI_IP_ADDRESS
#undef WIFI_IP_ADDRESS
#endif
#define WIFI_IP_ADDRESS      "192.168.50.95"

#ifdef WIFI_GATEWAY
#undef WIFI_GATEWAY
#endif
#define WIFI_GATEWAY         "192.168.50.1"

#ifdef WIFI_SUBNETMASK
#undef WIFI_SUBNETMASK
#endif
#define WIFI_SUBNETMASK      "255.255.255.0"

#ifdef WIFI_DNS
#undef WIFI_DNS
#endif
#define WIFI_DNS             "192.168.49.1"

#ifdef STA_SSID1
#undef STA_SSID1
#endif
#define STA_SSID1            "pi02"

#ifdef STA_PASS1
#undef STA_PASS1
#endif
#define STA_PASS1            "ladycaroline"

#ifdef OTA_URL
#undef OTA_URL
#endif
#define OTA_URL               "http://192.168.49.92:1880/sonoff/firmware.bin"

#ifdef MQTT_HOST
#undef MQTT_HOST
#endif
#define MQTT_HOST            "192.168.49.85"                // pi02

#ifdef MQTT_USER
#undef MQTT_USER
#endif
#define MQTT_USER            ""

#ifdef MQTT_PASS
#undef MQTT_PASS
#endif
#define MQTT_PASS            ""

#ifdef MQTT_STATUS_OFF
#undef MQTT_STATUS_OFF
#endif
#define MQTT_STATUS_OFF        "Off"  // [StateText1] Command or Status result when turned off (needs to be a string like "0" or "Off")

#ifdef MQTT_STATUS_ON
#undef MQTT_STATUS_ON
#endif
#define MQTT_STATUS_ON         "On"  // [StateText1] Command or Status result when turned on (needs to be a string like "0" or "Off")

#ifdef MQTT_FULLTOPIC
#undef MQTT_FULLTOPIC
#endif
#define MQTT_FULLTOPIC         "tydwr/conservatory/%topic%/%prefix%/" // [FullTopic] Subscribe and Publish full topic name

#ifdef MQTT_TOPIC
#undef MQTT_TOPIC
#endif
#define MQTT_TOPIC             "s004"           // [Topic] (unique) MQTT device topic

#ifdef TELE_PERIOD
#undef TELE_PERIOD
#endif
#define TELE_PERIOD            10               // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)

#ifdef FRIENDLY_NAME
#undef FRIENDLY_NAME
#endif
#define FRIENDLY_NAME        "s004"          // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa

#ifdef APP_TIMEZONE
#undef APP_TIMEZONE
#endif
#define APP_TIMEZONE           0                 // [Timezone] UTC (-12 .. 12 = hours from UTC, 99 = use TIME_DST/TIME_STD)

#if defined TEMP_RESOLUTION
#undef TEMP_RESOLUTION
#endif
#define TEMP_RESOLUTION        3                  // 3 dec digits in temperature

// Unneeded extras

#ifdef USE_DOMOTICZ
#undef USE_DOMOTICZ
#endif

#ifdef USE_DISCOVERY
#undef USE_DISCOVERY                          // Disable Discovery services for both MQTT and web server
#endif

#ifdef USE_HOME_ASSISTANT
#undef USE_HOME_ASSISTANT
#endif

#ifdef USE_I2C
#undef USE_I2C
#endif

#ifdef USE_EMULATION
#undef USE_EMULATION                          // Disable Wemo or Hue emulation
#endif

#ifdef USE_MHZ19
#undef USE_MHZ19
#endif

#ifdef USE_SENSEAIR
#undef USE_SENSEAIR
#endif

#ifdef USE_PMS5003
#undef USE_PMS5003
#endif

#ifdef USE_PZEM004T
#undef USE_PZEM004T
#endif

#ifdef USE_IR_REMOTE
#undef USE_IR_REMOTE
#endif

#ifdef USE_IR_RECEIVE
#undef USE_IR_RECEIVE
#endif

#ifdef USE_WS2812
#undef USE_WS2812
#endif

#ifdef USE_ARILUX_RF
#undef USE_ARILUX_RF
#endif

#endif  // _USER_CONFIG_OVERRIDE_H_
