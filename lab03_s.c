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

//variables para contador
int flag_antirrebote = 0;
int flag_antirrebote2 = 0;

uint8_t valor_adc;



//*****************************************************************************
// Definici�n de funciones para que se puedan colocar despu�s del main de lo
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void contador(void);
//*****************************************************************************
// C�digo de Interrupci�n
//*****************************************************************************
void __interrupt() isr(void)
{
   if(SSPIF == 1)
   {
       temporal = spiRead();
       if (temporal == 1)
       {
        spiWrite(valor_adc);
       }
       if(temporal == 0)
       {
         spiWrite(PORTD);
       }
        SSPIF = 0;
    }
   //INTERRUPCION DEL PORTB
    if (INTCONbits.RBIF == 1)//revisar bandera de interrupcion
    {
        contador();
        INTCONbits.RBIF = 0; //limpiar bandera
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
        valor_adc = ADC_Read(0);
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

    TRISB6 = 1;
    TRISB7 = 1;
    TRISD = 0;
    PORTD = 0x00;

    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupci�n MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupci�n MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select

    //interrupcion del puerto B (push buttons)
  INTCONbits.RBIE = 1; //interrupciones del puerto B
  OPTION_REGbits.nRBPU = 0; // habilitar pull ups individuales en B
  WPUBbits.WPUB7 = 1;       //pullup en B7
  WPUBbits.WPUB6 = 1;       //pullup en B6
  IOCBbits.IOCB7 = 1;       //interrupt on change en B7
  IOCBbits.IOCB6 = 1;       //interruot on change en B6
  INTCONbits.RBIF = 0;      //bandera apagada


    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}

void contador(void)
{
  if (PORTBbits.RB6 == 0) //antirrebotes
  {
    flag_antirrebote = 1;
  }
  if (PORTBbits.RB6 == 1 && flag_antirrebote == 1) //si se presiona el PB RB6...
  {
      __delay_ms(10);
      PORTD = PORTD + 1; //incrementamos en uno el puerto D
      flag_antirrebote = 0;
  }
  if (PORTBbits.RB7 == 0)//antirrebotes
  {
      flag_antirrebote2 = 1;
  }
  if (PORTBbits.RB7 == 1 && flag_antirrebote2 == 1)//si se presiona el PB RB7...
  {
      __delay_ms(10);
      PORTD = PORTD - 1;//decrementamos en uno el puerto D
      flag_antirrebote2 = 0;
  }
}
