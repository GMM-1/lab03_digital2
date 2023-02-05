/*
********************************************************************************
Universidad del Valle de Guatemala
curso: Electronica Digital 2
Autor: Gabriel Andrade
compilador: XC8
proyecto: laboratorio 03
hardware: PIC 16F887
creado: 03/01/2023
última modificación: 03/02/2023
********************************************************************************
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuración
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
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdio.h>
#include "SPI.h"
#include "LCD.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo
// contrario hay que colocarlos todas las funciones antes del main
char buffer[20]; //arreglo para guardar caracteres o variables y mostrar en la LCD
unsigned char dato_1;
unsigned char dato_2;
//*****************************************************************************
void setup(void);

//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Init();


    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);

       spiWrite(1);
       dato_1 = spiRead();       //recibo informacion y coloco en portd
       Lcd_Set_Cursor(1,1);
       sprintf(buffer, "ADC1: %u  cont:  ", dato_1);
        Lcd_Write_String(buffer);

       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect

       PORTCbits.RC1 = 0;       //Slave Select
       __delay_ms(1);

       spiWrite(1);
       dato_2 = spiRead();       //recibo informacion y coloco en portd
       Lcd_Set_Cursor(2,1);
       sprintf(buffer, "ADC2: %u    0  ", dato_2);
        Lcd_Write_String(buffer);

       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave Deselect

       //__delay_ms(250);
       //PORTB++;
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void)
{
    // oscilador a 4MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;

     //Seleccion de Oscilador interno
    OSCCONbits.SCS = 1;
    //puertos digitales
    ANSEL = 0;
    ANSELH = 0;

    //colocamos el slave select como salida
    TRISC2 = 0;
    TRISC1 = 0;

    //Salidas: PORTB & PORTD
    TRISB = 0;
    TRISD = 0;

    //reinicio de puertos
    PORTB = 0;
    PORTD = 0;

    //inhabilitamos el esclavo inicialmente (esta invertida)
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;

    //palabras de configuracion del SPI para el maestro
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}
