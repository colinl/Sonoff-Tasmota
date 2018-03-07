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

#define D_CMND_PID_SETPV "pv"
#define D_CMND_PID_SETSETPOINT "sp"
#define D_CMND_PID_SETPROPBAND "pb"
#define D_CMND_PID_SETINTEGRAL_TIME "ti"
#define D_CMND_PID_SETDERIVATIVE_TIME "td"
#define D_CMND_PID_SETINITIAL_INT "initint"
#define D_CMND_PID_SETDERIV_SMOOTH_FACTOR "d_smooth"
#define D_CMND_PID_SETAUTO "auto"
#define D_CMND_PID_SETMANUAL_POWER "manual_power"
#define D_CMND_PID_SETUPDATE_SECS "update_secs"

enum PIDCommands { CMND_PID_SETPV, CMND_PID_SETSETPOINT, CMND_PID_SETPROPBAND, CMND_PID_SETINTEGRAL_TIME,
  CMND_PID_SETDERIVATIVE_TIME, CMND_PID_SETINITIAL_INT, CMND_PID_SETDERIV_SMOOTH_FACTOR, CMND_PID_SETAUTO,
  CMND_PID_SETMANUAL_POWER, CMND_PID_SETUPDATE_SECS };
const char kPIDCommands[] PROGMEM = D_CMND_PID_SETPV "|" D_CMND_PID_SETSETPOINT "|" D_CMND_PID_SETPROPBAND "|"
  D_CMND_PID_SETINTEGRAL_TIME "|" D_CMND_PID_SETDERIVATIVE_TIME "|" D_CMND_PID_SETINITIAL_INT "|" D_CMND_PID_SETDERIV_SMOOTH_FACTOR "|"
  D_CMND_PID_SETAUTO "|" D_CMND_PID_SETMANUAL_POWER "|" D_CMND_PID_SETUPDATE_SECS;

static PID pid;

void PID_Init()
{
  snprintf_P(log_data, sizeof(log_data), "PID Init");
  AddLog(LOG_LEVEL_INFO);
  pid.initialise( PID_SETPOINT, PID_PROPBAND, PID_INTEGRAL_TIME, PID_DERIVATIVE_TIME, PID_INITIAL_INT,
    PID_DERIV_SMOOTH_FACTOR, PID_AUTO, PID_MANUAL_POWER );
}

void PID_Every_Second() {
  static int sec_counter = 0;
  if (sec_counter++ % PID_UPDATE_SECS  ==  0) {
    snprintf_P(log_data, sizeof(log_data), "Calling PID::tick()");
    AddLog(LOG_LEVEL_INFO);
    double power = pid.tick(utc_time);
    snprintf_P(log_data, sizeof(log_data), "Power from PID::tick(): %d%", power*100);
    AddLog(LOG_LEVEL_INFO);
  }
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
    // command starts with pid__
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + ua_prefix_len, kPIDCommands);
    serviced = true;
    switch (command_code) {
      case CMND_PID_SETPV:
      /*
      snprintf_P(log_data, sizeof(log_data), "Timeprop command timeprop_setpower: "
        "index: %d data_len: %d payload: %d topic: %s data: %s",
	      XdrvMailbox.index,
	      XdrvMailbox.data_len,
	      XdrvMailbox.payload,
	      (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
	      (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));
        AddLog(LOG_LEVEL_INFO);

      if (XdrvMailbox.index >=0 && XdrvMailbox.index < TIMEPROP_NUM_OUTPUTS) {
        timeprops[XdrvMailbox.index].setPower( atof(XdrvMailbox.data), utc_time );
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_TIMEPROP D_CMND_TIMEPROP_SETPOWER "%d\":\"%s\"}"),
        XdrvMailbox.index, XdrvMailbox.data);
        */
      break;

    case CMND_PID_SETSETPOINT:
      break;

    }

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
