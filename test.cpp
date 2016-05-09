#include  <stdio.h>
#include  <time.h>
extern "C" int init(int d_lev);
extern "C" int Sleep( int sec, int usec );
// Motor is the pin it is connected to, dir is direction 1 or 2, speed is 0-255
extern "C" int set_motor(int motor, int speed);


int main(){
  // Sets up raspbery pi hardware and ensures everything is working.
    init(0);
    
    // Test turning motors on
    set_motor(1, 255);
	Sleep(2,0);
    set_motor(1, 0);
    Sleep(2,0);
    set_motor(2, -255);
	Sleep(2,0);
    set_motor(2, 0);
    return 0;
}
