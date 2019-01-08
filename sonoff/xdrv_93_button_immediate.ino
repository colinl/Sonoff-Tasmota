/*
  xdrv_93_button_immediate.ino - Enhanced immediate button support for Sonoff-Tasmota
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
 * Code to give more immediate button handling particularly for Sonoff T1
 * To enable this add build with this file and define USE_BUTTON_IMMEDIATE
 */

#ifdef USE_BUTTON_IMMEDIATE

// long press tick count 1 second @ 50ms
#define BI_LONG_PRESS_TICK_COUNT 20
// double press tick count, max 50 ms ticks since last pressed (2 secs)
#define BI_DOUBLE_PRESS_TICK_COUNT 40
boolean bi_pressed[MAX_KEYS] = { false };
int bi_tick_counter[MAX_KEYS] = { 0 };
boolean  bi_long_press_passed[MAX_KEYS] = { false };


/* struct XDRVMAILBOX { */
/*   uint16_t      valid; */
/*   uint16_t      index; */
/*   uint16_t      data_len; */
/*   int16_t       payload; */
/*   char         *topic; */
/*   char         *data; */
/* } XdrvMailbox; */

boolean ButtonImmediatePressed()
{
  int index = XdrvMailbox.index;
  if ((PRESSED == XdrvMailbox.payload) && (NOT_PRESSED == lastbutton[index])) {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_LEVEL_10), index +1);
    AddLog(LOG_LEVEL_INFO);
    // the button has been pressed, check for quick double press
    if (bi_tick_counter[index] >= BI_DOUBLE_PRESS_TICK_COUNT)
    {
      // a long time since last press so not a double press,  toggle the relay
      // beware, this changes XdrvMailbox I think
      ExecuteCommandPower(index + 1, POWER_TOGGLE, SRC_BUTTON);
    }
    // restart the counter
    bi_pressed[index] = true;
    bi_tick_counter[index] = 0;
    bi_long_press_passed[index] = false;
    // and send MQTT toggle provided ButtonTopic is set
    SendKey(0, index + 1, POWER_TOGGLE);
  } else if ((NOT_PRESSED == XdrvMailbox.payload) && (PRESSED == lastbutton[index]))
  {

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_LEVEL_01 "  %d ticks"), index + 1, bi_tick_counter[index]);
    AddLog(LOG_LEVEL_INFO);
    // the button has been released
    bi_pressed[index] = false;
  }
  return true;  // Serviced here
}

void ButtonImmediateTick()
{
  // called every 50ms
  uint8_t i;
  uint8_t max = (devices_present > MAX_KEYS) ? MAX_KEYS : devices_present;
  for ( i = 0; i < max; i++ )
  {
    bi_tick_counter[i]++;
    // is the button pressed?
    if (bi_pressed[i]  &&  !bi_long_press_passed[i])
    {
      // yes increment the counter and see if enough time has passed
      if (bi_tick_counter[i] >= BI_LONG_PRESS_TICK_COUNT)
      {
        // stop this happening again
        bi_long_press_passed[i] = true;
        // send a long press mqtt message
        SendKey(0, i + 1, 3);
      }
    }
  }
}
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_93

boolean Xdrv93(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_BUTTON_PRESSED:
      result = ButtonImmediatePressed();
      break;
    case FUNC_EVERY_50_MSECOND:
      ButtonImmediateTick();
      break;
    }
  return result;
}


#endif // USE_BUTTON_IMMEDIATE
