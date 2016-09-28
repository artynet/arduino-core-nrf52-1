/*
  LowPower class for nRF52.
  Written by Chiara Ruggeri (chiara@arduino.org)
  
  Copyright (c) 2016 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Enjoy!  
*/

#ifndef LowPower_h
#define LowPower_h

#include "Arduino.h"
#include "nrf_timer.h"
#include "nrf_rtc.h"
#include "nrf_clock.h"
#include "nrf_lpcomp.h"

#define isGPIOMask 	0x10000
#define isNFCMask 	0x80000
#define isCompMask	0x20000

typedef enum{
	CONST_LATENCY = 0,
	LOW_POWER = 1
} standbyType;

typedef enum{
	OTHER = 0,
	GPIOReset = 1,
	NFCReset = 2,
	CompReset = 3
} resetReason;


class LowPowerClass
{
	public:
	
		/**
		* @brief
		* Name:
		*			powerOFF
		* Description:
		*			Put nRF52 in System OFF mode. System OFF is the
		* 			deepest power saving mode the system can enter.
		*			When in System OFF the device can be woken up through
		*			the following signals:
		*			-DETECT signal, generated by GPIO peripheral
		*			-ANADETECT signal, generated by LPCOMP module
		*			-SENSE signal, generated by NFC module to "wake-on-field"
		*			The device is reset when it wakes up from System OFF mode.
		*/
		void powerOFF(void);
		
		
		/**
		* @brief
		* Name:
		*			WakeUpByGPIO
		* Description:
		*			Configure the pin that will wakes up the board from the
		*			System OFF mode.
		* Arguments:
		*			-pinNo: number of pin
		*			-level: sense level that wakes up the board [LOW - HIGH]
		*/
		void wakeUpByGPIO(uint8_t pinNo, uint8_t level);
		
		
		/**
		* @brief
		* Name:
		*			WakeUpByNFC
		* Description:
		*			Wake up the board from System OFF mode when an NFC field
		*			is detected.
		*/
		void wakeUpByNFC(void);

		
		/**
		* @brief
		* Name:
		*			WakeUpByComp
		* Description:
		*			Wake up the board from System OFF mode when an the specified
		*			event on the selected analog pin is detected.
		* Arguments:
		*			-
		*/
		void wakeUpByComp(uint8_t pin, nrf_lpcomp_ref_t reference, detect_mode mode);

		/**
		* @brief
		* Name:
		*			WhoIs
		* Description:
		*			Detect the reason of the reset from System OFF mode.
		* Returned value:
		*			OTHER - Power on reset or other reasons.
		*			GPIOReset - GPIO caused the reset.
		*			NFCReset - NFC caused the reset.
		*			CompReset - Comparator caused the reset.
		*/
		resetReason whoIs(void);
		

		/**
		* @brief
		* Name:
		*			standby
		* Description:
		*			Put nRF52 in idle state. In this state every kind of event
		*			or interrupt will wake up the board, so make sure you manage
		*			any other interrupt request you want to use if you don't
		*			want to use the timer.
		*			When an interrupt occurs the related ISR will be served and
		*			then the board will return in standby.
		*			Two kind of sub power mode are available. Constant latency
		*			ensure the minimum time of response by keeping some resource
		*			active. Low power ensure the minimum power consumption for
		*			standby mode with a variable response time. See page 78 of
		*			datasheet for more information.
		*			In this function RTC is used as timer to minimizing power
		*			consumption.
		* Argument:
		*			-sec: number of seconds to wait in idle
		*			-function: function called when the timer generate an interrupt
		*			-mode: type of sub power mode (constant latency or low power)
		*/
		void standby(uint32_t sec, void(*function)(void), standbyType mode);
		//standbyType set to default mode - Low power
		void standby(uint32_t sec=0, void(*function)(void)=NULL);

		
		/**
		* @brief
		* Name:
		*			standbyMsec
		* Description:
		*			Put nRF52 in idle state. In this state every kind of event
		*			or interrupt will wake up the board, so make sure you manage
		*			any other interrupt request you want to use if you don't
		*			want to use the timer.
		*			When an interrupt occurs the related ISR will be served and
		*			then the board will return in standby.
		*			Two kind of sub power mode are available. Constant latency
		*			ensure the minimum time of response by keeping some resource
		*			active. Low power ensure the minimum power consumption for
		*			standby mode with a variable response time. See page 78 of
		*			datasheet for more information.
		*			In this function timer is used to wake up the board.
		* Argument:
		*			-msec: number of milli seconds to wait in idle
		*			-function: function called when the timer generate an interrupt
		*			-mode: type of sub power mode (constant latency or low power)
		*/
		void standbyMsec(uint32_t msec, void(*function)(void), standbyType mode);
		//standbyType set to default mode - Low power
		void standbyMsec(uint32_t msec=0, void(*function)(void)=NULL);

		
		/**
		* @brief
		* Name:
		*			wakeUpNow
		* Description:
		*			This funciton must to be called inside external interrupt
		*			handler to wake up the board if you are using standby
		*			low power mode.
		*/
		void wakeUpNow(void);
	
		// Callback user function
		void (*functionCallback)(void);
};

extern volatile bool event;
extern LowPowerClass LowPower;

#endif //LowPower_h