/*
  xdrv_91_timeprop.ino - Timeprop support for Sonoff-Tasmota
  Copyright (C) 2018 Colin Law and Thomas Herrmann
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

/**
 * Code to
 *
 * Usage:
 * Place this file in the sonoff folder.
 * Clone the library https://github.com/colinl/process-control.git from Github
 * into a subfolder of lib.
 * In user_config.h or user_config_override.h for a single relay, include
 * code as follows:

 #define USE_PID    //  include the pid feature (+?k)


 * Publish values .. to the topic(s) described above
 *
**/


#ifdef USE_PID

# include "PID.h"

#define D_CMND_PID "pid_"
/*
#define D_CMND_TIMEPROP_SETPOWER "setpower_"    // add index no on end (0:8) and data is power 0:1

enum TimepropCommands { CMND_TIMEPROP_SETPOWER };
const char kTimepropCommands[] PROGMEM = D_CMND_TIMEPROP_SETPOWER;
*/
void PID_Init()
{
  snprintf_P(log_data, sizeof(log_data), "PID Init");
  AddLog(LOG_LEVEL_INFO);
  /*
  int cycleTimes[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_CYCLETIMES};
  int deadTimes[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_DEADTIMES};
  int opInverts[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_OPINVERTS};
  int fallbacks[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_FALLBACK_POWERS};
  int maxIntervals[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_MAX_UPDATE_INTERVALS};

  for (int i=0; i<TIMEPROP_NUM_OUTPUTS; i++) {
    timeprops[i].initialise(cycleTimes[i], deadTimes[i], opInverts[i], fallbacks[i],
      maxIntervals[i], utc_time);
  }
  */
}

void PID_Every_Second() {
  /*
  for (int i=0; i<TIMEPROP_NUM_OUTPUTS; i++) {
    int newState = timeprops[i].tick(utc_time);
    if (newState != -1) {   // -1 means leave as is
      ExecuteCommandPower(relayNos[i], newState);
    }
  }
  */
}

/* struct XDRVMAILBOX { */
/*   uint16_t      valid; */
/*   uint16_t      index; */
/*   uint16_t      data_len; */
/*   int16_t       payload; */
/*   char         *topic; */
/*   char         *data; */
/* } XdrvMailbox; */

// To get here post with topic cmnd/timeprop_setpower_n where n is index into timeprops 0:7
boolean PID_Command()
{
  char command [CMDSZ];
  boolean serviced = true;
  uint8_t ua_prefix_len = strlen(D_CMND_PID); // to detect prefix of command

  snprintf_P(log_data, sizeof(log_data), "Command called: "
    "index: %d data_len: %d payload: %d topic: %s data: %s\n",
    XdrvMailbox.index,
    XdrvMailbox.data_len,
    XdrvMailbox.payload,
    (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
    (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));

    AddLog(LOG_LEVEL_INFO);

  if (0 == strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_PID), ua_prefix_len)) {
    #if 0
    // command starts with timeprop_
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + ua_prefix_len, kTimepropCommands);
    if (CMND_TIMEPROP_SETPOWER == command_code) {
      /*
      snprintf_P(log_data, sizeof(log_data), "Timeprop command timeprop_setpower: "
        "index: %d data_len: %d payload: %d topic: %s data: %s",
	      XdrvMailbox.index,
	      XdrvMailbox.data_len,
	      XdrvMailbox.payload,
	      (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
	      (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));
        AddLog(LOG_LEVEL_INFO);
      */
      if (XdrvMailbox.index >=0 && XdrvMailbox.index < TIMEPROP_NUM_OUTPUTS) {
        timeprops[XdrvMailbox.index].setPower( atof(XdrvMailbox.data), utc_time );
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_TIMEPROP D_CMND_TIMEPROP_SETPOWER "%d\":\"%s\"}"),
        XdrvMailbox.index, XdrvMailbox.data);
    }
    else {
      serviced = false;
    }
    #endif //0
    serviced = false;     // remove this ************************************************
  } else {
    serviced = false;
  }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_92

boolean Xdrv92(byte function)
{
  boolean result = false;

  switch (function) {
  case FUNC_INIT:
    PID_Init();
    break;
  case FUNC_EVERY_SECOND:
    PID_Every_Second();
    break;
  case FUNC_COMMAND:
    result = PID_Command();
    break;
  }
  return result;
}

#endif // USE_TIMEPROP
