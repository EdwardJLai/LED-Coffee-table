#include "mbed.h"
#include "WS2811.h"
#include "Colors.h"
#include "rtos.h"

DigitalIn device1(D12); // mux 1
DigitalIn device2(D13); // mux 2
DigitalIn device3(D14); // mux 3
DigitalIn device4(D15); // mux 4
DigitalOut myled1(LED1, LED2);
DigitalOut control0(A0); //bit 0
DigitalOut control1(A1); //bit 1
DigitalOut control2(A2); //bit 2
DigitalOut control3(A3); //bit 3
PwmOut IRLED(D2);
PwmOut IRLED2(D3);
//IR send and receive demo
//LED1 and LED2 indicate TX/RX activity
unsigned const nLEDs = MAX_LEDS_PER_STRIP;
// PTC ports from the bottom right to the top
unsigned const DATA_OUT_PIN1 = 30; // PTC7
unsigned const DATA_OUT_PIN2 = 29;
unsigned const DATA_OUT_PIN3 = 23;
unsigned const DATA_OUT_PIN4 = 22;
unsigned const DATA_OUT_PIN5 = 21;
unsigned const DATA_OUT_PIN6 = 20;
unsigned const DATA_OUT_PIN7 = 5;
WS2811 lightStrip1(nLEDs, DATA_OUT_PIN1);
WS2811 lightStrip2(nLEDs, DATA_OUT_PIN2);
WS2811 lightStrip3(nLEDs, DATA_OUT_PIN3);
WS2811 lightStrip4(nLEDs, DATA_OUT_PIN4);
WS2811 lightStrip5(nLEDs, DATA_OUT_PIN5);
WS2811 lightStrip6(nLEDs, DATA_OUT_PIN6);
WS2811 lightStrip7(nLEDs, DATA_OUT_PIN7);

float touchPercentage;
unsigned frames;

float brite;

int command1[16];
int command2[16];
int command3[16];
int command4[16];
int light_command[4][16];

Mutex command_mutex;

static void showSolidColor(WS2811 &strip, uint8_t r, uint8_t g, uint8_t b, unsigned i)
{
    unsigned nLEDs = strip.numPixels();
    strip.setPixelColor(i, r, g, b);
    strip.show();
}

void lightThread(void const*args){
    lightStrip1.begin();
    lightStrip2.begin();
    lightStrip3.begin();
    lightStrip4.begin();
    lightStrip5.begin();
    lightStrip6.begin();
    lightStrip7.begin();
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 16; j++){
            light_command[i][j] = false;
        }
    }
    while(true){
        //control lights
        //first copy the original into a new array
        command_mutex.lock();
        for (int i = 0; i < 16; i++) {
            light_command[0][i] = command1[i];
            light_command[1][i] = command2[i];
            light_command[2][i] = command3[i];
            light_command[3][i] = command4[i];
        }
        command_mutex.unlock();
        for (int j = 0; j < 8; j++) {
            //top rows
            if (light_command[0][j]) {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip1, 100, 0, 0, j*4 + i);
                }
            } else {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip1, 100, 0, 0, j*4 + i);
                }
            }
            if (light_command[1][j]) {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip3, 100, 0, 0, j*4 + i);
                }
            } else {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip3, 100, 0, 0, j*4 + i);
                }
            }
            if (light_command[2][j]) {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip5, 100, 0, 0, j*4 + i);
                }
            } else {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip5, 100, 0, 0, j*4 + i);
                }
            }
            if (light_command[3][j]) {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip7, 100, 0, 0, j*4 + i);
                }
            } else {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip7, 100, 0, 0, j*4 + i);
                }
            }
        }
        for (int j = 8; j < 16; j++) {
            //bottom rows
            if (light_command[0][j]) {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip2, 100, 0, 0, j*4 + i);
                }
            } else {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip2, 100, 0, 0, j*4 + i);
                }
            }
            if (light_command[1][j]) {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip4, 100, 0, 0, j*4 + i);
                }
            } else {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip4, 100, 0, 0, j*4 + i);
                }
            }
            if (light_command[2][j]) {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip6, 100, 0, 0, j*4 + i);
                }
            } else {
                for (int i = 0; i < 4; i++){
                    showSolidColor(lightStrip6, 100, 0, 0, j*4 + i);
                }
            }
        }
        WS2811::startDMA();
    }
}

int main() {
    //IR Transmit code
    IRLED.period(1.0/38000.0);
    IRLED = 0.5;
    IRLED2.period(1.0/38000.0);
    IRLED2 = 0.5;
    //Drive IR LED data pin with 38Khz PWM Carrier
    Thread lights(lightThread);
    // lightStrip1.begin();
    // lightStrip2.begin();
    // lightStrip3.begin();
    // lightStrip4.begin();
    // lightStrip5.begin();
    // lightStrip6.begin();
    // lightStrip7.begin();
    // lightStrip8.begin();
    while(1) {
        command_mutex.lock();
        for (int i = 0; i < 16; i++) {
            /*
            pc.printf("%d%d%d%d ,", control0.read(), control1.read(), control2.read(), control3.read());
//            control3 = (i&8 >> 3);
//            control2 = (i&4 >> 2);
//            control1 = (i&2 >> 1);
//            control0 = (i&1);
            */
            if (i == 0){
                control3 = 0;
                control2 = 0;
                control1 = 0;
                control0 = 0;
            }else if(i == 1){
                control3 = 0;
                control2 = 0;
                control1 = 0;
                control0 = 1;
            }else if(i == 2){
                control3 = 0;
                control2 = 0;
                control1 = 1;
                control0 = 0;
            }else if(i == 3){
                control3 = 0;
                control2 = 0;
                control1 = 1;
                control0 = 1;
            }else if(i == 4){
                control3 = 0;
                control2 = 1;
                control1 = 0;
                control0 = 0;
            }else if(i == 5){
                control3 = 0;
                control2 = 1;
                control1 = 0;
                control0 = 1;
            }else if(i == 6){
                control3 = 0;
                control2 = 1;
                control1 = 1;
                control0 = 0;
            }else if(i == 7){
                control3 = 0;
                control2 = 1;
                control1 = 1;
                control0 = 1;
            }else if(i == 8){
                control3 = 1;
                control2 = 0;
                control1 = 0;
                control0 = 0;
            }else if(i == 9){
                control3 = 1;
                control2 = 0;
                control1 = 0;
                control0 = 1;
            }else if(i == 10){
                control3 = 1;
                control2 = 0;
                control1 = 1;
                control0 = 0;
            }else if(i == 11){
                control3 = 1;
                control2 = 0;
                control1 = 1;
                control0 = 1;
            }else if(i == 12){
                control3 = 1;
                control2 = 1;
                control1 = 0;
                control0 = 0;
            }else if(i == 13){
                control3 = 1;
                control2 = 1;
                control1 = 0;
                control0 = 1;
            }else if(i == 14){
                control3 = 1;
                control2 = 1;
                control1 = 1;
                control0 = 0;
            }else if(i == 15){
                control3 = 1;
                control2 = 1;
                control1 = 1;
                control0 = 1;
            }
            wait(0.001);

            command1[i] = ((device1.read() == 0) ? 1 : 0);
            command2[i] = ((device2.read() == 0) ? 1 : 0);
            command3[i] = ((device3.read() == 0) ? 1 : 0);
            command4[i] = ((device4.read() == 0) ? 1 : 0);
            
        }
        command_mutex.unlock();
        Thread::wait(100);
        /*
        for(int i = 1; i < 2; i++){
            control3 = 0;
            control2 = 0;
            control1 = 0;
            control0 = 1;
            if(pc.readable()) {
                myled1 = 1;
                device.putc(pc.getc());
                myled1 = 0;
            }
            if(device.readable()) {
                wait(0.3);
                myled1 = 0;
                wait(0.2);
                myled1 = 1;
                command1[i] = !command1[i];
                if (command[i]) {
                //do stuff
                    showSolidColor(lightStrip1, 100, 0, 0, i);
                    WS2811::startDMA();
                } else{
                    showSolidColor(lightStrip1, 0, 0, 0, i);
                    WS2811::startDMA();
                }
                pc.putc(device.getc());
            }
        }
        */
        /*
        for (int j = 0; j < 16; j++) {
            if (command[j]) {
                //do stuff
                showSolidColor(lightStrip1, 100, 0, 0, j);
            } else{
                showSolidColor(lightStrip1, 0, 0, 0, j);
            }
        }
        WS2811::startDMA();
        */
    }
}
