/*
 * sct.c
 *
 *  Created on: 15. 10. 2020
 *      Author: Hana Stolarova
 */

//including all .h files
#include "stm32f0xx.h"
#include  "sct.h"
#include "main.h"

//defining functions/macros so we can set register signals

void sct_led(uint32_t value)
{
	for (uint8_t j = 0; j<32; j++)
	{
		HAL_GPIO_WritePin(SCT_SDI_GPIO_Port, SCT_SDI_Pin, (value & 1));  //assigning value 1 to sct_sdi using HAL command

		// sct_sdi(value & 1);
		value >>=1;
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 1); //assigning value 1 to sct_clk using HAL command
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 0);
	}

	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, 1); //assigning value 1 to sct_nla using HAL command
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, 0);

	//sct_nla(1);
	//sct_nla(0);

}


void sct_init(void)
{
	HAL_GPIO_WritePin(SCT_NOE_GPIO_Port, SCT_NOE_Pin, 0); //zeros into registers
	//sct_noe(0);
	sct_led(0);			//output activation using neg.OE

}

void sct_value (uint16_t value) //function uses value, renders its value into segments which then displayson the board
{
	static const uint32_t reg_values[3][10] =				//translation of numbers into segments
	{
	 {
	     //PCDE--------GFAB @ DIS1 							//this part tells which segments are ment to light up for specific numbers
		 0b0111000000000111 << 16,
		 0b0100000000000001 << 16,
		 0b0011000000001011 << 16,
		 0b0110000000001011 << 16,
		 0b0100000000001101 << 16,
		 0b0110000000001110 << 16,
		 0b0111000000001110 << 16,
		 0b0100000000000011 << 16,
		 0b0111000000001111 << 16,
		 0b0110000000001111 << 16,
	 },
	 {
		 //----PCDEGFAB---- @ DIS2
		 0b0000111101110000 << 0,
		 0b0000110000010000 << 0,
		 0b0000101110110000 << 0,
		 0b0000111010110000 << 0,
		 0b0000110011010000 << 0,
		 0b0000111011100000 << 0,
		 0b0000111111100000 << 0,
		 0b0000110000110000 << 0,
		 0b0000111111110000 << 0,
		 0b0000111011110000 << 0,
	 },
	 {
		 //PCDE--------GFAB @ DIS3
		 0b0111000000000111 << 0,
		 0b0100000000000001 << 0,
		 0b0011000000001011 << 0,
		 0b0110000000001011 << 0,
		 0b0100000000001101 << 0,
		 0b0110000000001110 << 0,
		 0b0111000000001110 << 0,
		 0b0100000000000011 << 0,
		 0b0111000000001111 << 0,
		 0b0110000000001111 << 0,
	 },
	 };

	uint32_t reg = 0;

	reg |= reg_values[0][value/100 % 10];		//value of the position of hundreds
	reg |= reg_values[1][value/10 % 10];		//value of the position of tens
	reg |= reg_values[2][value/1 % 10];			//value of the position of units

	sct_led(reg); 			//enlisting final value into register
}













