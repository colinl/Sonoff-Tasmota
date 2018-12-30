/*
  user_config_override.h - user configuration overrides my_user_config.h for Sonoff-Tasmota

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

// force the compiler to show a warning to confirm that this file is inlcuded
#warning **** user_config_override.h: Using Settings from this File ****

/*****************************************************************************************************\
 * USAGE:
 *   To modify the stock configuration without changing the my_user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *   (3) for platformio:
 *         define USE_CONFIG_OVERRIDE as a build flags.
 *         ie1 : export PLATFORMIO_BUILD_FLAGS='-DUSE_CONFIG_OVERRIDE'
 *         ie2 : enable in file platformio.ini "build_flags = -Wl,-Tesp8266.flash.1m0.ld -DUSE_CONFIG_OVERRIDE"
 *       for Arduino IDE:
 *         enable define USE_CONFIG_OVERRIDE in my_user_config.h
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update my_user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/

#ifdef CFG_HOLDER
#undef CFG_HOLDER
#endif
#define CFG_HOLDER           0x18123002   // yymmddnn  change this to force changes to be used immediately

#ifdef MODULE
#undef MODULE
#endif
#define MODULE               SONOFF_T11 // [Module] Sonoff T1 1 channel

#ifdef WIFI_IP_ADDRESS
#undef WIFI_IP_ADDRESS
#endif
#define WIFI_IP_ADDRESS      "192.168.49.84"

#ifdef WIFI_GATEWAY
#undef WIFI_GATEWAY
#endif
#define WIFI_GATEWAY         "192.168.49.1"

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
#define STA_SSID1            "RedKite"

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
#define MQTT_HOST            "192.168.49.83"                // owl2

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
#define MQTT_FULLTOPIC         "tydwr/kitchen/%topic%/%prefix%/" // [FullTopic] Subscribe and Publish full topic name

#ifdef MQTT_TOPIC
#undef MQTT_TOPIC
#endif
#define MQTT_TOPIC             "s007"           // [Topic] (unique) MQTT device topic

#ifdef TELE_PERIOD
#undef TELE_PERIOD
#endif
#define TELE_PERIOD            60               // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)

#ifdef FRIENDLY_NAME
#undef FRIENDLY_NAME
#endif
#define FRIENDLY_NAME        "s007"          // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa

#ifdef APP_TIMEZONE
#undef APP_TIMEZONE
#endif
#define APP_TIMEZONE           0                 // [Timezone] UTC (-12 .. 12 = hours from UTC, 99 = use TIME_DST/TIME_STD)

#if defined TEMP_RESOLUTION
#undef TEMP_RESOLUTION
#endif
#define TEMP_RESOLUTION        3                  // 3 dec digits in temperature

# if defined KEY_HOLD_TIME
#undef KEY_HOLD_TIME
#endif
#define KEY_HOLD_TIME          10                // [SetOption32] Number of 0.1 seconds to hold Button or external Pushbutton before sending HOLD message

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

#ifdef USE_TIMERS
#undef USE_TIMERS
#endif

#ifdef USE_DS18x20
#undef USE_DS18x20
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

#ifdef USE_NOVA_SDS
#undef USE_NOVA_SDS
#endif

#ifdef USE_SERIAL_BRIDGE
#undef USE_SERIAL_BRIDGE
#endif

#ifdef USE_TUYA_DIMMER
#undef USE_TUYA_DIMMER
#endif

#ifdef USE_PZEM004T
#undef USE_PZEM004T
#endif

#ifdef USE_PZEM_AC
#undef USE_PZEM_AC
#endif

#ifdef USE_PZEM_DC
#undef USE_PZEM_DC
#endif

#ifdef USE_MCP39F501
#undef USE_MCP39F501
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

#ifdef USE_SR04
#undef USE_SR04
#endif

#ifdef USE_HX711
#undef USE_HX711
#endif

#ifdef USE_TX20_WIND_SENSOR
#undef USE_TX20_WIND_SENSOR
#endif

#ifdef USE_RC_SWITCH
#undef USE_RC_SWITCH
#endif

#endif  // _USER_CONFIG_OVERRIDE_H_
