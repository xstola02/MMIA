/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Hana Stolarova Git: https://github.com/xstola02/MMIA
 * @brief          : Main program body
 ******************************************************************************
**/
#include "stm32f0xx.h"



#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


int main(void)
{

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;				// real clock counter
	GPIOA->MODER |= GPIO_MODER_MODER5_0;			//configuring/setting input/output pin we want (mod 01)

    /* Loop forever */
	for(;;)
	{
		uint32_t pole = 0b10101001110111011100101010000000 ;  // field filled with binary number we want

		for (uint8_t i = 0; i < 32; i++)					// going through all 32 bits in pole
		{
			if(pole & 0x80000000) GPIOA->BSRR = (1<<5); 	// bit set/reset register
			else GPIOA->BRR = (1<<5); 						//bit reset register

			pole <<=1;										//bit shift of the field

			for (volatile uint32_t i = 0; i < 100000; i++) {} 	//empty waiting loop
		}
	}
}
