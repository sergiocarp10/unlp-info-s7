/*
 * controlador.h
 *
 * Created: 30/7/2022 04:31:12
 *  Author: Calder�n Sergio
 */ 


#ifndef CONTROLADOR_H_
	#define CONTROLADOR_H_

	// ----------------------- Includes ---------------
	
	// Archivo de cabecera del M�dulo LED
	#include "led.h"
	
	// Archivo de cabecera del Conversor Anal�gico-Digital
	#include "adc.h"

	// Archivo de cabecera para manejo de mensajes
	#include "CLI.h"

	// ----------------- Prototipos de funciones p�blicas ----------
	
	// Selecciona un LED para su posterior regulaci�n con ADC
	void Controlador_Select(LED_Option option);
	
	void Controlador_Set_RGB(uint8_t r, uint8_t g, uint8_t b);
	
	void Controlador_Clear();
	
	void Controlador_ADC_Update(uint8_t bright);
	
	void Controlador_ADC_ON();
	void Controlador_ADC_OFF();

#endif /* CONTROLADOR_H_ */