#ifndef Timeprop_h
#define Timeprop_h

class Timeprop {
public:
  /*
    Initialiser given
      cycleTime seconds
      actuator deadTime seconds
      whether to invert the output
      fallback power value if updates are not received within time below
      max number of seconds to allow between updates before falling back to default power
      current time in seconds
  */
  void initialise( int cycleTime, int deadTime, unsigned char invert, float fallbackPower, int maxUpdateInterval,
    unsigned long nowSecs);

  /* set current power required 0:1, given power and current time in seconds */
  void setPower( float power, unsigned long nowSecs ) {
    m_power = power;
    m_lastPowerUpdateTime = nowSecs;
  };

  /* called regularly to provide new output value */
  /* returns new o/p state 0, 1 or -1 to leave as is */
  int tick(unsigned long nowSecs);

private:
  int m_cycleTime;        // cycle time seconds, float to force float calcs
  int m_deadTime;         // actuator action time seconds
  unsigned char m_invert; // whether to invert the output
  float m_dtoc;           // deadTime/m_cycleTime
  int m_opState;          // current output state (before invert)
  float m_power;          // required power 0:1
  float m_fallbackPower;  // falls back to this if updates not received with max allowed timezone
  int m_maxUpdateInterval;  // max time between updates
  unsigned long m_lastPowerUpdateTime;   // the time of last power update secs
};

#endif // Timeprop_h
