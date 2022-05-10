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
 * @file   : messages_transfer.c
 * @date   : 10 may. 2022
 * @author : NOMBRE <MAIL>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include "messages_transfer.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/
bool msgbuffer_init(msgbuffer_t *self, size_t size)
{
  self->queue = xQueueCreate((unsigned portBASE_TYPE) size, sizeof(void*));

  if(NULL != self->queue)
  {
    return true;
  }
  return false;
}

void *msgbuffer_sender_message_create(msgbuffer_t const *self, size_t message_size)
{
  return pvPortMalloc(message_size);
}

bool msgbuffer_sender_post(msgbuffer_t const *self, void *message, portTickType delay)
{

  if(NULL == message)
  {
    return false;
  }

  portBASE_TYPE status;
  status = xQueueSendToBack(self->queue, (void*)&message, delay);
  if (pdPASS == status)
  {
    return true;
  }
  return false;
}

void *msgbuffer_receiver_get(msgbuffer_t const *self, portTickType delay)
{
  portBASE_TYPE status;
  void *message;

  status = xQueueReceive(self->queue, (void*)&message, delay);
  if (pdPASS == status)
  {
    return message;
  }
  return NULL;
}

void msgbuffer_receiver_message_destroid(msgbuffer_t const *self, void *message)
{
  if(NULL != message)
  {
    vPortFree((void*)message);
  }
}

/********************** end of file ******************************************/
