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
#include "hcsr04.hpp"
#define SINGLE_TST 0
#define ALL_IN_ONE_TST 1
#define PRINT_STATUS 0
#define PRINT_SINGLE 1
using namespace hcsr_04;
static float distance_cm=0;
static float distance_inches=0;
static unsigned long time_echo=0;
static int error_get=0;
static int error_poll=0;
/*7 is echo and 8 is trigger*/
static hcsr04 my_ultra(7,8);
void setup() {
  Serial.begin(9600);
}

void loop() {
#if SINGLE_TST && !(ALL_IN_ONE_TST)
  my_ultra.hcsr04Trigger();
  error_poll=my_ultra.hcsr04EchoPoll();
  if(HCSR04_IS_ERR(error_poll))
      //my_ultra.hcsr04PrintErr(HCSR04_TO_DIAGN(error_get));
  my_ultra.hcsr04DelayNextMeasurement();
#elif ALL_IN_ONE_TST && !SINGLE_TST
  error_poll=my_ultra.hcsr04AllInOne(ALLINONE_TODELAY);
  if(HCSR04_IS_ERR(error_poll))
   my_ultra.hcsr04PrintErr(HCSR04_TO_DIAGN(error_poll));
#endif

#if !PRINT_STATUS && PRINT_SINGLE
if(my_ultra.hcsr04CheckOOR()){
  Serial.println("[ ERROR ]Out of range \n");
}
else{
  time_echo=my_ultra.hcsr04GetMicros();
  error_get=my_ultra.hcsr04GetDistance(distance_cm,GET_CM);
  if(HCSR04_IS_ERR(error_get))
    my_ultra.hcsr04PrintErr(HCSR04_TO_DIAGN(error_get));
  error_get=my_ultra.hcsr04GetDistance(distance_inches,GET_INCHES);
  if(HCSR04_IS_ERR(error_get))
    my_ultra.hcsr04PrintErr(HCSR04_TO_DIAGN(error_get));
    Serial.println("Distance in cm and inches and time: ");
    Serial.print("CM:  ");
    Serial.println(distance_cm);
    Serial.print("Inches:  ");
    Serial.println(distance_cm);
    Serial.print("Time:  ");
    Serial.println(time_echo);
    Serial.println("****************************");
}
#elif PRINT_STATUS && !PRINT_SINGLE
  my_ultra.Print(1);
#endif
}
