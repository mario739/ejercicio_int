/*
 * Copyright (c) YEAR NOMBRE <MAIL>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : application.c
 * @date   : May 10, 2022
 * @author : NOMBRE <MAIL>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
#include "application.h"

/********************** macros and definitions *******************************/
#define BUTTON_TIME_MAX_VALUE_          (9999)
#define BUTTON_MSG_MAX_LEN_             (10)
/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
static QueueHandle_t queue_1;
static char const *const g_str_led_on_ = "LED ON";
static char const *const g_str_button_format_ = "TEC%d T%d";
/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

static void task_sender_a(void *p_parameter)
{
  char data_print[30];
  led_init(LD1_GPIO_Port, LD1_Pin);
  while (true)
  {
    led_toggle(LD1_GPIO_Port, LD1_Pin);
    if (true == led_read_state(LD1_GPIO_Port, LD1_Pin))
    {
      printf_data("[a] Led ON\r\n");
      size_t str_msg_len = strlen(g_str_led_on_);
      char *str_msg = (char*)pvPortMalloc(str_msg_len + 1);
      if (NULL != str_msg)
      {
        strncpy(str_msg, g_str_led_on_, str_msg_len + 1);
        portBASE_TYPE status;
        status = xQueueSend(queue_1, (void* )&str_msg, 0);
        if (pdPASS == status)
        {
          sprintf(data_print,"[a] Send: %s\r\n",str_msg);
          printf_data(data_print);
        }
      }
    }
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}
static void task_sender_b(void *p_parameter)
{
  char data_print[50];
  char str_msg_buffer[BUTTON_MSG_MAX_LEN_ + 1];
  buttons_init();
  while (true)
  {
    for (unsigned int button_id = 0; button_id < 2; ++button_id)
    {
      uint32_t time = button_press_time(button_id);
      sprintf(data_print,"[b] Button:%u, Time:%lu\r\n",button_id, time);
      printf_data(data_print);
      memset(data_print,0,sizeof(data_print));

      if (BUTTON_TIME_MAX_VALUE_ < time)
      {
        time = BUTTON_TIME_MAX_VALUE_;
      }
      snprintf(str_msg_buffer, BUTTON_MSG_MAX_LEN_ + 1, g_str_button_format_, button_id, time);
      size_t str_msg_len = strlen(str_msg_buffer);
      char *str_msg = (char*)pvPortMalloc(str_msg_len + 1);
      if (NULL != str_msg)
      {
        strncpy(str_msg, str_msg_buffer, str_msg_len + 1);

        portBASE_TYPE status;
        status = xQueueSendToBack(queue_1, (void* )&str_msg, 0);
        if (pdPASS == status)
        {
          sprintf(data_print,"[b] Send: %s\r\n",str_msg);
          printf_data(data_print);
        }
      }
    }

    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}
static void tarea_receiver_c(void *p_parameter)
{
  portBASE_TYPE status;
  char const *str_msg = NULL;
  char data_print[30];
  while (true)
  {
    status = xQueueReceive(queue_1, (void*)&str_msg, portMAX_DELAY);
    if (pdPASS == status)
    {
      sprintf(data_print,"[c] Receive: %s\r\n",str_msg);
      printf_data(data_print);
      vPortFree((void*)str_msg);
      str_msg = NULL;
    }
  }
}
int application(void)
{
  BaseType_t res;
  res = xTaskCreate(task_sender_a, (const char*)"task_sender_a", configMINIMAL_STACK_SIZE * 2, NULL,tskIDLE_PRIORITY + 1, NULL);
  configASSERT(res == pdPASS);

  res = xTaskCreate(task_sender_b, (const char*)"task_sender_b", configMINIMAL_STACK_SIZE * 2, NULL,tskIDLE_PRIORITY + 1,NULL);
  configASSERT(res == pdPASS);

  res = xTaskCreate(tarea_receiver_c, (const char*)"tarea_receiver_c", configMINIMAL_STACK_SIZE * 2, NULL,tskIDLE_PRIORITY + 1,NULL);
  configASSERT(res == pdPASS);

  queue_1 = xQueueCreate(5, sizeof(char*));
  configASSERT(queue_1!= NULL);
  osKernelStart();

  while (1)
  {
  }
  return 1;
}

/********************** end of file ******************************************/
