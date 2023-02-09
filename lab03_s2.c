/*
 ********************************************************************************
Universidad del Valle de Guatemala
 * curso: Electronica Digital 2
Autor: Gabriel Andrade
compilador: XC8
proyecto: laboratorio 03
hardware: PIC 16F887
creado: 03/01/2023
�ltima modificaci�n: 03/02/2023
 ********************************************************************************
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuraci�n
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definici�n e importaci�n de librer�as
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "adc.h"
//*****************************************************************************
// Definici�n de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000

uint8_t temporal = 0;
//*****************************************************************************
// Definici�n de funciones para que se puedan colocar despu�s del main de lo
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// C�digo de Interrupci�n
//*****************************************************************************
void __interrupt() isr(void){
   if(SSPIF == 1){
       temporal = spiRead();
       if (temporal == 1){
        spiWrite(PORTB);
        SSPIF = 0;}
    }
}
//*****************************************************************************
// C�digo Principal
//*****************************************************************************
void main(void) {
    ADC_Init(AN0);
    setup();

    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1)
    {
        PORTB = ADC_Read(0);
        __delay_ms(250);
    }
    return;
}
//*****************************************************************************
// Funci�n de Inicializaci�n
//*****************************************************************************
void setup(void){
      // oscilador a 4MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;

     //Seleccion de Oscilador interno
    OSCCONbits.SCS = 1;
    ANSEL = 0;
    ANSELH = 0;
    ANSELbits.ANS0 = 1;

    TRISA0 = 1;

    TRISB = 0;
    TRISD = 0;

    PORTB = 0;
    PORTD = 0;

    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupci�n MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupci�n MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}
