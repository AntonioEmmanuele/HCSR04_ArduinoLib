/*
    HCSR04_ArduinoLib - Copyright (C) 2021 Antonio Emmanuele
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#ifndef HCSR04_HPP
#define HCSR04_HPP
#include "Arduino.h"
namespace hcsr_04{
/*Various flags*/
/* Value for performing conversion of the echo high time in cm*/
#define TIME_TO_CM 58
/* Value for performing conversion of the echo high time in inches*/
#define TIME_TO_INCH 148
/*Max time for pulsing, if read_time is above this value then there aren't obstacules*/
#define MAX_PULSE_TIME 23200 
/* Max time in microseconds to wait for echo pin becoming high,100 ms should be enought...*/
#define MAX_ECHO_MICROSECONDS_WAIT 100000
/* From one measurement to another we need to wait 60 ms*/
#define NEXT_MEASUREMENT_DELAY 60

/*Echo poll return values*/
/* Error for echo never becoming high*/
#define ERR_ECHOPOLL_TIME_ELAPSE 1 
/*Echo Poll return this value if the Obstacule(or the object  we want to find dist) is  out of range*/
#define ECHOPOLL_OOR 2
/* Basically the oppose of ECHOPOLL_OOR*/
#define ECHOPOLL_INRANGE 3

/*Flags for get Function*/
/*using this flg the function will return the value in cm*/
#define GET_CM 4
#define GET_INCHES 3
#define ERR_GET_INVALID_FLG 2

/*Flags for All in One function*/
#define ALLINONE_TODELAY 1


/* Macro used for checking errors in return types*/
#define HCSR04_IS_ERR(value) value<0              

/*Macro used to pass error to the diagnose function*/
#define HCSR04_TO_DIAGN(v) (-1)*v             

class hcsr04{
  private:
  /* The pin used to read the time of the coming back wave*/
    unsigned int echo_pin;
  /* The pin used to trigger ic measurement*/
    unsigned int trigger_pin;
  /*last distance calculated in inches and in cm*/
    float last_distance_inch;
    float last_distance_cm;
  /* Time of the wave*/
    unsigned long read_time;
  public:
  
    hcsr04(unsigned int,unsigned int);
    hcsr04(void);
    void hcsr04Print(unsigned int);
    void hcsr04Trigger(void);
    int hcsr04EchoPoll(void);
    int hcsr04GetDistance(float&,unsigned int);
    unsigned long hcsr04GetMicros(void);
    bool hcsr04CheckOOR(void);
    void hcsr04DelayNextMeasurement(void);
    int hcsr04AllInOne(unsigned int);
    void hcsr04PrintErr(unsigned int);
};
}
#endif
