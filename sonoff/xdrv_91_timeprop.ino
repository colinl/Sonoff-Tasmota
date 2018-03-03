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
#define D_CMND_TIMEPROP "timeprop_"
#define D_CMND_TIMEPROP_SETPOWER "setpower"    // data is power 0:1
#define D_CMND_TIMEPROP_CMD_B "CmndB"

enum TimepropCommands { CMND_TIMEPROP_SETPOWER, CMND_TIMEPROP_CMD_B };
const char kTimepropCommands[] PROGMEM = D_CMND_TIMEPROP_SETPOWER "|" D_CMND_TIMEPROP_CMD_B;

class Timeprop {
public:
  /*
    Initialiser given
      cycleTime seconds
      actuator deadTime seconds
      whether to invert the output
      fallback power value if updates are not received within time below
      max number of seconds to allow between updates before falling back to default power
      relay number to use 1:8
  */
  void initialise( int cycleTime, int deadTime, boolean invert, float fallbackPower, int maxUpdateInterval, int relayNo);

  /* set current power required 0:1 */
  void setPower( float power );

  /* call this regularly to update the output */
  void tick();

private:
  int m_cycleTime;        // cycle time seconds, float to force float calcs
  int m_deadTime;         // actuator action time seconds
  boolean m_invert;       // whether to invert the output
  int m_relayNo;          // relay number to use 1:8
  float m_dtoc;           // deadTime/m_cycleTime
  int m_opState;          // current output state (before invert)
  float m_power;          // required power 0:1
  float m_fallbackPower;  // falls back to this if updates not received with max allowed timezone
  int m_maxUpdateInterval;  // max time between updates
  uint32_t m_lastPowerUpdateTime;   // the time of last power update
};

void Timeprop::initialise( int cycleTime, int deadTime, boolean invert, float fallbackPower, int maxUpdateInterval, int relayNo) {
  m_cycleTime = cycleTime;
  m_deadTime = deadTime;
  m_invert = invert;
  m_relayNo = relayNo;
  m_fallbackPower = fallbackPower;
  m_maxUpdateInterval = maxUpdateInterval;

  m_dtoc = (float)deadTime/cycleTime;
  m_opState = -1;   // current output state, initialise to illegal value to indicate unknown
  setPower(m_fallbackPower);
}

inline void Timeprop::setPower( float power ) {
  m_power = power;
  m_lastPowerUpdateTime = utc_time;
}

/* called regularly to update the output */
void Timeprop::tick() {
  int newState;
  float wave;
  float direction;
  float effectivePower;

  // check whether too long has elapsed since power was last updated
  if (utc_time - m_lastPowerUpdateTime > m_maxUpdateInterval) {
    snprintf_P(log_data, sizeof(log_data), "No timeprop power updates, reverting to fallback value");
    AddLog(LOG_LEVEL_INFO);
    setPower(m_fallbackPower);
  }

  wave = (utc_time % m_cycleTime)/(float)m_cycleTime;
  // determine direction of travel and convert to triangular wave
  if (wave < 0.5) {
    direction = 1;      // on the way up
    wave = wave*2;
  } else {
    direction = -1;     // on the way down
    wave = (1 - wave)*2;
  }
  //int wave_pc = wave * 100;
  //snprintf_P(log_data, sizeof(log_data), "wave_pc: %d, direction %d", wave_pc, direction);
  //AddLog(LOG_LEVEL_INFO);
  // if a dead_time has been supplied for this o/p then adjust power accordingly
  if (m_deadTime > 0  && m_power > 0.0  &&  m_power < 1.0) {
      effectivePower = (1.0-2.0*m_dtoc)*m_power + m_dtoc;
  } else {
      effectivePower = m_power;
  }
  //int epower_pc = effectivePower * 100;
  //int power_pc = timeprop_power * 100;
  //snprintf_P(log_data, sizeof(log_data), "power_pc: %d, epower_pc: %d", power_pc, epower_pc);
  //AddLog(LOG_LEVEL_INFO);
  //  cope with end cases in case values outside 0..1
  if (effectivePower <= 0.0) {
      newState = 0;     // no heat
  } else if (effectivePower >= 1.0) {
      newState = 1;     // full heat
  } else {
      // only allow power to come on on the way down and off on the way up, to reduce short pulses
      if (effectivePower >= wave  &&  direction == -1) {
          newState = 1;
          //snprintf_P(log_data, sizeof(log_data), "setting to 1");
          //AddLog(LOG_LEVEL_INFO);
      } else if (effectivePower <= wave  &&  direction == 1) {
          newState = 0;
          //snprintf_P(log_data, sizeof(log_data), "setting to 0");
          //AddLog(LOG_LEVEL_INFO);
      } else {
          // otherwise leave it as it is
          newState = m_opState;
          //snprintf_P(log_data, sizeof(log_data), "leaving as is");
          //AddLog(LOG_LEVEL_INFO);
      }
  }
  if (newState != m_opState) {
    m_opState = newState;
    ExecuteCommandPower(m_relayNo, m_invert ? (1-m_opState) : m_opState);
  }
}

static Timeprop timeprop;

void Timeprop_Init()
{
  snprintf_P(log_data, sizeof(log_data), "Timeprop Init");
  AddLog(LOG_LEVEL_INFO);
  timeprop.initialise(TIMEPROP_CYCLETIME, TIMEPROP_DEADTIME, TIMEPROP_OPINVERT, TIMEPROP_FALLBACK_POWER,
    TIMEPROP_MAX_UPDATE_INTERVAL, 1);
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
  timeprop.tick();
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
/*
  snprintf_P(log_data, sizeof(log_data), "Command called: "
    "index: %d data_len: %d payload: %d topic: %s data: %s\n",
    XdrvMailbox.index,
    XdrvMailbox.data_len,
    XdrvMailbox.payload,
    (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
    (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));

    AddLog(LOG_LEVEL_INFO);
*/
  if (0 == strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_TIMEPROP), ua_prefix_len)) {
    // command starts with Timeprop
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + ua_prefix_len, kTimepropCommands);
    //snprintf_P(log_data, sizeof(log_data), "Timeprop Command found: %d", command_code);
    //AddLog(LOG_LEVEL_INFO);
    if (CMND_TIMEPROP_SETPOWER == command_code) {

      snprintf_P(log_data, sizeof(log_data), "Timeprop command timeprop_setpower: "
        "index: %d data_len: %d payload: %d topic: %s data: %s",
	      XdrvMailbox.index,
	      XdrvMailbox.data_len,
	      XdrvMailbox.payload,
	      (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
	      (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));
        AddLog(LOG_LEVEL_INFO);
        //timeprop_power = atof(XdrvMailbox.data);
        timeprop.setPower( atof(XdrvMailbox.data) );
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_TIMEPROP D_CMND_TIMEPROP_SETPOWER "\":\"%s\"}"),XdrvMailbox.data);
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
  case FUNC_EVERY_SECOND:
    Timeprop_Every_Second();
    break;
  case FUNC_COMMAND:
    result = Timeprop_Command();
    break;
  }
  return result;
}

#endif // USE_TIMEPROP
