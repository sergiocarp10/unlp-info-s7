/*
 * MiPrimerHolaMundo.c
 *
 * Created: 14/3/2022 15:26:25
 * Author : NotBad
 */ 

/* Inclusi�n de cabeceras de bibliotecas de c�digo */
#include <avr/io.h>		// Definici�n de Registros del microcontrolador
#define F_CPU 8000000UL // Especifico la frecuencia de reloj del MCU en 8MHz
#include <util/delay.h> // Retardos por software � Macros: depende de F_CPU

/* Funci�n main */
int main (void)
{
	/* Setup */
	DDRC &=~(1<<PC0);	// PC0 es entrada
	PORTC |= (1<<PC0);	// Escribo un "1" en PC0 (�?)
	DDRB = 0xFF;		// Todo PORTB es salida
	
	/* Loop */
	while(1)
	{
		if (PINC & (1<<PINC0))		// Si PINC0 vale 1
		{
			PORTB = 0b10101010;		// Bits pares de PORTB en bajo, impares en alto
			_delay_ms(100);			// Dormir por 100 ms
			PORTB = 0x00;			// Escribir "0" en todo PORTB
			_delay_ms(100);			// Dormir por 100 ms
		}
		else
		{
			PORTB = 0b01010101;		// Bits pares de PORTB en alto, impares en bajo
			_delay_ms(100);			// Dormir por 100 ms
			PORTB = 0x00;			// Escribir "0" en todo PORTB
		_delay_ms(100);				// Dormir por 100 ms
		}
	}
	
	/* Punto de finalizaci�n del programa (NO se debe llegar a este lugar) */
	return 0;

}