/*
 * serialPort.c
 *
 * Created: 07/10/2020 03:02:18 p. m.
 *  Author: Calder�n Sergio
 */ 

#include "SerialPort.h"

// Inicializaci�n de Puerto Serie
void SerialPort_Init(uint32_t f_cpu){
	// Deshabilitar receptor, transmisor e interrupciones
	UCSR0B = 0;
	
	// Definir tama�o de dato de 8 bits (8 bit data)
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	
	// Para la frecuencia de CPU actual, configurar a 9600 bps, 1 stop
	if (f_cpu == 16000000UL) UBRR0L = 103;
	else if (f_cpu == 8000000UL) UBRR0L = 51;
	else if (f_cpu == 4000000UL) UBRR0L = 25;
}


// Inicializaci�n de Transmisor

void SerialPort_TX_Enable(void){
	UCSR0B |= (1<<TXEN0);
}

void SerialPort_TX_Interrupt_Enable(void){
	UCSR0B |= (1<<UDRIE0);
}

void SerialPort_TX_Interrupt_Disable(void)
{
	UCSR0B &=~(1<<UDRIE0);
}


// Inicializaci�n de Receptor

void SerialPort_RX_Enable(void){
	UCSR0B |= (1<<RXEN0);
}

void SerialPort_RX_Interrupt_Enable(void){
	UCSR0B |= (1<<RXCIE0);
}


// Transmisi�n

void SerialPort_Send_Data(uint8_t data){
	UDR0 = data;
}


// Recepci�n

uint8_t SerialPort_Receive_Data(void){
	return UDR0;
}
