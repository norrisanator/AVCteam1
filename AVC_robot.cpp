#include  <stdio.h>
#include  <time.h>

extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep( int sec, int usec );
// Motor is the pin it is connected to, dir is direction 1 or 2, speed is 0-255
extern "C" int set_motor(int motor, int speed);

int main(){
  // Sets up raspbery pi hardware and ensures everything is working.
    init(0)
	// Test turning motors on for 5 seconds
	set_motor(1, 255);
    set_motor(2, -255);
    Sleep(5,0);
    set_motor(1, 0);
    set_motor(2, 0);
  // Test code for camera, takes picture and prints it.
  char c;
  while(true){
    // Reads current image from camera stores in memory.
    take_picture();
	int sum=0;
	char c;
	for(int i=0; i<320; i++){
		c = get_pixel(120, i, 3);
		sum += i*c;
	}
	
    printf("%d\n",sum);
    // Repeats every half second.
    Sleep(0,500000);
  }

  
  return 0;
}
