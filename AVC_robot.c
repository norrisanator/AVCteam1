#include  <stdio.h>
#include  <time.h>

extern "C" int InitHardware();
extern "C" int Sleep( int sec, int usec );
// motor is the pin it is connected to, dir is direction 1 or 2, speed is 0-255
extern "C" int SetMotor(int motor, int dir, int speed);

int main(){
  // Sets up raspbery pi hardware and ensures everything is working.
  InitHardware();
//test code for camera, takes picture and prints it.
  while(true){
    // reads current image from camera stores in memory.
    int take_picture();
    // returns "whiteness" of each pixel.
    char get_pixel(160,120,3);
    printf("%d\n",get_pixel);
    //repeats every half second.
    Sleep(0.500000);
  }

  
  return 0;
}
