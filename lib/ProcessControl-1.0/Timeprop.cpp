#include "Timeprop.h"

void Timeprop::initialise( int cycleTime, int deadTime, unsigned char invert, float fallbackPower, int maxUpdateInterval,
  unsigned long nowSecs) {
  m_cycleTime = cycleTime;
  m_deadTime = deadTime;
  m_invert = invert;
  m_fallbackPower = fallbackPower;
  m_maxUpdateInterval = maxUpdateInterval;

  m_dtoc = (float)deadTime/cycleTime;
  m_opState = -1;   // current output state, initialise to illegal value to indicate unknown
  setPower(m_fallbackPower, nowSecs);
}

/* called regularly to provide new output value */
/* returns new o/p state 0, 1 or -1 to leave as is */
int Timeprop::tick( unsigned long nowSecs) {
  int answer = -1;
  int newState;
  float wave;
  float direction;
  float effectivePower;

  // check whether too long has elapsed since power was last updated
  if (nowSecs - m_lastPowerUpdateTime > m_maxUpdateInterval) {
    // yes, go to fallback power
    setPower(m_fallbackPower, nowSecs);
  }

  wave = (nowSecs % m_cycleTime)/(float)m_cycleTime;
  // determine direction of travel and convert to triangular wave
  if (wave < 0.5) {
    direction = 1;      // on the way up
    wave = wave*2;
  } else {
    direction = -1;     // on the way down
    wave = (1 - wave)*2;
  }
  // if a dead_time has been supplied for this o/p then adjust power accordingly
  if (m_deadTime > 0  && m_power > 0.0  &&  m_power < 1.0) {
      effectivePower = (1.0-2.0*m_dtoc)*m_power + m_dtoc;
  } else {
      effectivePower = m_power;
  }
  //  cope with end cases in case values outside 0..1
  if (effectivePower <= 0.0) {
      newState = 0;     // no heat
  } else if (effectivePower >= 1.0) {
      newState = 1;     // full heat
  } else {
      // only allow power to come on on the way down and off on the way up, to reduce short pulses
      if (effectivePower >= wave  &&  direction == -1) {
          newState = 1;
      } else if (effectivePower <= wave  &&  direction == 1) {
          newState = 0;
      } else {
          // otherwise leave it as it is
          newState = m_opState;
      }
  }
  if (newState != m_opState) {
    m_opState = newState;
    answer = m_invert ? (1-m_opState) : m_opState;
  }
  // else leave at -1 for no change
  return answer;
}
