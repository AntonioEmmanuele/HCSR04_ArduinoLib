#include "hcsr04.hpp"
#define SINGLE_TST 1
#define ALL_IN_ONE_TST 0
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
  my_ultra.hcsr04EchoPoll();
#elif ALL_IN_ONE_TST && !SINGLE_TST
  error_poll=my_ultra.hcsr04AllInOne();
  if(HCSR04_IS_ERR(error_poll))
    my_ultra.hcsr04PrintErr(HCSR04_TO_DIAGN(error_poll));
#endif

#if !PRINT_STATUS && PRINT_SINGLE
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
#elif PRINT_STATUS && !PRINT_SINGLE
  my_ultra.Print(1);
#endif
  
  
  
}
