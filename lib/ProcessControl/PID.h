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
 **/

 /**
  * A PID control class
  *
  * Github repository https://github.com/colinl/process-control.git
  *
  * Given ...
  *
  * Usage:
  * First call initialise(), see below for parameters then
  * ...
  * The functions require a parameter nowSecs which is a representation of the
  * current time in seconds. The absolute value of this is immaterial, it is
  * used for relative timing only.
  *
 **/


#ifndef PID_h
#define PID_h

class PID {
public:
  /*
    Initialiser given

      current time in seconds
  */
  void initialise( 
    unsigned long nowSecs);


  /* called regularly to provide new output value */
  /* returns ...*/
  int tick(unsigned long nowSecs);

private:

  int m_maxUpdateInterval;  // max time between updates
  unsigned long m_lastPowerUpdateTime;   // the time of last power update secs
};

#endif // Timeprop_h
