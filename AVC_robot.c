#include  <stdio.h>
#include  <time.h>

extern "C" int InitHardware();
extern "C" int Sleep( int sec, int usec );
// motor is the pin it is connected to, dir is direction 1 or 2, speed is 0-255
extern "C" int SetMotor(int motor, int dir, int speed);

int main()
{
  // Sets up raspbery pi hardware and ensures everything is working.
  InitHardware();
  
  
  return 0;
}
