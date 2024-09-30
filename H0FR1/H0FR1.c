/*
 BitzOS (BOS) V0.3.6 - Copyright (C) 2017-2024 Hexabitz
 All rights reserved

 File Name     : H0FR1.c
 Description   : Source code for module H0FR1.
 	 	 	 	 (Description_of_module)

(Description of Special module peripheral configuration):
>>
>>
>>

 */

/* Includes ------------------------------------------------------------------*/
#include "BOS.h"
#include "H0FR1_inputs.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* Define UART variables */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart6;

/* Exported variables */
extern FLASH_ProcessTypeDef pFlash;
extern uint8_t numOfRecordedSnippets;

/* Local functions */
static Module_Status PollingSleepCLISafe(uint32_t period, long Numofsamples);

/* Module exported parameters ------------------------------------------------*/
module_param_t modParam[NUM_MODULE_PARAMS] ={{.paramPtr = NULL, .paramFormat =FMT_FLOAT, .paramName =""}};

/* Private variables ---------------------------------------------------------*/
TimerHandle_t xTimerSwitch = NULL;
SwitchState_t SwitchState = STATE_OFF, SwitchOldState = STATE_ON; // Initial state value to solid switch

/* Private function prototypes -----------------------------------------------*/
void SwitchTimerCallback(TimerHandle_t xTimerSwitch);
void ExecuteMonitor(void);
void FLASH_Page_Eras(uint32_t Addr );

/* Create CLI commands --------------------------------------------------------*/
portBASE_TYPE OnCommand(int8_t *pcWriteBuffer,size_t xWriteBufferLen,const int8_t *pcCommandString);
portBASE_TYPE OffCommand(int8_t *pcWriteBuffer,size_t xWriteBufferLen,const int8_t *pcCommandString);
portBASE_TYPE ToggleCommand(int8_t *pcWriteBuffer,size_t xWriteBufferLen,const int8_t *pcCommandString);

/* CLI command structure : on */
const CLI_Command_Definition_t OnCommandDefinition = {
		(const int8_t*) "on", /* The command string to type. */
		(const int8_t*) "on:\r\n Turn solid state Switch ON with a timeout (ms) (1st par.). Use 'inf' to turn on constantly\r\n\r\n",
		OnCommand, /* The function to run. */
		1 /* One parameter is expected. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : off */
const CLI_Command_Definition_t OffCommandDefinition = {
		(const int8_t*) "off", /* The command string to type. */
		(const int8_t*) "off:\r\n Turn solid state Switch OFF\r\n\r\n",
		OffCommand, /* The function to run. */
		0 /* No parameters are expected. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : toggle */
const CLI_Command_Definition_t ToggleCommandDefinition = {
		(const int8_t*) "toggle", /* The command string to type. */
		(const int8_t*) "toggle:\r\n Toggle solid state Switch\r\n\r\n",
		ToggleCommand, /* The function to run. */
		0 /* No parameters are expected. */
};

/*-----------------------------------------------------------*/

/* CLI command structure : sample */

/* CLI command structure : stop */


/* CLI command structure : unit */


/* CLI command structure : rate */



/* CLI command structure : calibration */


/* CLI command structure : zerocal */


/*-----------------------------------------------------------*/
/* CLI command structure : weight */


/*-----------------------------------------------------------*/
/* CLI command structure : weight */


/* ---------------------------------------------------------------------
 |							 Private Functions	                	   |
 ----------------------------------------------------------------------- 
 */

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow : 
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 48000000
 *            HCLK(Hz)                       = 48000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 1
 *            HSE Frequency(Hz)              = 8000000
 *            PREDIV                         = 1
 *            PLLMUL                         = 6
 *            Flash Latency(WS)              = 1
 * @param  None
 * @retval None
 */
void SystemClock_Config(void){
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI
			| RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
	RCC_OscInitStruct.PLL.PLLN = 12;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

	/** Initializes the peripherals clocks
	 */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_USART2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM1;
	PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLKSOURCE_PCLK1;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	__SYSCFG_CLK_ENABLE();

	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	
}

/*-----------------------------------------------------------*/


/* --- Save array topology and Command Snippets in Flash RO --- 
 */
uint8_t SaveToRO(void){
	BOS_Status result =BOS_OK;
	HAL_StatusTypeDef FlashStatus =HAL_OK;
	uint16_t add =8;
    uint16_t temp =0;
	uint8_t snipBuffer[sizeof(snippet_t) + 1] ={0};
	
	HAL_FLASH_Unlock();
	/* Erase RO area */
	FLASH_PageErase(FLASH_BANK_1,RO_START_ADDRESS);
	FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
	FLASH_PageErase(FLASH_BANK_1,RO_MID_ADDRESS);
	//TOBECHECKED
	FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
	if(FlashStatus != HAL_OK){
		return pFlash.ErrorCode;
	}
	else{
		/* Operation is completed, disable the PER Bit */
		CLEAR_BIT(FLASH->CR,FLASH_CR_PER);
	}
	
	/* Save number of modules and myID */
	if(myID){
		temp =(uint16_t )(N << 8) + myID;
		//HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,RO_START_ADDRESS,temp);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,RO_START_ADDRESS,temp);
		//TOBECHECKED
		FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
		if(FlashStatus != HAL_OK){
			return pFlash.ErrorCode;
		}
		else{
			/* If the program operation is completed, disable the PG Bit */
			CLEAR_BIT(FLASH->CR,FLASH_CR_PG);
		}
		
		/* Save topology */
		for(uint8_t i =1; i <= N; i++){
			for(uint8_t j =0; j <= MaxNumOfPorts; j++){
				if(array[i - 1][0]){

          	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,RO_START_ADDRESS + add,array[i - 1][j]);
				 //HALFWORD 	//TOBECHECKED
					FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
					if(FlashStatus != HAL_OK){
						return pFlash.ErrorCode;
					}
					else{
						/* If the program operation is completed, disable the PG Bit */
						CLEAR_BIT(FLASH->CR,FLASH_CR_PG);
						add +=8;
					}
				}
			}
		}
	}
	
	// Save Command Snippets
	int currentAdd = RO_MID_ADDRESS;
	for(uint8_t s =0; s < numOfRecordedSnippets; s++){
		if(snippets[s].cond.conditionType){
			snipBuffer[0] =0xFE;		// A marker to separate Snippets
			memcpy((uint32_t* )&snipBuffer[1],(uint8_t* )&snippets[s],sizeof(snippet_t));
			// Copy the snippet struct buffer (20 x numOfRecordedSnippets). Note this is assuming sizeof(snippet_t) is even.
			for(uint8_t j =0; j < (sizeof(snippet_t)/4); j++){
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,currentAdd,*(uint64_t* )&snipBuffer[j*8]);
				//HALFWORD
				//TOBECHECKED
				FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
				if(FlashStatus != HAL_OK){
					return pFlash.ErrorCode;
				}
				else{
					/* If the program operation is completed, disable the PG Bit */
					CLEAR_BIT(FLASH->CR,FLASH_CR_PG);
					currentAdd +=8;
				}
			}
			// Copy the snippet commands buffer. Always an even number. Note the string termination char might be skipped
			for(uint8_t j =0; j < ((strlen(snippets[s].cmd) + 1)/4); j++){
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,currentAdd,*(uint64_t* )(snippets[s].cmd + j*4 ));
				//HALFWORD
				//TOBECHECKED
				FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
				if(FlashStatus != HAL_OK){
					return pFlash.ErrorCode;
				}
				else{
					/* If the program operation is completed, disable the PG Bit */
					CLEAR_BIT(FLASH->CR,FLASH_CR_PG);
					currentAdd +=8;
				}
			}
		}
	}
	
	HAL_FLASH_Lock();
	
	return result;
}

/* --- Clear array topology in SRAM and Flash RO --- 
 */
uint8_t ClearROtopology(void){
	// Clear the array 
	memset(array,0,sizeof(array));
	N =1;
	myID =0;
	
	return SaveToRO();
}
/*-----------------------------------------------------------*/

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

/*-----------------------------------------------------------*/

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

/* --- H0FR1 module initialization.
 */
void Module_Peripheral_Init(void){

	/* Array ports */
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	//	MX_USART4_UART_Init();
	MX_USART5_UART_Init();
	MX_USART6_UART_Init();
	/* Switch GPIO */
	SwitchInit();
	/* Create a timeout software timer for Switch_on() API */
	xTimerSwitch =xTimerCreate("SwitchTimer",pdMS_TO_TICKS(1000),pdFALSE,(void* )1,SwitchTimerCallback);
	//Circulating DMA Channels ON All Module
	for (int i = 1; i <= NumOfPorts; i++) {
		if (GetUart(i) == &huart1) {
			index_dma[i - 1] = &(DMA1_Channel1->CNDTR);}
		else if (GetUart(i) == &huart2) {
			index_dma[i - 1] = &(DMA1_Channel2->CNDTR);}
		else if (GetUart(i) == &huart3) {
			index_dma[i - 1] = &(DMA1_Channel3->CNDTR);}
		else if (GetUart(i) == &huart4) {
			index_dma[i - 1] = &(DMA1_Channel4->CNDTR);}
		else if (GetUart(i) == &huart5) {
			index_dma[i - 1] = &(DMA1_Channel5->CNDTR);}
		else if (GetUart(i) == &huart6) {
			index_dma[i - 1] = &(DMA1_Channel6->CNDTR);}
	}
}

/*-----------------------------------------------------------*/
/* --- H0FR1 message processing task.
 */

Module_Status Module_MessagingTask(uint16_t code, uint8_t port, uint8_t src, uint8_t dst, uint8_t shift) {

	Module_Status result = H0FR1_OK;
	uint32_t temp32 = 0;

	switch (code) {
	case CODE_H0FR1_ON:
		temp32 = ((uint32_t) cMessage[port - 1][shift] << 0) + ((uint32_t) cMessage[port - 1][1 + shift] << 8)+ ((uint32_t) cMessage[port - 1][2 + shift] << 16)+ ((uint32_t)cMessage[port - 1][3 + shift] << 24);
		OutputOn(temp32);
		break;

	case CODE_H0FR1_OFF:
		OutputOff();
		break;

	case CODE_H0FR1_TOGGLE:
		OutputToggle();
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
	FreeRTOS_CLIRegisterCommand(&OnCommandDefinition);
	FreeRTOS_CLIRegisterCommand(&OffCommandDefinition);
	FreeRTOS_CLIRegisterCommand(&ToggleCommandDefinition);
}

/* --- Get the port for a given UART. 
 */
uint8_t GetPort(UART_HandleTypeDef *huart){

	if(huart->Instance == USART6)
		return P1;
	else if(huart->Instance == USART2)
		return P2;
	else if(huart->Instance == USART5)
		return P3;
	else if(huart->Instance == USART3)
		return P4;
	else if(huart->Instance == USART1)
		return P5;
	
	return 0;
}

/* --- Register this module CLI Commands
 */


/*-----------------------------------------------------------*/


/* Module special task function (if needed) */
//void TOFTask(void *argument){
//
//
//	/* Infinite loop */
//	for(;;){
//
//
//		taskYIELD();
//	}
//
//}

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/


/* -----------------------------------------------------------------------

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
/* --- Switch timer callback ---*/
void SwitchTimerCallback(TimerHandle_t xTimerSwitch){
	/* The solid state switched off after elapsed time */
	OutputOff();
}
/* -----------------------------------------------------------------------
 |								  User Function                           |
/* -----------------------------------------------------------------------

/****************************************************************************
* @brief  Turns on the solid state switch and sets a timeout to turn it off.
* @param  timeout: Duration in milliseconds for which the switch should remain on.
* @retval Module_Status.
****************************************************************************/
Module_Status OutputOn(uint32_t timeout) {

	if (timeout < 0U || timeout > portMAX_DELAY)
		return H0FR1_ERR_WrongParams;
	/* Turn on */
	HAL_GPIO_WritePin(SWITCH_PORT, SWITCH_PIN, GPIO_PIN_SET);

	/* Timeout */
	if (timeout != portMAX_DELAY) {
		/* Stop (Reset) the timer if it's already running */
		if (xTimerIsTimerActive(xTimerSwitch))
			xTimerStop(xTimerSwitch, 100);
		/* Update timer timeout - This also restarts the timer */
		xTimerChangePeriod(xTimerSwitch, pdMS_TO_TICKS(timeout), 100);
	}
	/* Update Switch state */
	SwitchState = STATE_ON;
	SwitchOldState = SwitchState;

	return H0FR1_OK;
}
/*-----------------------------------------------------------*/
/****************************************************************************
* @brief  Turns off the solid state switch.
* @retval Module_Status.
****************************************************************************/
Module_Status OutputOff(void) {

	/* Turn off */
	HAL_GPIO_WritePin(SWITCH_PORT, SWITCH_PIN, GPIO_PIN_RESET);

	/* Update Switch state */
	SwitchState = STATE_OFF;

	return H0FR1_OK;
}
/*-----------------------------------------------------------*/
/****************************************************************************
* @brief  Toggle the solid state Switch.
* @retval Module_Status.
****************************************************************************/
Module_Status OutputToggle(void) {
	Module_Status result;

	if (SwitchState) {
		/*if the state machine was ON turned it OFF*/
		result = OutputOff();
		if (result != H0FR1_OK)
			return H0FR1_ERROR;
	} else {
		if (SwitchOldState == STATE_ON){
			result = OutputOn(portMAX_DELAY);
			if ( result != H0FR1_OK)
				return H0FR1_ERROR;
		}
	}
	return H0FR1_OK;
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
 |								Commands							      |
   -----------------------------------------------------------------------
*/
portBASE_TYPE OnCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString) {

	Module_Status result = H0FR1_OK;
	int8_t *pcParameterString1;
	portBASE_TYPE xParameterStringLength1 = 0;
	uint32_t timeout = 0;
	static const int8_t *pcOKMessage = (int8_t*) "Solid state Switch is turned ON with timeout %d ms\r\n";
	static const int8_t *pcOKMessageInf = (int8_t*) "Solid state Switch is turned ON without timeout\r\n";

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

	if (!strcmp((char*) pcParameterString1, "inf") || !strcmp((char*) pcParameterString1, "INF"))
		timeout = portMAX_DELAY;
	else
		timeout = (uint32_t) atol((char*) pcParameterString1);

	result = OutputOn(timeout);

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
portBASE_TYPE OffCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString) {

	Module_Status result = H0FR1_OK;
	static const int8_t *pcMessage = (int8_t*) "Solid state Switch is turned OFF\r\n";

	/* Remove compile time warnings about unused parameters, and check the
	 write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	 write buffer length is adequate, so does not check for buffer overflows. */
	(void) pcCommandString;
	(void) xWriteBufferLen;
	configASSERT(pcWriteBuffer);

	result = OutputOff();

	/* Respond to the command */
	if (result == H0FR1_OK) {
		strcpy((char*) pcWriteBuffer, (char*) pcMessage);
	}
	/* There is no more data to return after this single string, so return
	 pdFALSE. */
	return pdFALSE;
}
/*-----------------------------------------------------------*/
portBASE_TYPE ToggleCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString) {

	Module_Status result = H0FR1_OK;
	static const int8_t *pcOK1Message = (int8_t*) "Solid state Switch is turned ON\r\n";
	static const int8_t *pcOK0Message = (int8_t*) "Solid state Switch is turned OFF\r\n";

	/* Remove compile time warnings about unused parameters, and check the
	 write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	 write buffer length is adequate, so does not check for buffer overflows. */
	(void) xWriteBufferLen;
	configASSERT(pcWriteBuffer);

	result = OutputToggle();

	/* Respond to the command */
	if (result == H0FR1_OK) {
		if (SwitchState) {
			strcpy((char*) pcWriteBuffer, (char*) pcOK1Message);
		} else {
			strcpy((char*) pcWriteBuffer, (char*) pcOK0Message);
		}
	}
	/* There is no more data to return after this single string, so return
	 pdFALSE. */
	return pdFALSE;
}

/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
