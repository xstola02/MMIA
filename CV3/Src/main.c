/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Hana Stolarova Git: https://github.com/xstola02/MMIA
 * @brief          : Main program body
 ******************************************************************************
**/
#include "stm32f0xx.h"
#include  "sct.h"


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


int main(void)
{
	sct_init();
	sct_led(0x7A5C36DE);


    /* Loop forever */
	for(;;)
	{

	}

}
