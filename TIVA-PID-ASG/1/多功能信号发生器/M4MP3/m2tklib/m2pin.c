/*

  m2pin.c
  
  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2011  olikraus@gmail.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "m2.h"


#define M2_IS_KEY_ASSIGNED 1

struct _m2_key_to_pin_entry
{
  uint8_t status;
#if 1
  Pin_Map pin;
#else
  uint8_t pin;
#endif
};
typedef struct _m2_key_to_pin_entry m2_key_to_pin_entry_t;

m2_key_to_pin_entry_t m2_key_to_pin_array[M2_KEY_CNT];

static uint8_t m2_get_idx_by_key(uint8_t key) M2_NOINLINE;

static uint8_t m2_get_idx_by_key(uint8_t key)
{
  if ( key > 0 )
  {
    key--;
    if ( key >= M2_KEY_CNT )
    {
      key = M2_KEY_CNT-1;
    }
  }
  return key;
}

#if 1
void m2_SetPin(uint8_t key, Pin_Map pin)
#else
void m2_SetPin(uint8_t key, uint8_t pin)
#endif
{
  uint8_t idx = m2_get_idx_by_key(key);
  m2_key_to_pin_array[idx].status |= M2_IS_KEY_ASSIGNED;
#if 1
    memcpy(&m2_key_to_pin_array[idx].pin, &pin, sizeof(Pin_Map));
#else
    m2_key_to_pin_array[idx].pin = pin;
#endif
  m2_InitEventSource();
}

#if 1
Pin_Map m2_GetPin(uint8_t key)
#else
uint8_t m2_GetPin(uint8_t key)
#endif
{
  uint8_t idx = m2_get_idx_by_key(key);
  return m2_key_to_pin_array[idx].pin;
}

uint8_t m2_IsPinAssigned(uint8_t key)
{
  uint8_t idx = m2_get_idx_by_key(key);
  return m2_key_to_pin_array[idx].status & M2_IS_KEY_ASSIGNED;
}

