/*
 ********************************************************************************
Universidad del Valle de Guatemala
 * curso: 
Autor: Gabriel Andrade
compilador: XC8
proyecto: 
hardware: PIC 16F887
creado: 
última modificación: 
 ********************************************************************************
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 4000000
#include <xc.h>
#include "spi.h"                            // Libreria del protocolo SPI

#define SS_DIR TRISBbits.TRISB2                // Pin SS
#define SS_PIN PORTBbits.RB2


// modulos distintos    SSPSTAT-CKE | SSPCON1=SSPCON 
////////////////////////////////////////////////////////////////////////////////
// PROTOTIPOS DE FUNCIONES
////////////////////////////////////////////////////////////////////////////////
void setupINTOSC(void);

////////////////////////////////////////////////////////////////////////////////
// CODIGO PRINCIPAL
////////////////////////////////////////////////////////////////////////////////

void main()
{
    setupINTOSC();
    ANSEL = 0x00;
    ANSELH = 0x00;                      // Coloca todos los pines como digitales
    TRISAbits.TRISA0 = 1;               // Pin RA0 como entrada
    TRISAbits.TRISA1 = 1;               // Pin RA1 como entrada
    SS_DIR = 0;                         // Pin SS como salida
    
    SPI_Init_Master(SPI_CLK_DIV_16, MODE_1, HALF_DATA);
    
    SS_PIN = 1;                        // Inicialmente deshabilita al esclavo
    
    while (1)
    {
        if(PORTAbits.RA0 == 1)              // Control del led 1 del esclavo
        {
            while(PORTAbits.RA0 == 1);
            __delay_ms(20);
            
            SS_PIN = 0;                     // Habilita al esclavo
            SPI_Write(0x10);                // Envia el dato al esclavo por SPI
            SS_PIN = 1;                     // Deshabilita al esclavo
        }
        
        if(PORTAbits.RA1 == 1)              // Control del led 2 del esclavo
        {
            while(PORTAbits.RA1 == 1);
            __delay_ms(20);
            
            SS_PIN = 0;                     // Habilita al esclavo
            SPI_Write(0x20);                // Envia el dato al esclavo por SPI
            SS_PIN = 1;                     // Deshabilita al esclavo
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// FUNCIONES
////////////////////////////////////////////////////////////////////////////////

void setupINTOSC(void) {
    //Seleccion de Oscilador interno
    OSCCONbits.SCS = 1;

    // oscilador a 4MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
}
