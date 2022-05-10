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
#include "messages_transfer.h"

/********************** macros and definitions *******************************/
#define BUTTON_TIME_MAX_VALUE_          (9999)
#define BUTTON_MSG_MAX_LEN_             (10)
/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
static char const *const g_str_led_on_ = "LED ON";
static char const *const g_str_button_format_ = "TEC%d T%d";
static msgbuffer_t g_msgbuffer_a1;
/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

static void led_routine(bool enable)
{
  if (false == enable)
  {
    return;
  }
  printf_data("[a] Led ON\r\n");
  size_t str_msg_len = strlen(g_str_led_on_);

  void *p_message = msgbuffer_sender_message_create(&g_msgbuffer_a1, str_msg_len + 1);
  {
    if (NULL != p_message)
    {
      strncpy((char*)p_message, g_str_led_on_, str_msg_len + 1);
    }
  }
  msgbuffer_sender_post(&g_msgbuffer_a1, p_message, 0);

}
static void task_sender_a(void *p_parameter)
{
  led_init(LD1_GPIO_Port, LD1_Pin);
  while (true)
  {
    led_toggle(LD1_GPIO_Port, LD1_Pin);
    led_routine(led_read_state(LD1_GPIO_Port, LD1_Pin));
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

static size_t generate_message_task_b(uint8_t button_id)
{
  char data_print[50];
  char str_msg_buffer[BUTTON_MSG_MAX_LEN_ + 1];

  uint32_t time = button_press_time(button_id);

  sprintf(data_print, "[b] Button:%u, Time:%lu\r\n", button_id, time);
  printf_data(data_print);

  if (BUTTON_TIME_MAX_VALUE_ < time)
  {
    time = BUTTON_TIME_MAX_VALUE_;
  }
  snprintf(str_msg_buffer, BUTTON_MSG_MAX_LEN_ + 1, g_str_button_format_, button_id, time);
  return strlen(str_msg_buffer);
}

static void rutine_buttons(uint8_t button_id)
{
  char str_msg_buffer[BUTTON_MSG_MAX_LEN_ + 1];
  size_t str_msg_len = generate_message_task_b(button_id);

  void *p_message = msgbuffer_sender_message_create(&g_msgbuffer_a1, str_msg_len + 1);
  {
    if (NULL != p_message)
    {
      strncpy((char*)p_message, str_msg_buffer, str_msg_len + 1);
    }
  }
  msgbuffer_sender_post(&g_msgbuffer_a1, p_message, 0);

}

static void task_sender_b(void *p_parameter)
{
  buttons_init();
  while (true)
  {
    rutine_buttons(1);
    rutine_buttons(0);
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

static void tarea_receiver_c(void *p_parameter)
{
  char data_print[30];
  while (true)
  {
    void *p_message = msgbuffer_receiver_get(&g_msgbuffer_a1, portMAX_DELAY);
    if (p_message!=NULL)
    {
      sprintf(data_print,"[c] msg: %s\r\n",p_message);
      printf_data(data_print);
    }
    msgbuffer_receiver_message_destroid(&g_msgbuffer_a1, p_message);
  }
}
int application(void)
{
  BaseType_t res;
  res = xTaskCreate(task_sender_a, (const char*)"task_sender_a", configMINIMAL_STACK_SIZE * 2, NULL,tskIDLE_PRIORITY + 1, NULL);
  configASSERT(res == pdPASS);

  res = xTaskCreate(task_sender_b, (const char*)"task_sender_b", configMINIMAL_STACK_SIZE * 2, NULL,tskIDLE_PRIORITY + 1, NULL);
  configASSERT(res == pdPASS);

  res = xTaskCreate(tarea_receiver_c, (const char*)"tarea_receiver_c", configMINIMAL_STACK_SIZE * 2, NULL,tskIDLE_PRIORITY + 1, NULL);
  configASSERT(res == pdPASS);

  msgbuffer_init(&g_msgbuffer_a1, 5);
  osKernelStart();

  while (1)
  {
  }
  return 1;
}

/********************** end of file ******************************************/
