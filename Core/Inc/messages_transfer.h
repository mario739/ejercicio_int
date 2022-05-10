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
 * @file   : messages_transfer.h
 * @date   : 10 may. 2022
 * @author : NOMBRE <MAIL>
 * @version	v1.0.0
 */

#ifndef INC_MESSAGES_TRANSFER_H_
#define INC_MESSAGES_TRANSFER_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os.h"
#include "main.h"
/********************** macros ***********************************************/
#define MSGBUFFER_SIZE_CALCULATE(elements)              ((elements) * sizeof(void*))
/********************** typedef **********************************************/

/********************** external data declaration ****************************/
typedef struct
{
    xQueueHandle queue;
} msgbuffer_t;

/********************** external functions declaration ***********************/

bool msgbuffer_init(msgbuffer_t *self, size_t size);

void *msgbuffer_sender_message_create(msgbuffer_t const *self, size_t message_size);

bool msgbuffer_sender_post(msgbuffer_t const *self, void *message, portTickType delay);

void *msgbuffer_receiver_get(msgbuffer_t const *self, portTickType delay);

void msgbuffer_receiver_message_destroid(msgbuffer_t const *self, void *message);


/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* INC_MESSAGES_TRANSFER_H_ */
/********************** end of file ******************************************/

