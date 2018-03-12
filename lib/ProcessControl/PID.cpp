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

extern void AddLog(unsigned char loglevel);
extern char log_data[400];
extern void snprintf_P(char*, unsigned int, char const*, ...);
extern char* dtostrfd(double number, unsigned char prec, char *s);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};
char buf[10];

PID::PID() {
  m_initialised = 0;
  m_last_sample_time = 0;
  m_lastPvUpdateTime = 0;
}

void PID::initialise( double setpoint, double prop_band, double t_integral, double t_derivative,
  double integral_default, int max_interval, double smooth_factor, unsigned char mode_auto, double manual_op ) {

  m_setpoint = setpoint;
  m_prop_band = prop_band;
  m_t_integral = t_integral;
  m_t_derivative = t_derivative;
  m_integral_default = integral_default;
  m_max_interval = max_interval;
  m_smooth_factor= smooth_factor;
  m_mode_auto= mode_auto;
  m_manual_op = manual_op;

  m_initialised = 1;

}


/* called regularly to calculate and return new power value */
double PID::tick( unsigned long nowSecs ) {

  // check whether too long has elapsed since pv was last updated
  /*
  if (m_maxUpdateInterval > 0  &&  nowSecs - m_lastPowerUpdateTime > m_maxUpdateInterval) {
    // yes, go to fallback power

  }
  */
  double power;
  unsigned char integral_locked = 0;
  double factor;
  if (m_initialised && m_lastPvUpdateTime) {
    snprintf_P(log_data, sizeof(log_data), "Initialised and pv");
    AddLog(LOG_LEVEL_INFO);
    // we have been initialised and have been given a pv value
    // is this the first time through here?
    if (m_last_sample_time) {
        snprintf_P(log_data, sizeof(log_data), "Not first time");
        AddLog(LOG_LEVEL_INFO);
      // not first time
      unsigned long delta_t = nowSecs - m_last_sample_time;  // seconds
        snprintf_P(log_data, sizeof(log_data), "delta_t: %d", delta_t);
        AddLog(LOG_LEVEL_INFO);
      if (delta_t <= 0 || delta_t > m_max_interval) {
        // too long since last sample so leave integral as is and set deriv to zero
          snprintf_P(log_data, sizeof(log_data), "Too long");
          AddLog(LOG_LEVEL_INFO);
        m_derivative = 0;
      } else {
        if (m_smooth_factor > 0) {
          // A derivative smoothing factor has been supplied
          // smoothing time constant is td/factor but with a min of delta_t to stop overflows
          int ts = m_t_derivative/m_smooth_factor > delta_t ? m_t_derivative/m_smooth_factor : delta_t;
          factor = 1.0/(ts/delta_t);
        } else {
          // no integral smoothing so factor is 1, this makes smoothed_value the previous pv
          factor = 1.0;
        }
        dtostrfd(factor, 2, buf);
        snprintf_P(log_data, sizeof(log_data), "factor: %s", buf);
        AddLog(LOG_LEVEL_INFO);
        double delta_v = (m_pv - m_smoothed_value) * factor;
        m_smoothed_value = m_smoothed_value + delta_v;
        m_derivative = m_t_derivative * delta_v/delta_t;
        dtostrfd(m_smoothed_value, 2, buf);
        snprintf_P(log_data, sizeof(log_data), "m_smoothed_value: %s", buf);
        AddLog(LOG_LEVEL_INFO);
        dtostrfd(m_derivative, 2, buf);
        snprintf_P(log_data, sizeof(log_data), "m_derivative: %s", buf);
        AddLog(LOG_LEVEL_INFO);
        // lock the integral if abs(previous integral + error) > prop_band/2
        // as this means that P + I is outside the linear region so power will be 0 or full
        // also lock if control is disabled
        double error = m_pv - m_setpoint;
        dtostrfd(error, 2, buf);
        snprintf_P(log_data, sizeof(log_data), "error: %s", buf);
        AddLog(LOG_LEVEL_INFO);
        double pbo2 = m_prop_band/2.0;
        double epi = error + m_integral;
        if (epi < 0.0) epi = -epi;    // abs value of error + m_integral
        if (epi < pbo2  && m_mode_auto) {
          integral_locked = 0;
          m_integral = m_integral + error * delta_t/m_t_integral;
          // clamp to +- 0.5 prop band widths so that it cannot push the zero power point outside the pb
          if ( m_integral < -pbo2 ) {
            m_integral = -pbo2;
          } else if (m_integral > pbo2) {
            m_integral = pbo2;
          }
        } else {
          snprintf_P(log_data, sizeof(log_data), "integral locked");
          AddLog(LOG_LEVEL_INFO);
          integral_locked = 1;
        }
        dtostrfd(m_integral, 2, buf);
        snprintf_P(log_data, sizeof(log_data), "m_integral: %s", buf);
        AddLog(LOG_LEVEL_INFO);
      }

    } else {
      // first time through, initialise context data
      m_smoothed_value = m_pv;
      // setup the integral term so that the power out would be integral_default if pv=setpoint
      m_integral = (0.5 - m_integral_default)*m_prop_band;
      m_derivative = 0.0;
      dtostrfd(m_integral, 2, buf);
      snprintf_P(log_data, sizeof(log_data), "First time m_integral: %s", buf);
      AddLog(LOG_LEVEL_INFO);
    }

    double proportional = m_pv - m_setpoint;
    dtostrfd(proportional, 2, buf);
    snprintf_P(log_data, sizeof(log_data), "proportional: %s", buf);
    AddLog(LOG_LEVEL_INFO);
    power = -1.0/m_prop_band * (proportional + m_integral + m_derivative) + 0.5;
    dtostrfd(power, 2, buf);
    snprintf_P(log_data, sizeof(log_data), "power: %s", buf);
    AddLog(LOG_LEVEL_INFO);
    if (power < 0.0) {
      power = 0.0;
    } else if (power > 1.0) {
      power = 1.0;
    }
    // set power to disabled value if the loop is not enabled
    if (!m_mode_auto) {
      power = m_manual_op;
    }
    m_last_sample_time = nowSecs;
  } else {
    // not yet initialised or no pv value yet so set power to disabled value
    snprintf_P(log_data, sizeof(log_data), "PID not yet initialised or no pv");
    AddLog(LOG_LEVEL_INFO);
    power = m_manual_op;
  }
  return power;
}

// call to pass in new process value
void PID::setPv( double pv, unsigned long nowSecs ){
  m_pv = pv;
  m_lastPvUpdateTime = nowSecs;
}

// methods to modify configuration data
void PID::setSp( double setpoint ) {
  m_setpoint = setpoint;
}

void PID::setPb( double prop_band ) {
  m_prop_band = prop_band;
}

void PID::setTi( double t_integral ) {
  m_t_integral = t_integral;
}

void PID::setTd( double t_derivative ) {
  m_t_derivative = t_derivative;
}

void PID::setInitialInt( double integral_default ) {
  m_integral_default = integral_default;
}

void PID::setDSmooth( double smooth_factor ) {
  m_smooth_factor = smooth_factor;
}

void PID::setAuto( unsigned char mode_auto ) {
  m_mode_auto = mode_auto;
}

void PID::setManualPower( double manual_op ) {
  m_manual_op = manual_op;
}

void PID::setMaxInterval( int max_interval ) {
  m_max_interval = max_interval;
}
