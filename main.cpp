#include "mbed.h"

// This defines the normal sequence of the leds (green=1, blue=2, red=3)
const int M=3;
int led_cycle[M] = {1,2,3};

// This will record the sequence that will be defined when the button is pressed. In this case, it is 6, but can be changed
const int N = 6;
int sequence[N];

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

InterruptIn button(USER_BUTTON);

// t is to change the led turned on, numb_of_presses tells us how many times the button has been pressed
int t=0;
int numb_of_presses=0;

// Defines the timeout and time interval
Timeout button_debounce_timeout;
float debounce_time_interval = 0.3;

void onButtonStopDebouncing(void);

// Interrupt - when button is pressed, appends the led turned on to array sequence, and turns all leds on
void onButtonPress(void) {
        if (led_cycle[t]==1) {
            sequence[numb_of_presses] = 1;
        }
        else if (led_cycle[t]==2) {
            sequence[numb_of_presses] = 2;
        }  
        else if (led_cycle[t]==3) {
            sequence[numb_of_presses] = 3;
        }

        led1=true;
        led2=true;
        led3=true;

        numb_of_presses = numb_of_presses + 1;

        button.rise(NULL);
        button_debounce_timeout.attach(onButtonStopDebouncing, debounce_time_interval);
}

void onButtonStopDebouncing(void) {
        button.rise(onButtonPress);
}

// Turns on the desired led based on the sequence and the value of t
void select_led(int l) {
        if (l==1) {
                led1 = true;
                led2 = false;
                led3 = false;
        }
        else if (l==2) {
                led1 = false;
                led2 = true;
                led3 = false;
        }
        else if (l==3) {
                led1 = false;
                led2 = false;
                led3 = true;
        }
}

int main() {
    button.rise(onButtonPress);

    while (numb_of_presses < N){
        select_led(led_cycle[t]);
        wait(1.0);
        t=(t+1)%M;
    }

    while (numb_of_presses >= N) {    
        select_led(sequence[t]);
        wait(0.5);
        t=(t+1)%N;
    }
}