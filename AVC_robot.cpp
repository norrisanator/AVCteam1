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
//proportional constant
#define KP 0.0015
//derivative constant
#define KD 0.0005
//integral constant
#define KI 0
#define MAZE_KP 0.05
#define MAZE_KD 0
#define MAZE_KI 0
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

void maze();
//stops motors spinning
void handle_signal(int signal){
    if(signal == SIGINT){
        set_motor(1, 0);
        set_motor(2, 0);
        
        std::exit(0);
    }
}
// https://github.com/kaiwhata/ENGR101-2016/wiki/Network-commands
// more detail on open gate
void openGate(){
    connect_to_server((char *)"130.195.6.196", 1024);
    send_to_server((char *)"Please");
    char message[24];
    receive_from_server(message);
    // null character limits message length at 6 characters long
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
    
    openGate();
    
    // Test code for camera, takes picture and prints it.
    int black_count = 0;
    int counter = 0;
    bool first_full_line = true;
    //maze();
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
        // loop takes a line of pixels 5 wide horizontally accross the picture
        for(int i=0; i<PICTURE_WIDTH; i++){
            for(int j = 0; j<5; j++){
                // c gets assigned 0 or 1
                c = get_pixel(i, (PICTURE_HEIGHT/2)+j, 3) > 127;
                // 0 or 1 added to total_white
                total_white += c;
            }
            // num_white gets the values from just 1 pixel line across the picture.
            c = get_pixel(i, PICTURE_HEIGHT/2, 3) > 127;
            num_white += c;
            // added to the total
            total += (i-(PICTURE_WIDTH/2))*c;
        }
        
        total_error += total;
        // PID control see https://github.com/kaiwhata/ENGR101-2016/wiki/PID-(Proportional-Integral-Derivative)-Control
        // for more detail
        double proportional_signal = total*KP;
        double derivative_signal = (total-prev_error/0.1)*KD;
        double integral_signal =  total_error*KI;
        prev_error = total;
        int total_signal = proportional_signal + derivative_signal + integral_signal;
        //deadend checker
        counter++;
        // if the camera sees nothing but black implements black_count
        if(num_white == 0){
            black_count++;
        }
        // takes a picture of a verticle column in centre of picture
        // currently unused.
        for(int i=0; i<PICTURE_HEIGHT; i++){
            c = get_pixel(PICTURE_WIDTH/2, i, 3) > 127;
            num_col_white += c;
        }
        // detecting if picture is fully white, e.g. only see white
        if (num_white >= 320){
            printf("hit\n");
            if(first_full_line == true){
                // drives straight through
                first_full_line = false;
                set_motor(1, MOTOR_SPEED);
                set_motor(2, -MOTOR_SPEED);
                Sleep(1,0);
            } else {
                //turns left
                set_motor(1, -MOTOR_SPEED - total_signal);
                set_motor(2, -MOTOR_SPEED - total_signal);
            }
        } else if(num_white > 0){
            // drives forward
            set_motor(1, MOTOR_SPEED - total_signal);
            set_motor(2, -MOTOR_SPEED - total_signal);
        } else {
            // turns
            set_motor(1, -MOTOR_SPEED);
            set_motor(2, MOTOR_SPEED);
        }
        // if stuck at dead end for too long spins around
        if(black_count == 25){
            black_count = 0;
            set_motor(1, MOTOR_SPEED);
            //turns avc around
            set_motor(2, MOTOR_SPEED);
            Sleep(1,0);
        }
        //resetes counting how many times it detects dead end.
        if(counter == 60){
            counter = 0;
            black_count =0;
        }

        /*printf("num white:%d\n", num_white);
        printf("black:%d\n", black_count);
        printf("Counter:%d\n", counter);
        printf("Row:%d\n", num_white);
        printf("Col:%d\n", num_col_white);*/
        
        // detecting if at red square for start of maze    
        int num_red = 0;
        for(int i=0; i<PICTURE_WIDTH; i++){
            bool r = get_pixel(i, PICTURE_WIDTH/2, 0) > 200;
            bool g = get_pixel(i, PICTURE_WIDTH/2, 1) > 200;
            bool b = get_pixel(i, PICTURE_WIDTH/2, 2) > 200;
            if(r && !g && !b){
                num_red++;
            }
        }
        //printf("Red:%d\n", num_red);
        if(num_red > 100){
            set_motor(1, 0);
            set_motor(2, 0);
            maze();
        }
           
        Sleep(0,SLEEP_TIME);
    }
    return 0;
}
void maze(){
    // maze code
    int prev_error = 0;
    int total_error = 0;
    while(true){
        int ir_sensor_left = read_analog(2);
        int ir_sensor_right = read_analog(1);
        int ir_sensor_forward = read_analog(0);
        printf("Left:%d\n", ir_sensor_left);
        printf("Right:%d\n", ir_sensor_right);
        printf("Ford:%d\n", ir_sensor_forward);
        int error_signal = ir_sensor_right - ir_sensor_left;
        total_error += error_signal;
        // see PID control https://github.com/kaiwhata/ENGR101-2016/wiki/PID-(Proportional-Integral-Derivative)-Control
        double proportional_signal = error_signal*MAZE_KP;
        double derivative_signal = (error_signal-prev_error/0.1)*MAZE_KD;
        double integral_signal =  total_error*MAZE_KI;
        prev_error = error_signal;

        int total_signal = proportional_signal + derivative_signal + integral_signal;
        // robot will always turn right
        if(ir_sensor_right < 130){
            printf("hi\n");
            set_motor(1, MOTOR_SPEED);
            set_motor(2, -MOTOR_SPEED);
            Sleep(0, 800000);
            set_motor(1, MOTOR_SPEED);
            set_motor(2, 0);
            Sleep(1,500000);
            ir_sensor_right = read_analog(2);
            /*if(ir_sensor_right < 130){
                set_motor(1, MOTOR_SPEED);
                set_motor(2, -MOTOR_SPEED);
                Sleep(0, 500000);
                set_motor(1, MOTOR_SPEED);
                set_motor(2, 0);
                Sleep(1,0);
            }*/
        } else if(ir_sensor_forward > 300){
            // spins robot around
            set_motor(1, -MOTOR_SPEED);
            set_motor(2, -MOTOR_SPEED);
        } else {
            // goes straight forward
            set_motor(1, MOTOR_SPEED - total_signal);
            set_motor(2, -MOTOR_SPEED - total_signal);
        }
        
        Sleep(0,SLEEP_TIME);
    }
    return;
}
