/*
 * File:   main.c
 * Author: fredericovaz & joaofreitas
 *
 * Projecto - DAC de 8 bits, gera uma onda de 10Hz com Vp = a 2V com 256 Samples. 
 *
 * Formula -> Vadc = (Vdacref*DAC1DAT)/256 
 *
 * Através da formula construímos um array de valores, no matlab, correspondentes a onda que queremos 
 * O Tempo entre cada amostra/sample definido pelo Timer1 define a Freq do Sinal
 *
 * Neste caso como queremos 256 samples a 10Hz = 0.1s fazemos:
 *
 * 10Hz / 256 -> 0,1s / 256 = 391us 
 * 391us = 16us * PR1
 * PR1 = 391us / 16us
 * 
 * Não esquecer que terá um offeset de 2.5V! (Podem tira-lo com o filtro passa alto)
 *
 * Created on November 15, 2015, 3:27 PM
 */

#include "xc.h"
#include "config.h"
#include <stdio.h>
#include <libpic30.h>
#include <math.h>



#define BAUDRATE 19200U
#define BRG 51

#define Npts 256


// Var global

// 32 Samples
// const unsigned int Sinewave[] = { 128,152,176,198,218,234,245,253,255,253,245,234,218,198,176,152,128,103,79,57,37,21,10,2,0,2,10,21,37,57,79,103}; // 32 Samples

// 64 Samples 1 V
// const unsigned int Sinewave[] = { 128, 133, 138, 143, 148, 152, 156, 160, 164, 168, 171, 173, 175, 177, 178, 179, 179, 179, 178, 177, 175, 173, 171, 168, 164, 160, 156, 152, 148, 143, 138, 133, 128, 123, 118, 113, 108, 104, 100, 96,  92,  88,  85,  83,  81,  79,  78,  77,  77,  77,  78,  79,  81,  83,  85,  88,  92,  96,  100, 104, 108, 113, 118, 123 }; // 64 Samples

// 128 Samples 1 V
// const unsigned int Sinewave[] = { 128,  131, 133, 136, 138, 140, 143, 145, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 169, 171, 172, 173, 174, 175, 176, 177, 178, 178, 179, 179, 179, 179, 179, 179, 179, 178, 178, 177, 176, 175, 174, 173, 172, 171, 169, 168, 166, 164, 162, 160, 158, 156, 154, 152, 150, 148, 145, 143, 140, 138, 136, 133, 131, 128, 125, 123, 120, 118, 116, 113, 111, 108, 106, 104, 102, 100, 98,  96,  94,  92,  90,  88,  87,  85,  84,  83,  82,  81,  80,  79,  78,  78,  77,  77,  77,  77,  77,  77,  77,  78,  78,  79,  80,  81,  82,  83,  84,  85,  87,  88,  90,  92,  94,  96,  98,  100, 102, 104, 106, 108, 111, 113, 116, 118, 120, 123, 125 }; // 128 Samples

// 256 Samples 1 V
// const unsigned int Sinewave[] = { 128, 129, 131, 132, 133, 134, 136, 137, 138, 139, 140, 142, 143, 144, 145, 146, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 168, 169, 170, 171, 171, 172, 173, 173, 174, 174, 175, 175, 176, 176, 177, 177, 177, 178, 178, 178, 178, 179, 179, 179, 179, 179, 179, 179, 179, 179, 179, 179, 179, 179, 178, 178, 178, 178, 177, 177, 177, 176, 176, 175, 175, 174, 174, 173, 173, 172, 171, 171, 170, 169, 168, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 146, 145, 144, 143, 142, 140, 139, 138, 137, 136, 134, 133, 132, 131, 129, 128, 127, 125, 124, 123, 122, 120, 119, 118, 117, 116, 114, 113, 112, 111, 110, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99,  98,  97,  96,  95,  94,  93,  92,  91,  90,  89,  88,  88,  87,  86,  85,  85,  84,  83,  83,  82,  82,  81,  81,  80,  80,  79,  79,  79,  78,  78,  78,  78,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  78,  78,  78,  78,  79,  79,  79,  80,  80,  81,  81,  82,  82,  83,  83,  84,  85,  85,  86,  87,  88,  88,  89,  90,  91,  92,  93,  94, 95,  96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 110, 111, 112, 113, 114, 116, 117, 118, 119, 120, 122, 123, 124, 125, 127 };

// 256 Samples 2 V
const unsigned int Sinewave[] = { 128, 131, 133, 136, 138, 141, 143, 146, 148, 150, 153, 155, 158, 160, 162, 165, 167, 169, 172, 174, 176, 178, 181, 183, 185, 187, 189, 191, 193, 195, 197, 199, 200, 202, 204, 206, 207, 209, 210, 212, 213, 215, 216, 217, 218, 219, 221, 222, 223, 224, 224, 225, 226, 227, 227, 228, 228, 229, 229, 230, 230, 230, 230, 230, 230, 230 , 230, 230, 230, 230, 229, 229, 228, 228, 227, 227, 226, 225, 224, 224, 223, 222, 221, 219, 218, 217, 216, 215, 213, 212, 210, 209, 207, 206, 204, 202, 200, 199, 197, 195, 193, 191, 189, 187, 185, 183, 181, 178, 176, 174, 172, 169, 167, 165, 162, 160, 158, 155, 153, 150, 148, 146, 143, 141, 138, 136, 133, 131, 128, 125, 123, 120, 118, 115, 113, 110, 108, 106, 103, 101, 98, 96,  94,  91,  89,  87,  84,  82,  80,  78,  75,  73,  71,  69,  67,  65,  63,  61,  59,  57,  56,  54,  52,  50,  49,  47,  46,  44,  43,  41,  40,  39,  38,  37,  35,  34,  33,  32,  32,  31,  30,  29,  29,  28,  28,  27, 27,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  27,  27,  28,  28,  29,  29,  30,  31,  32,  32,  33,  34,  35,  37,  38,  39,  40,  41,  43,  44,  46,  47,  49,  50,  52,  54,  56,  57,  59,  61,  63,  65,  67,  69,  71,  73,  75,  78,  80, 82,  84,  87,  89,  91,  94,  96,  98,  101, 103, 106, 108, 110, 113, 115, 118, 120, 123, 125 };



// Configura o oscilador p/ 32Mhz
inline void CLKconfig(void) {

  CLKDIVbits.DOZE = 0;                  // 1:1
  CLKDIVbits.RCDIV = 0;                 // 8 MHz
}
// Configura os Pinos
inline void IOconfig(void) {

  ANSA = 0;                             // Porto A digital
  TRISAbits.TRISA0 = 0;                 // saida (LED Microstick)

  ANSB = 0x1000;                        // Porto B c/ RB12 Analogico
  TRISBbits.TRISB12 = 0;                // Saída Analogica do DAC

}
// Configuracao do Timer1
inline void ConfigTMR1(void) { 
    
  T1CON = 0x8030;                       // Enabled, prescaler 1:256, 16 us tck @ FCY=16MHz
  TMR1 = 0;
  PR1 = 24-1;                          // 1ms
  _T1IF = 0;
  _T1IE = 1;                            // Activa interrupcoes
}
// Configuração do DAC
inline void DACconfig (){

  DAC1CON = 0;                          // Reset ao DAC
  
  DAC1CONbits.DACOE=1;                  // DAC1 output pin is enabled (RB12/PIN23)
  DAC1CONbits.DACREF=0b10;              // DACx Reference Source AVDD
  DAC1CONbits.DACEN=1;                  // Enable DAC

}
// Interrupção do Timer1 
void _ISRFAST _T1Interrupt(void) {

  static unsigned char t = 0;

  DAC1DAT = Sinewave[t++];              
  t &= Npts-1; // 0 a 31

  _T1IF = 0; // clear the interrupt flag
}
int main(void) {

  CLKconfig();
  IOconfig();
  ConfigTMR1();
  DACconfig();


	do{

    // Nothing
                       
    

	}while(1);

  return 0;
}















