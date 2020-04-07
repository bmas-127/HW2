#include "mbed.h"
//#include "uLCD_4DGL.h"

Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
DigitalIn Switch (SW3);
DigitalOut redLED (LED1);
DigitalOut greenLED (LED2);
//uLCD_4DGL uLCD(D1, D0, D2);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);


int sample = 256;
int i, cnt;
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
float ADCdata[256];


int main(){
    int En, freq;
    float temp;
    int num[20];
/*
    while(1){
    for (int i = 0; i<10; i = i+1){
      display = table[i];
      wait(1);
    }
  }
  */
    En = 1;
    freq = 0;
    for (i = 0; i < sample; i++){
        Aout = Ain;
        ADCdata[i] = Ain;

        if(ADCdata[i] > 0.3 && En == 1){
          freq ++;
          En = 0;
        }

        if(ADCdata[i] < 0.25 && En == 0){
          En = 1;
        }

        wait(1./sample);
    }


    //pc.printf("%d\n", freq);
   // uLCD.printf("%d\n", freq); //Default Green on black text

    for (i = 0; i < sample; i++){
      pc.printf("%1.3f\r\n", ADCdata[i]);
    }

    cnt = 0;
    temp = (float)freq;
    while(freq != 0){
        num[cnt++] = freq % 10;
        freq /= 10;
    }



    temp = 0.0625 / temp;
    while(1){
      redLED = 1;
      greenLED = 0;

      while(Switch == 0){
        redLED = 0;
        greenLED = 1;
        for(i = cnt - 1; i > 0; i --){
            display = table[num[i]];
            wait(1);
        }
        display = table[num[i]] + 128;
        wait(1);
        display = table[10];
      }

      for(float j=0; j<2; j+=0.125 ){
        Aout = 0.5 + 0.5*sin(j*3.1415926);
        wait(temp);
      }
    }

}