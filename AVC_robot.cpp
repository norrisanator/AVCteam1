#include  <stdio.h>
#include  <time.h>
// constants
const int PICTURE_WIDTH = 320;
const int PICTURE_HEIGHT = 240;
const int THRESHOLD = 500000;
const double KP = 0.5;
const double KD = 0;
const double KI = 0;

extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep( int sec, int usec );
// Motor is the pin it is connected to, dir is direction 1 or 2, speed is 0-255
extern "C" int set_motor(int motor, int speed);

extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

void openGate(){
    connect_to_server("130.195.6.196", 1024);
    send_to_server("Hello server");
    char message[24];
    receive_from_server(message);
    printf("%s", message);
}


int main(){
    // Sets up raspbery pi hardware and ensures everything is working.
    init(0);
    
    //openGate();
    
    // Test code for camera, takes picture and prints it.
    while(true){
        // Reads current image from camera stores in memory.
        take_picture();
    	int total=0;
		int prev_error = 0;
		int total_error = 0;
    	bool c;
    	for(int i=0; i<PICTURE_WIDTH; i++){
            c = get_pixel(i, PICTURE_HEIGHT/2, 3) > 127;
            //printf("%d\n", c);
            total += (i-(PICTURE_WIDTH/2))*c;
	    }
		
		total_error += total;

        double proportional_signal = total*KP;
		double derivative_signal = (total-prev_error)*KD;
		double integral_signal =  total_error*KI;
		prev_error = total;
		
		double total_signal = proportional_signal + derivative_signal + integral_signal;
		
		set_motor(1, 100 + (total_signal/(160*1*KP))*255);
		set_motor(2, 100 + (total_signal/(160*1*KP))*255);
	
        printf("tot:%d\nprop:%d\n",totalm proportional_signal);
        // Repeats every half second.
        Sleep(0,100000);
    }
    return 0;
}
