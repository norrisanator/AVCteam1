#include  <stdio.h>
#include  <time.h>
#include <csignal>
#include <cstdlib>
// constants
#define MESSAGE_LENGTH 6
#define MOTOR_SPEED 50
#define SLEEP_TIME 100000
#define PICTURE_WIDTH 320
#define PICTURE_HEIGHT 240
#define THRESHOLD 500000
#define KP 0.0015
#define KD 0.0005
#define KI 0
//line following methods
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep( int sec, int usec );
// Motor is the pin it is connected to, dir is direction 1 or 2, speed is 0-255
extern "C" int set_motor(int motor, int speed);
//Server methods
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);
//Digital sensors methods
extern "C" int init(int d_lev);
extern "C" int read_analog(int ch_adc);
//extern "C" int select_IO(int chan, int direct);

void handle_signal(int signal){
    if(signal == SIGINT){
        set_motor(1, 0);
        set_motor(2, 0);
        
        std::exit(0);
    }
}

void openGate(){
    connect_to_server("130.195.6.196", 1024);
    send_to_server("Please");
    char message[24];
    receive_from_server(message);
    message[MESSAGE_LENGTH] = '\0';
    send_to_server(message);
    printf("%s", message);
}

void testSensors(){
    //int select_IO(0, 1);
    int analog_sensor_reading = read_analog(0);
    printf("%d\n",analog_sensor_reading);
    Sleep(0,500000);
}


int main(){
    // Sets up raspbery pi hardware and ensures everything is working.
    init(0);
    
    std::signal(SIGINT, handle_signal);
    
    //openGate();
    
    // Test code for camera, takes picture and prints it.
    int black_count = 0;
    int counter = 0;
    while(true){
        // Reads current image from camera stores in memory.
        take_picture();
        int total=0;
        int total_error = 0;
        int num_white = 0;
        int num_col_white = 0;
        int total_white = 0;
        int prev_error = 0;
        bool c;
        for(int i=0; i<PICTURE_WIDTH; i++){
            for(int j = 0; j<5; j++){
                c = get_pixel(i, (PICTURE_HEIGHT/2)+j, 3) > 127;
                total_white += c;
            }
            c = get_pixel(i, PICTURE_HEIGHT/2, 3) > 127;
            num_white += c;
            //printf("%d\n", c);
            total += (i-(PICTURE_WIDTH/2))*c;
        }
        
        total_error += total;

        double proportional_signal = total*KP;
        double derivative_signal = (total-prev_error/0.1)*KD;
        double integral_signal =  total_error*KI;
        prev_error = total;

        int total_signal = proportional_signal + derivative_signal + integral_signal;
        //deadend checker
        counter++;
        if(num_white == 0){
            black_count++;
        }
        
        for(int i=0; i<PICTURE_HEIGHT; i++){
            c = get_pixel(PICTURE_WIDTH/2, i, 3) > 127;
            num_col_white += c;
        }
        /*
        
        forloop(read 3-5 rows of centre of image){
            similiar to what we have currently
        }
        forloop(read 3-5 columns centre of image){
            will return stuff if line is directly ahead
        }
        if(camera is all white){
            turn left
        }
        else if(camera has white in front and black on sides){
            go forward
        }
        else if(check what side line is on ){
            turn to that side
        }
        else if ( check if dead end)
        use deadend code.
        */
        if (num_white >= 320){
            set_motor(1, -MOTOR_SPEED - total_signal);
            set_motor(2, -MOTOR_SPEED - total_signal);
        } else if(num_white > 0){
            set_motor(1, MOTOR_SPEED - total_signal);
            set_motor(2, -MOTOR_SPEED - total_signal);
        } else {
            set_motor(1, -MOTOR_SPEED);
            set_motor(2, MOTOR_SPEED);
        }
        if(black_count == 25){
            black_count = 0;
            set_motor(1, MOTOR_SPEED);
            //turns avc slightly one way(probably)
            set_motor(2, MOTOR_SPEED);
            Sleep(2,0);
        }
        if(counter == 60){
            counter = 0;
            black_count =0;
        }
        /*
        if((total_white/5) == 300){
            set_motor(1, MOTOR_SPEED);
            set_motor(2, MOTOR_SPEED);
            Sleep(0,500000);
        }
        */
            //printf("tot:%d\nprop:%f\n",total, proportional_signal);
            printf("num white:%d\n", num_white);
            printf("black:%d\n", black_count);
            printf("Counter:%d\n", counter);
            printf("Row:%d\n", num_white);
            printf("Col:%d\n", num_col_white);
            
        int num_red = 0;
        for(int i=0; i<PICTURE_WIDTH; i++){
            bool r = get_pixel(i, PICTURE_WIDTH/2, 0) > 200;
            bool g = get_pixel(i, PICTURE_WIDTH/2, 1) > 200;
            bool b = get_pixel(i, PICTURE_WIDTH/2, 2) > 200;
            if(r && !g && !b){
                num_red++;
            }
	    }
        printf("Red:%d\n", num_red);
        if(num_red > 100){
            set_motor(1, 0);
            set_motor(2, 0);
            return 0;
        }
           
            Sleep(0,SLEEP_TIME);
        }

    

    return 0;
}
