/**
 * @brief Laser Ping Driver to determine distance
 * @author Michael Burmeister
 * @date March 30, 2019
 * @version 1.0
 * 
*/

#include <stdio.h>
#include <propeller2.h>
#include "Custom/laserping.h"


#define LASER 36
#define PING 0

int i;

int main()
{

  laserping_start('S', LASER);
  
  while(1)
  {
    i = laserping_distance();
    printf("Distance: %d\n", i);
    wait(500);
  }  
}
