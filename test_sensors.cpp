#include  <stdio.h>
#include  <time.h>
//line following methods
extern "C" int init(int d_lev);
//Digital sensors methods;
extern "C" int Sleep( int sec, int usec );
extern "C" int read_analog(int ch_adc);

void maze();

void handle_signal(int signal){
    if(signal == SIGINT){       
        std::exit(0);
    }
}


int main(){
    // Sets up raspbery pi hardware and ensures everything is working.
    init(0);
    std::signal(SIGINT, handle_signal);
    while(true){
        printf("0:%d\n", read_analog(0));
        printf("1:%d\n", read_analog(1));
        printf("2:%d\n", read_analog(2));
        Sleep(0,1000000);
    }
    return 0;
}
