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
 * @file   : driver_buttons.c
 * @date   : May 10, 2022
 * @author : NOMBRE <MAIL>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include "driver_buttons.h"

/********************** macros and definitions *******************************/

#define EXP_STEP_BTN_0_                 (5)
#define EXP_STEP_BTN_1_                 (7)
#define MAX_TIME_                       (10 * 1000)
#define BUTTONS_N_                      (2)

/********************** internal data declaration ****************************/
typedef struct
{
    uint32_t cnt_btn_0;
    uint32_t cnt_btn_1;
} button_t;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
static button_t g_button_;
/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/
void buttons_init(void)
{
//  g_button_.time = false;
  g_button_.cnt_btn_0 = 1;
  g_button_.cnt_btn_1 = 1;
}

uint32_t button_press_time(unsigned int button_id)
{
  uint32_t ret = 0;
  switch (button_id)
  {
    case 0:
      g_button_.cnt_btn_0 = (MAX_TIME_ < g_button_.cnt_btn_0) ? 1 : g_button_.cnt_btn_0 * EXP_STEP_BTN_0_;
      g_button_.cnt_btn_0++;
      ret = g_button_.cnt_btn_0;
      break;
    case 1:
      g_button_.cnt_btn_1 = (MAX_TIME_ < g_button_.cnt_btn_1) ? 1 : g_button_.cnt_btn_1 * EXP_STEP_BTN_1_;
      g_button_.cnt_btn_1++;
      ret = g_button_.cnt_btn_1;
      break;
    default:
      ret = 0;
      break;
  }
  return ret;
}
/********************** end of file ******************************************/
