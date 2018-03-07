/**
 * Copyright 2018 Colin Law
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * See Timeprop.h for Usage
 *
 **/


#include "PID.h"

PID::PID() {
  m_initialised = 0;
}

void PID::initialise( double setpoint, double prop_band, double t_integral, double t_derivative,
  double integral_default, double smooth_factor, unsigned char mode_auto, double manual_op ) {

  m_setpoint = setpoint;
  m_prop_band = prop_band;
  m_t_integral = t_integral;
  m_t_derivative = t_derivative;
  m_integral_default = integral_default;
  m_smooth_factor= smooth_factor;
  m_mode_auto= mode_auto;
  m_manual_op = manual_op;

  m_initialised = 1;

}


/* called regularly to calculate and return new power value */
double PID::tick( unsigned long nowSecs ) {
  m_lastPvUpdateTime = nowSecs;


  // check whether too long has elapsed since power was last updated
  /*
  if (m_maxUpdateInterval > 0  &&  nowSecs - m_lastPowerUpdateTime > m_maxUpdateInterval) {
    // yes, go to fallback power

  }
  */
  return 0.5;
}
