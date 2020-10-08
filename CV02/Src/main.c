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


void EXTI0_1_IRQHandler(void)
 {
	if (EXTI->PR & EXTI_PR_PR0)  // check line 0 has triggered the IT
	{
		EXTI->PR |= EXTI_PR_PR0; // clear the pending bit
		GPIOB->ODR ^=  (1<<4) ;
    }
 }

int main(void)
{
	 RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN; // enable
	 GPIOA->MODER |= GPIO_MODER_MODER4_0; // LED1 = PA4, output
	 GPIOB->MODER |= GPIO_MODER_MODER0_0; // LED2 = PB0, output
	 GPIOC->PUPDR |= GPIO_PUPDR_PUPDR0_0; // S2 = PC0, pullup
	 GPIOC->PUPDR |= GPIO_PUPDR_PUPDR1_0; // S1 = PC1, pullup

	 RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;


	 SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC; // select PC0 for EXTI0
	 EXTI->IMR |= EXTI_IMR_MR0; // mask
	 EXTI->FTSR |= EXTI_FTSR_TR0; // trigger on falling edge
	 NVIC_EnableIRQ(EXTI0_1_IRQn); // enable EXTI0_1

    /* Loop forever */
	for(;;)
	{

	}

}
