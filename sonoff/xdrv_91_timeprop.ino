/*
  xdrv_08_user_application.ino - Timeprop support for Sonoff-Tasmota
  Copyright (C) 2018 Thomas Herrmann
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


/* This file can be used as a starting point for logic running locally on the device,
   e.g. to run a thermostat or similar without the need for a home automation platform
   or MQTT server.
   Local logic may also be used for critical control tasks that must be running if
   the automation fails */

#ifdef USE_TIMEPROP

enum TimepropCommands { CMND_TIMEPROP_SETPOWER, CMND_TIMEPROP_CMD_B };
const char kTimepropCommands[] PROGMEM = D_CMND_TIMEPROP_SETPOWER "|" D_CMND_TIMEPROP_CMD_B;

static double timeprop_power = 0.0;

void Timeprop_Init()
{
  snprintf_P(log_data, sizeof(log_data), "Timeprop Init");
  AddLog(LOG_LEVEL_INFO);
}

int counter_50ms = 0;
void Timeprop_Every_50ms() {
  // CDL edited later so this is not called
  counter_50ms++;
  if (counter_50ms % 200 == 0) {
    snprintf_P(log_data, sizeof(log_data), "200 calls of Timeprop Every 50ms");
    AddLog(LOG_LEVEL_INFO);
   }
}

void Timeprop_Every_Second() {
  ExecuteCommandPower(1, timeprop_power >= 0.5 ? 1 : 0);
  //snprintf_P(log_data, sizeof(log_data), "Timeprop Every Second");
  //AddLog(LOG_LEVEL_INFO);
}

/* struct XDRVMAILBOX { */
/*   uint16_t      valid; */
/*   uint16_t      index; */
/*   uint16_t      data_len; */
/*   int16_t       payload; */
/*   char         *topic; */
/*   char         *data; */
/* } XdrvMailbox; */

// To get here post with topic cmnd/timeprop_setpower etc
boolean Timeprop_Command()
{
  char command [CMDSZ];
  boolean serviced = true;
  uint8_t ua_prefix_len = strlen(D_CMND_TIMEPROP); // to detect prefix of command

  snprintf_P(log_data, sizeof(log_data), "Command called: "
    "index: %d data_len: %d payload: %d topic: %s data: %s\n",
    XdrvMailbox.index,
    XdrvMailbox.data_len,
    XdrvMailbox.payload,
    (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
    (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));

    AddLog(LOG_LEVEL_INFO);

  if (0 == strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_TIMEPROP), ua_prefix_len)) {
    // command starts with Timeprop
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + ua_prefix_len, kTimepropCommands);
    snprintf_P(log_data, sizeof(log_data), "Timeprop Command found: %d", command_code);

      AddLog(LOG_LEVEL_INFO);
    if (CMND_TIMEPROP_SETPOWER == command_code) {
      snprintf_P(log_data, sizeof(log_data), "Timeprop command timeprop_setpower called: "
        "index: %d data_len: %d payload: %d topic: %s data: %s\n",
	      XdrvMailbox.index,
	      XdrvMailbox.data_len,
	      XdrvMailbox.payload,
	      (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
	      (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));

        AddLog(LOG_LEVEL_INFO);
        timeprop_power = atof(XdrvMailbox.data);
    }
    else if ((CMND_TIMEPROP_CMD_B == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_DOMOTICZ_IDX)) {
      // if (XdrvMailbox.payload >= 0) {
      //   Settings.domoticz_key_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
      // }
      snprintf_P(log_data, sizeof(log_data), "Timeprop Command B called.");
      AddLog(LOG_LEVEL_INFO);
      // todo:
      // snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DOMOTICZ_COMMAND_INDEX_LVALUE, command, XdrvMailbox.index,
      // Settings.domoticz_key_idx[XdrvMailbox.index -1]);
    }
    else {
      serviced = false;
    }
  } else {
    serviced = false;
  }
  return serviced;
}

void Timeprop_Show_Sensor() {
  // todo: find out what this is used for
  snprintf_P(log_data, sizeof(log_data), "Timeprop Show Sensor");
  AddLog(LOG_LEVEL_INFO);
}

void Timeprop_Set_Power() {
  // todo: find out what this is used for
  snprintf_P(log_data, sizeof(log_data), "Timeprop Set Power");
  AddLog(LOG_LEVEL_INFO);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_91

boolean Xdrv91(byte function)
{
  boolean result = false;

  switch (function) {
  case FUNC_INIT:
    Timeprop_Init();
    break;
  case FUNC_EVERY_50_MSECOND:
    // Timeprop_Every_50ms();       CDL Don't need this
    break;
  case FUNC_EVERY_SECOND:
    Timeprop_Every_Second();
    break;
  case FUNC_COMMAND:
    result = Timeprop_Command();
    break;
  case FUNC_SHOW_SENSOR:
    Timeprop_Show_Sensor();
    break;
  case FUNC_SET_POWER:
    Timeprop_Set_Power();
    break;
    /* FUNC_MQTT_SUBSCRIBE */
    /*   FUNC_MQTT_INIT */
    /*   return FUNC_MQTT_DATA */
    /*   return FUNC_COMMAND */
  }
  return result;
}

#endif // USE_TIMEPROP
