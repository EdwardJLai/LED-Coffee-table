#include "mbed.h"
#include "WS2811.h"
#include "Colors.h"
#include "rtos.h"

uint8_t const nLEDs = 32;
uint8_t const num_strip = 7;

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
// unsigned const nLEDs = MAX_LEDS_PER_STRIP;
// PTE ports from the bottom left to the top
unsigned const DATA_OUT_PIN1 = 30; // PTE30
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

uint8_t rainbow_matrix[7][nLEDs][3];
WS2811 lightStrips[num_strip] = {lightStrip1, lightStrip2, lightStrip3, lightStrip4, lightStrip5, lightStrip6, lightStrip7};
uint8_t command[4][16];

Mutex command_mutex;

static void showFourContRainbow(WS2811 &strip, uint8_t i, uint8_t stripNum, bool on)
{
    if (stripNum >= num_strip){
        return;
    }
    if (on){
        strip.setPixelColor(i, rainbow_matrix[stripNum][i][0], rainbow_matrix[stripNum][i][1], rainbow_matrix[stripNum][i][2]);
        strip.setPixelColor(i+1, rainbow_matrix[stripNum][i+1][0], rainbow_matrix[stripNum][i+1][1], rainbow_matrix[stripNum][i+1][2]);
        strip.setPixelColor(i+2, rainbow_matrix[stripNum][i+2][0], rainbow_matrix[stripNum][i+2][1], rainbow_matrix[stripNum][i+2][2]);
        strip.setPixelColor(i+3, rainbow_matrix[stripNum][i+3][0], rainbow_matrix[stripNum][i+3][1], rainbow_matrix[stripNum][i+3][2]);
    }else {
        strip.setPixelColor(i, 0, 0, 0);
        strip.setPixelColor(i+1, 0, 0, 0);
        strip.setPixelColor(i+2, 0, 0, 0);
        strip.setPixelColor(i+3, 0, 0, 0);
    }
    strip.show();
}

void lightThread(void const*args){
    for (int i = 0; i < num_strip; i++) {
        lightStrips[i].begin();
    }
    while(true){
        //control lights
        //first copy the original into a new array
        // command_mutex.lock();
        // command_mutex.unlock();
        for (uint8_t j = 0; j < 8; j ++) {
            for (uint8_t i = 0; i < 4; i++) {
                showFourContRainbow(lightStrips[i*2] ,j*4, i*2, command[i][j]);
            }
        }
        for (uint8_t j = 8; j < 16; j ++) {
            for (uint8_t i = 0; i < 4; i++) {
                showFourContRainbow(lightStrips[2*i + 1], (15-j)*4, (i+1) * 2, command[i][j]);
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
    float const frequency = .3;
    for (uint8_t k = 0; k < 7; k++){
        for (uint8_t i = 0; i < 32; ++i)
        {
           rainbow_matrix[k][i][0] = sin(frequency*(i+(k*4)) + 0) * 127 + 128;
           rainbow_matrix[k][i][1] = sin(frequency*(i+(k*4)) + 2) * 127 + 128;
           rainbow_matrix[k][i][2]  = sin(frequency*(i+(k*4)) + 4) * 127 + 128;
        }
    }
    while(1) {
//        command_mutex.lock();
        for (int i = 0; i < 16; i++) {
            
            // pc.printf("%d%d%d%d ,", control0.read(), control1.read(), control2.read(), control3.read());
            // bit shifting didn't work...
            // control3 = (i&8 >> 3);
            // control2 = (i&4 >> 2);
            // control1 = (i&2 >> 1);
            // control0 = (i&1);
            
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

            command[0][i] = (device1.read() + 1) % 2;
            command[1][i] = (device2.read() + 1) % 2;
            command[2][i] = (device3.read() + 1) % 2;
            command[3][i] = (device4.read() + 1) % 2;
            
        }
//        command_mutex.unlock();
    }
}
