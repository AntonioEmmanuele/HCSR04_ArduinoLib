#include "hcsr04.hpp"
namespace hcsr_04{
  /*
   * @brief: Constructor
   * @input: echo_pin, trigger_pin
   */
  hcsr04::hcsr04(unsigned int echo_pin,unsigned int trigger_pin){
    pinMode(echo_pin,INPUT);
    pinMode(trigger_pin,OUTPUT);
    this->echo_pin=echo_pin;
    this->trigger_pin=trigger_pin;
    this->last_distance_cm=0;
    this->last_distance_inch=0;
    this->read_time=0;
  }
  /*
   * @brief: copy constructor
   *      ECHO       Trigger    
   *     Pin 7        Pin 8        
   */
  hcsr04::hcsr04(void){
    hcsr04(7,8);
  }
 /*
  * @brief: Prints the last distance measured from an obstacule and the time where the echo_pin was high (read_time)
  * @in : flg -> With this flg set to 1 the object prints also the value of the other parameters (pins and sound speed)
  * @pre: The Serial Object must be previously initialized (in the setup())
  */
 void hcsr04::hcsr04Print(unsigned int flg){
  Serial.println("[ HCSR04 PRINT]  \n ");
  if(flg){
    Serial.print("    [CURRENT ECHO_PIN ]   :  ");
    Serial.println(this->echo_pin);
    Serial.print("    [CURRENT TRIGGER_PIN ]   :  ");
    Serial.println(this->trigger_pin);
  }
    Serial.print("    [LAST DISTANCE CM]   :  ");
    Serial.println(this->last_distance_cm);
        Serial.print("    [LAST DISTANCE INCH]   :  ");
    Serial.println(this->last_distance_inch);
    Serial.print("    [ READ  TIME VALUE]   :  ");
    Serial.println(this->read_time);
    Serial.println(" \n \n");
  }
  /*
   * @brief:This function sets trigger pin high for 10 microseconds starting the IC Operations.
   */
  void hcsr04::hcsr04Trigger(void){
    digitalWrite(this->trigger_pin,HIGH);
    delayMicroseconds(10);
    digitalWrite(this->trigger_pin,LOW);
  }
  /*
   * @brief: polls echo pin. When echo pin becomes high it calculates the value of time of his high value and then performs converions.
   * @brief: If echo doesn't became high then return error ECHO_TIME_ELAPSE error
   * @return : false if there is a problem.
   */
  int  hcsr04::hcsr04EchoPoll(void){
      unsigned long time=micros();
      while(!digitalRead(this->echo_pin)){
        if((micros()-time)>=MAX_ECHO_MICROSECONDS_WAIT)
          return -ERR_ECHOPOLL_TIME_ELAPSE;
      }
      this->read_time=micros();
      while(digitalRead(this->echo_pin))
        time=micros();
      this->read_time=time-this->read_time;
      this->last_distance_inch=this->read_time/TIME_TO_INCH;
      this->last_distance_cm=this->read_time/TIME_TO_CM;
      if(hcsr04CheckOOR())
        return ECHOPOLL_OOR;
      else
        return ECHOPOLL_INRANGE;
   }
  /*
   * @brief : obtain the obstacule distance in cm and inches.
   * @in: flg , use GET_CM to return the value in cm, else the fun will return it in inches
   * @out: obstacule (first parameter passed by reference).
   */
 int hcsr04::hcsr04GetDistance(float& val,unsigned int flg){
  switch(flg){
     case GET_CM:
      val = this->last_distance_cm;
      break;
     case GET_INCHES:
      val = this->last_distance_inch;
      break;
      default:
        return -ERR_GET_INVALID_FLG;
    }
  }
  /*
   * @brief : returns the last value of read_val
   * @ret : read_time(The time of echo pin being high).
   */
  unsigned long hcsr04::hcsr04GetMicros(void){
    return this->read_time;
  }
  /*
   * @brief:Check if the IC doesn't find obstacules.
   * @ret: true if obstacule free.
   */
  bool hcsr04::hcsr04CheckOOR(void){
    return this->read_time>MAX_PULSE_TIME;
  }
  /*
   * @brief:delays code execution for the next measurement
   */
  void hcsr04::hcsr04DelayNextMeasurement(void){
    delay(NEXT_MEASUREMENT_DELAY);
  }
  /*
   * @brief: all in  one measurement.
   * @in:function flag(To delay..)
   * @ret : Same ad echoPoll
   */
  int hcsr04::hcsr04AllInOne(unsigned int flg){
    int to_ret;
    hcsr04Trigger();
    to_ret=hcsr04EchoPoll();
    if(flg==ALLINONE_TODELAY)
      hcsr04DelayNextMeasurement();
    return to_ret;
   }
   /*
    * @brief : Give information about the error received as input.
    */
   void hcsr04PrintErr(unsigned int error){
      switch(error){
          case ERR_ECHOPOLL_TIME_ELAPSE:
            Serial.println("[ERROR] Echo pin never becomes high ");
            break;
          case ERR_GET_INVALID_FLG:
            Serial.println("[ERROR] Invalid flag passed to getDistance, use GET_CM or GET_INCHES ");
            break;
          default:
            Serial.println("[ERROR ] Invalid error code ");
            break;
      }
   }
}
