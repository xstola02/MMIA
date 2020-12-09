/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#include "lwip/opt.h"
#include <string.h>

#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"

#define TELNET_THREAD_PRIO  ( tskIDLE_PRIORITY + 4 )
#define CMD_BUFFER_LEN 64


static void telnet_process_command(char *cmd, struct netconn *conn)
	{
	char s[64];
	char *token;
	char *saveptr1;
		token = strtok_r(cmd, " ", &saveptr1);

	sprintf(s,"Prijato: %s\n", cmd);

	netconn_write(conn, s, strlen(s), NETCONN_COPY);


	if (strcasecmp(token, "HELLO") == 0)		//pokud napiseme "HELLO"
		{
			printf("Komunikace OK\n");
		}

		//_____________________________________________

		else if (strcasecmp(token, "LD1") == 0)		//pokud napiseme "LD1"
		{
			token = strtok_r(NULL, " ", &saveptr1);

			if (strcasecmp(token, "ON") == 0)  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);

			else if (strcasecmp(token, "OFF") ==0)  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
			printf("OK\n");
		}

		//_____________________________________________

		else if (strcasecmp(token, "LD2") == 0) 			//pokud napiseme "LD2"
			{
				token = strtok_r(NULL, " ", &saveptr1);

				if (strcasecmp(token, "ON") == 0)  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

				else if (strcasecmp(token, "OFF") == 0)  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
				printf("OK\n");
			}

		//_____________________________________________

		else if (strcasecmp(token, "LD3") == 0) 			//pokud napiseme "LD3"
			{
				token = strtok_r(NULL, " ", &saveptr1);

				if (strcasecmp(token, "ON") == 0)  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);

				else if (strcasecmp(token, "OFF") == 0)  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
				printf("OK\n");
			}

		//_____________________________________________


		else if (strcasecmp(token, "STATUS") == 0) 			//pokud napiseme "STATUS"
		{

			int state_led1 = HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin); 		//nacti set/reset hodnotu pinu
			if (state_led1 == 0)
				printf("LED1 OFF\n");
			else if (state_led1 == 1)
				printf("LED1 ON\n");

			int state_led2 = HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin);
			if (state_led2 == 0)
				printf("LED2 OFF\n");
			else if (state_led2 == 1)
				printf("LED2 ON\n");

			int state_led3 = HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin);
						if (state_led3 == 0)
							printf("LED3 OFF\n");
						else if (state_led3 == 1)
							printf("LED3 ON\n");

		}








	}

static void telnet_byte_available(uint8_t c, struct netconn *conn)
{
	static uint16_t cnt;
	static char data[CMD_BUFFER_LEN];
	if (cnt < CMD_BUFFER_LEN && c >= 32 && c <= 127)
		data[cnt++] = c;
	if (c == '\n' || c == '\r') {
		data[cnt] = '\0';
		telnet_process_command(data, conn);
		cnt = 0;
	}
}


/*-----------------------------------------------------------------------------------*/
static void telnet_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  struct netbuf *buf;
  uint8_t *data;
  u16_t len;
      
  LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!=NULL)
  {  
    /* Bind connection to well known port number 7. */
    err = netconn_bind(conn, NULL, 23);
    
    if (err == ERR_OK)
    {
      /* Tell connection to go into listening mode. */
      netconn_listen(conn);
    
      while (1) 
      {
        /* Grab new connection. */
         accept_err = netconn_accept(conn, &newconn);
    
        /* Process the new connection. */
        if (accept_err == ERR_OK) 
        {

          while (netconn_recv(newconn, &buf) == ERR_OK) 
          {
            do 
            {
              netbuf_data(buf, (void**)&data, &len);
              while (len--) telnet_byte_available(*data++, newconn);

            } 
            while (netbuf_next(buf) >= 0);
          
            netbuf_delete(buf);
          }
        
          /* Close connection and discard connection identifier. */
          netconn_close(newconn);
          netconn_delete(newconn);
        }
      }
    }
    else
    {
      netconn_delete(newconn);
    }
  }
}
/*-----------------------------------------------------------------------------------*/

void telnet_init(void)
{
  sys_thread_new("telnet_thread", telnet_thread, NULL, DEFAULT_THREAD_STACKSIZE, TELNET_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
