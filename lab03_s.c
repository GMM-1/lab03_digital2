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

unsigned char dato_rx = 0;

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
    TRISDbits.TRISD0 = 0;                      // Pin RD0 como salida
    TRISDbits.TRISD1 = 0;                      // Pin RD1 como salida
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    SPI_Init_Slave(SPI_SLAVE_PIN, MODE_1);
    
    while (1)
    {
        dato_rx = SPI_Read();               // Lee el dato que envia el esclavo
        if(dato_rx > 0)                     // Verifica si hay datos recibidos
        {
            switch(dato_rx)                 // Compara el dato recibido
            {
                case 0x10:                  // Si el dato es 0x10, enciende o apaga el led 1
                    PORTDbits.RD0 = !PORTDbits.RD0;
                    break;
                    
                case 0x20:                  // Si el dato es 0x20, enciende o apaga el led 2
                    PORTDbits.RD1 = !PORTDbits.RD1;
                    break;
            }
            dato_rx = 0;
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
