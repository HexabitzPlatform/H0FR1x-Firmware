/*
 BitzOS (BOS) V0.3.0 - Copyright (C) 2017-2024 Hexabitz
 All rights reserved

 File Name     : H0FR1.c
 Description   : Source code for module H0FR1 SPDT mechanical DC relay



 Required MCU resources :

 >> USARTs 1,2,3,5,6 for module ports.
 >> GPIOB 0 for Switch.

 */

/* Includes ------------------------------------------------------------------*/
#include "BOS.h"

/* Define UART variables */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart6;

/* Module exported parameters ------------------------------------------------*/

module_param_t modParam[NUM_MODULE_PARAMS] ={{.paramPtr =NULL, .paramFormat =FMT_FLOAT, .paramName ="current"}};
/* Exported variables */
extern FLASH_ProcessTypeDef pFlash;
extern uint8_t numOfRecordedSnippets;

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
TimerHandle_t xTimerSwitch = NULL;
Switch_state_t Switch_state = STATE_OFF, Switch_Oldstate = STATE_ON;
uint8_t SwitchindMode = 0;
uint32_t  t0, temp32;

float Current = 0.0f;

/* Private function prototypes -----------------------------------------------*/
void SwitchTimerCallback(TimerHandle_t xTimerSwitch);
void TIM3_Init(void);
void TIM3_DeInit(void);
void SetupPortForRemoteBootloaderUpdate(uint8_t port);
void remoteBootloaderUpdate(uint8_t src,uint8_t dst,uint8_t inport,uint8_t outport);
/* Create CLI commands --------------------------------------------------------*/
portBASE_TYPE onCommand(int8_t *pcWriteBuffer,size_t xWriteBufferLen,const int8_t *pcCommandString);
portBASE_TYPE offCommand(int8_t *pcWriteBuffer,size_t xWriteBufferLen,const int8_t *pcCommandString);
portBASE_TYPE toggleCommand(int8_t *pcWriteBuffer,size_t xWriteBufferLen,const int8_t *pcCommandString);
portBASE_TYPE ledModeCommand(int8_t *pcWriteBuffer,size_t xWriteBufferLen,const int8_t *pcCommandString);


/* CLI command structure : on */
const CLI_Command_Definition_t onCommandDefinition =
		{ (const int8_t*) "on", /* The command string to type. */
				(const int8_t*) "on:\r\n Turn solid state Switch on with a timeout (ms) (1st par.). Use 'inf' to turn on constantly\r\n\r\n",
				onCommand, /* The function to run. */
				1 /* One parameter is expected. */
		};
/*-----------------------------------------------------------*/
/* CLI command structure : off */
const CLI_Command_Definition_t offCommandDefinition = { (const int8_t*) "off", /* The command string to type. */
(const int8_t*) "off:\r\n Turn solid state Switch off\r\n\r\n", offCommand, /* The function to run. */
0 /* No parameters are expected. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : toggle */
const CLI_Command_Definition_t toggleCommandDefinition = {
		(const int8_t*) "toggle", /* The command string to type. */
		(const int8_t*) "toggle:\r\n Toggle solid state Switch\r\n\r\n",
		toggleCommand, /* The function to run. */
		0 /* No parameters are expected. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : ledMode */
const CLI_Command_Definition_t ledModeCommandDefinition =
		{ (const int8_t*) "ledmode", /* The command string to type. */
				(const int8_t*) "ledMode:\r\n Set solid state Switch indicator LED mode ('on' or 'off') (1st par.)\r\n\r\n",
				ledModeCommand, /* The function to run. */
				1 /* One parameter is expected. */
		};
/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
 |				    	Private Functions	 						|							|
 -----------------------------------------------------------------------
 */

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1
			| RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	__SYSCFG_CLK_ENABLE()
	;

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}

/*-----------------------------------------------------------*/
/* --- Save array topology and Command Snippets in Flash RO --- 
 */
uint8_t SaveToRO(void) {
	BOS_Status result = BOS_OK;
	HAL_StatusTypeDef FlashStatus = HAL_OK;
	uint16_t add = 2, temp = 0;
	uint8_t snipBuffer[sizeof(snippet_t) + 1] = { 0 };

	HAL_FLASH_Unlock();

	/* Erase RO area */
	FLASH_PageErase(RO_START_ADDRESS);
	FlashStatus = FLASH_WaitForLastOperation(
			(uint32_t) HAL_FLASH_TIMEOUT_VALUE);
	if (FlashStatus != HAL_OK) {
		return pFlash.ErrorCode;
	} else {
		/* Operation is completed, disable the PER Bit */
		CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	}

	/* Save number of modules and myID */
	if (myID) {
		temp = (uint16_t) (N << 8) + myID;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, RO_START_ADDRESS, temp);
		FlashStatus = FLASH_WaitForLastOperation(
				(uint32_t) HAL_FLASH_TIMEOUT_VALUE);
		if (FlashStatus != HAL_OK) {
			return pFlash.ErrorCode;
		} else {
			/* If the program operation is completed, disable the PG Bit */
			CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
		}

		/* Save topology */
		for (uint8_t i = 1; i <= N; i++) {
			for (uint8_t j = 0; j <= MaxNumOfPorts; j++) {
				if (array[i - 1][0]) {
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,
					RO_START_ADDRESS + add, array[i - 1][j]);
					add += 2;
					FlashStatus = FLASH_WaitForLastOperation(
							(uint32_t) HAL_FLASH_TIMEOUT_VALUE);
					if (FlashStatus != HAL_OK) {
						return pFlash.ErrorCode;
					} else {
						/* If the program operation is completed, disable the PG Bit */
						CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
					}
				}
			}
		}
	}

	// Save Command Snippets
	int currentAdd = RO_MID_ADDRESS;
	for (uint8_t s = 0; s < numOfRecordedSnippets; s++) {
		if (snippets[s].cond.conditionType) {
			snipBuffer[0] = 0xFE;		// A marker to separate Snippets
			memcpy((uint8_t*) &snipBuffer[1], (uint8_t*) &snippets[s],
					sizeof(snippet_t));
			// Copy the snippet struct buffer (20 x numOfRecordedSnippets). Note this is assuming sizeof(snippet_t) is even.
			for (uint8_t j = 0; j < (sizeof(snippet_t) / 2); j++) {
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, currentAdd,
						*(uint16_t*) &snipBuffer[j * 2]);
				FlashStatus = FLASH_WaitForLastOperation(
						(uint32_t) HAL_FLASH_TIMEOUT_VALUE);
				if (FlashStatus != HAL_OK) {
					return pFlash.ErrorCode;
				} else {
					/* If the program operation is completed, disable the PG Bit */
					CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
					currentAdd += 2;
				}
			}
			// Copy the snippet commands buffer. Always an even number. Note the string termination char might be skipped
			for (uint8_t j = 0; j < ((strlen(snippets[s].cmd) + 1) / 2); j++) {
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, currentAdd,
						*(uint16_t*) (snippets[s].cmd + j * 2));
				FlashStatus = FLASH_WaitForLastOperation(
						(uint32_t) HAL_FLASH_TIMEOUT_VALUE);
				if (FlashStatus != HAL_OK) {
					return pFlash.ErrorCode;
				} else {
					/* If the program operation is completed, disable the PG Bit */
					CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
					currentAdd += 2;
				}
			}
		}
	}

	HAL_FLASH_Lock();

	return result;
}

/* --- Clear array topology in SRAM and Flash RO --- 
 */
uint8_t ClearROtopology(void) {
	// Clear the array 
	memset(array, 0, sizeof(array));
	N = 1;
	myID = 0;

	return SaveToRO();
}

/* --- Trigger ST factory bootloader update for a remote module.
 */
void remoteBootloaderUpdate(uint8_t src,uint8_t dst,uint8_t inport,uint8_t outport){

	uint8_t myOutport =0, lastModule =0;
	int8_t *pcOutputString;

	/* 1. Get route to destination module */
	myOutport =FindRoute(myID,dst);
	if(outport && dst == myID){ /* This is a 'via port' update and I'm the last module */
		myOutport =outport;
		lastModule =myID;
	}
	else if(outport == 0){ /* This is a remote update */
		if(NumberOfHops(dst)== 1)
		lastModule = myID;
		else
		lastModule = route[NumberOfHops(dst)-1]; /* previous module = route[Number of hops - 1] */
	}

	/* 2. If this is the source of the message, show status on the CLI */
	if(src == myID){
		/* Obtain the address of the output buffer.  Note there is no mutual
		 exclusion on this buffer as it is assumed only one command console
		 interface will be used at any one time. */
		pcOutputString =FreeRTOS_CLIGetOutputBuffer();

		if(outport == 0)		// This is a remote module update
			sprintf((char* )pcOutputString,pcRemoteBootloaderUpdateMessage,dst);
		else
			// This is a 'via port' remote update
			sprintf((char* )pcOutputString,pcRemoteBootloaderUpdateViaPortMessage,dst,outport);

		strcat((char* )pcOutputString,pcRemoteBootloaderUpdateWarningMessage);
		writePxITMutex(inport,(char* )pcOutputString,strlen((char* )pcOutputString),cmd50ms);
		Delay_ms(100);
	}

	/* 3. Setup my inport and outport for bootloader update */
	SetupPortForRemoteBootloaderUpdate(inport);
	SetupPortForRemoteBootloaderUpdate(myOutport);


	/* 5. Build a DMA stream between my inport and outport */
	StartScastDMAStream(inport,myID,myOutport,myID,BIDIRECTIONAL,0xFFFFFFFF,0xFFFFFFFF,false);
}

/* --- Setup a port for remote ST factory bootloader update:
 - Set baudrate to 57600
 - Enable even parity
 - Set datasize to 9 bits
 */
void SetupPortForRemoteBootloaderUpdate(uint8_t port){
	UART_HandleTypeDef *huart =GetUart(port);

	huart->Init.BaudRate =57600;
	huart->Init.Parity = UART_PARITY_EVEN;
	huart->Init.WordLength = UART_WORDLENGTH_9B;
	HAL_UART_Init(huart);

	/* The CLI port RXNE interrupt might be disabled so enable here again to be sure */
	__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
}

/* --- (H0FR6 || H0FR7) module initialization ---
 */
void Module_Peripheral_Init(void) {

	/* Array ports */
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_USART5_UART_Init();
	MX_USART6_UART_Init();


	/* Create a timeout timer for Switch_on() API */
	xTimerSwitch =xTimerCreate("SwitchTimer",pdMS_TO_TICKS(1000),pdFALSE,(void* )1,SwitchTimerCallback);

	/* Switch GPIO */
	Switch_Init();
}



/*-----------------------------------------------------------*/

/* --- H0FR1 message processing task. */
Module_Status Module_MessagingTask(uint16_t code, uint8_t port, uint8_t src,
		uint8_t dst, uint8_t shift) {
	Module_Status result = H0FR1_OK;
	uint32_t period;
	uint32_t timeout;

	switch (code) {
	case CODE_H0FR1_ON:
		temp32 = cMessage[port - 1][shift] + ((uint32_t) cMessage[port - 1][1 + shift] << 8)+ ((uint32_t) cMessage[port - 1][2 + shift] << 16)+ ((uint32_t)cMessage[port - 1][3 + shift]<< 24);
		Output_on(temp32);
		break;

	case CODE_H0FR1_OFF:
		Output_off();
		break;

	case CODE_H0FR1_TOGGLE:
		Output_toggle();
		break;



	default:
		result = H0FR1_ERR_UnknownMessage;
		break;
	}

	return result;
}

/*-----------------------------------------------------------*/

/* --- Register this module CLI Commands 
 */
void RegisterModuleCLICommands(void) {
	FreeRTOS_CLIRegisterCommand(&onCommandDefinition);
	FreeRTOS_CLIRegisterCommand(&offCommandDefinition);
	FreeRTOS_CLIRegisterCommand(&toggleCommandDefinition);
	FreeRTOS_CLIRegisterCommand(&ledModeCommandDefinition);

}

/*-----------------------------------------------------------*/

/* --- Get the port for a given UART. 
 */
uint8_t GetPort(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART5)
		return P1;
	else if (huart->Instance == USART2)
		return P2;
	else if (huart->Instance == USART6)
		return P3;
	else if (huart->Instance == USART3)
		return P4;
	else if (huart->Instance == USART1)
		return P5;

	return 0;
}

/*-----------------------------------------------------------*/

/* --- Switch timer callback ---*/
void SwitchTimerCallback(TimerHandle_t xTimerSwitch) {

	Output_off();

}
/*-----------------------------------------------------------*/


/* -----------------------------------------------------------------------
 |					          APIs	 								|								 	|
 -----------------------------------------------------------------------
 */

/* --- Turn on the solid state Switch ---
 */
Module_Status Output_on(uint32_t timeout) {
	Module_Status result = H0FR1_OK;

	/* Turn on */
	HAL_GPIO_WritePin(_Switch_PORT, _Switch_PIN, GPIO_PIN_SET);

	/* Indicator LED */
	if (SwitchindMode)
		IND_ON();

	/* Timeout */
	if (timeout != portMAX_DELAY) {
		/* Stop the timer if it's already running */
		if (xTimerIsTimerActive(xTimerSwitch))
			xTimerStop(xTimerSwitch, 100);
		/* Update timer timeout - This also restarts the timer */
		xTimerChangePeriod(xTimerSwitch, pdMS_TO_TICKS(timeout), 100);
	}

	/* Update Switch state */
	Switch_state = STATE_ON;
	Switch_Oldstate = Switch_state;

	return result;
}

/*-----------------------------------------------------------*/

/* --- Turn off the solid state Switch ---
 */
Module_Status Output_off(void) {
	Module_Status result = H0FR1_OK;



	/* Turn off */
	HAL_GPIO_WritePin(_Switch_PORT, _Switch_PIN, GPIO_PIN_RESET);

	/* Indicator LED */
	if (SwitchindMode)
		IND_OFF();

	/* Update Switch state */
	Switch_state = STATE_OFF;

	return result;
}

/*-----------------------------------------------------------*/

/* --- Toggle the solid state Switch ---
 */
Module_Status Output_toggle(void) {
	Module_Status result = H0FR1_OK;

	if (Switch_state) {
		result = Output_off();
	} else {
		if (Switch_Oldstate == STATE_ON)
			result = Output_on(portMAX_DELAY);

	}

	return result;
}

/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
 |					    		Commands							|									 	|
 -----------------------------------------------------------------------
 */

portBASE_TYPE onCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen,
		const int8_t *pcCommandString) {
	Module_Status result = H0FR1_OK;

	int8_t *pcParameterString1;
	portBASE_TYPE xParameterStringLength1 = 0;
	uint32_t timeout = 0;
	static const int8_t *pcOKMessage =
			(int8_t*) "Solid state Switch is turned on with timeout %d ms\r\n";
	static const int8_t *pcOKMessageInf =
			(int8_t*) "Solid state Switch is turned on without timeout\r\n";

	/* Remove compile time warnings about unused parameters, and check the
	 write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	 write buffer length is adequate, so does not check for buffer overflows. */
	(void) xWriteBufferLen;
	configASSERT(pcWriteBuffer);

	/* Obtain the 1st parameter string. */
	pcParameterString1 = (int8_t*) FreeRTOS_CLIGetParameter(pcCommandString, /* The command string itself. */
	1, /* Return the first parameter. */
	&xParameterStringLength1 /* Store the parameter string length. */
	);

	if (!strcmp((char*) pcParameterString1, "inf")
			|| !strcmp((char*) pcParameterString1, "INF"))
		timeout = portMAX_DELAY;
	else
		timeout = (uint32_t) atol((char*) pcParameterString1);

	result = Output_on(timeout);

	/* Respond to the command */
	if (result == H0FR1_OK) {
		if (timeout != portMAX_DELAY) {
			sprintf((char*) pcWriteBuffer, (char*) pcOKMessage, timeout);
		} else {
			strcpy((char*) pcWriteBuffer, (char*) pcOKMessageInf);
		}
	}

	/* There is no more data to return after this single string, so return
	 pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/

portBASE_TYPE offCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen,
		const int8_t *pcCommandString) {
	Module_Status result = H0FR1_OK;

	static const int8_t *pcMessage =
			(int8_t*) "Solid state Switch is turned off\r\n";

	/* Remove compile time warnings about unused parameters, and check the
	 write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	 write buffer length is adequate, so does not check for buffer overflows. */
	(void) pcCommandString;
	(void) xWriteBufferLen;
	configASSERT(pcWriteBuffer);

	result = Output_off();

	/* Respond to the command */
	if (result == H0FR1_OK) {
		strcpy((char*) pcWriteBuffer, (char*) pcMessage);
	}

	/* There is no more data to return after this single string, so return
	 pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/

portBASE_TYPE toggleCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen,
		const int8_t *pcCommandString) {
	Module_Status result = H0FR1_OK;

	static const int8_t *pcOK1Message =
			(int8_t*) "Solid state Switch is turned on\r\n";
	static const int8_t *pcOK0Message =
			(int8_t*) "Solid state Switch is turned off\r\n";

	/* Remove compile time warnings about unused parameters, and check the
	 write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	 write buffer length is adequate, so does not check for buffer overflows. */
	(void) xWriteBufferLen;
	configASSERT(pcWriteBuffer);

	result = Output_toggle();

	/* Respond to the command */
	if (result == H0FR1_OK) {
		if (Switch_state) {
			strcpy((char*) pcWriteBuffer, (char*) pcOK1Message);
		} else {
			strcpy((char*) pcWriteBuffer, (char*) pcOK0Message);
		}
	}

	/* There is no more data to return after this single string, so return
	 pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/

portBASE_TYPE ledModeCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen,
		const int8_t *pcCommandString) {
	int8_t *pcParameterString1;
	portBASE_TYPE xParameterStringLength1 = 0;

	static const int8_t *pcOK1Message =
			(int8_t*) "Solid state Switch indicator LED is enabled\r\n";
	static const int8_t *pcOK0Message =
			(int8_t*) "Solid state Switch indicator LED is disabled\r\n";

	/* Remove compile time warnings about unused parameters, and check the
	 write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	 write buffer length is adequate, so does not check for buffer overflows. */
	(void) xWriteBufferLen;
	configASSERT(pcWriteBuffer);

	/* Obtain the 1st parameter string. */
	pcParameterString1 = (int8_t*) FreeRTOS_CLIGetParameter(pcCommandString, /* The command string itself. */
	1, /* Return the first parameter. */
	&xParameterStringLength1 /* Store the parameter string length. */
	);
	if (!strcmp((char*) pcParameterString1, "on")
			|| !strcmp((char*) pcParameterString1, "ON"))
		SwitchindMode = 1;
	else if (!strcmp((char*) pcParameterString1, "off")
			|| !strcmp((char*) pcParameterString1, "OFF"))
		SwitchindMode = 0;

	/* Respond to the command */
	if (SwitchindMode) {
		strcpy((char*) pcWriteBuffer, (char*) pcOK1Message);
	} else {
		strcpy((char*) pcWriteBuffer, (char*) pcOK0Message);
	}

	/* There is no more data to return after this single string, so return
	 pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/


/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
