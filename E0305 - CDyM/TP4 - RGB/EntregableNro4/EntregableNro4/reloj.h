/*
 * reloj.h
 *
 * Created: 4/7/2022 14:50:40
 *  Author: Calder�n Sergio
 */ 


#ifndef RELOJ_H_
	#define RELOJ_H_

	// -------------------------- Includes ----------------------------
	
	// Archivo de cabecera del Microcontrolador
	#include <avr/io.h>
	
	// Archivo de cabecera del m�dulo LED, necesario para forzar apagados
	#include "led.h"
	
	// -------------------------- Tipos de Datos --------------------------
	
	// Se�ales disponibles entre los Timer 0 y 1 
	// Se puede activar, desactivar y modificar el Duty Cycle de cada una
	typedef enum {T1_PWM_A, T1_PWM_B, T0_NORMAL} RELOJ_Signal;
	
	// ------------------------- Constantes -----------------------------
	

	// --------------------- Prototipos de funciones p�blicas ------------
	
	// Inicializa el Timer 0
	// En este proyecto, es el encargado de generar interrupciones
	void RELOJ_T0_Init();
	
	// Inicializa el Timer 1
	// En este proyecto, es el encargado de generar las se�ales PWM
	void RELOJ_T1_Init();
	
	// Asigna el prescaler adecuado para dar inicio a los relojes
	void RELOJ_Start_Both();
	
	// Timer 0 - Interrupci�n de Overflow
	void RELOJ_T0_OVF_Interrupt_Enable();
	void RELOJ_T0_OVF_Interrupt_Disable();

	// Timer 0 - Interrupci�n de Compare Match A
	void RELOJ_T0_COMPA_Interrupt_Enable();
	void RELOJ_T0_COMPA_Interrupt_Disable();
	
	// Timer 0 - Actualizaci�n de brillo
	void RELOJ_T0_UpdateOCR(uint8_t);
	
	// Timer 1 - Se�al PWM de OC1A
	void RELOJ_T1_PWM_A_Enable();
	void RELOJ_T1_PWM_A_Disable();
	void RELOJ_T1_PWM_A_UpdateOCR(uint8_t);
	
	// Timer 1 - Se�al PWM de OC1B
	void RELOJ_T1_PWM_B_Enable();
	void RELOJ_T1_PWM_B_Disable();
	void RELOJ_T1_PWM_B_UpdateOCR(uint8_t);

#endif /* RELOJ_H_ */