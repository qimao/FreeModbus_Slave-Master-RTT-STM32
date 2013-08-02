/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : STM32F103X RT-Thread 0.3.1 USB-CDC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#define  BSP_MODULE

#include <bsp.h>
#include <rthw.h>
#include <rtthread.h>
/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/** This function will initial STM32 board**/
void rt_hw_board_init()
{
	BSP_Init();
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the RCC.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void RCC_Configuration(void)
{
    //�����Ǹ���ģ�鿪��ʱ��
    //����GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | \
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | \
						   RCC_APB2Periph_GPIOE ,
                           ENABLE);
    //����AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//����ADCת��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); //9M
    //����USART1 USART4
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    //����DMAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMAʱ��
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configer NVIC
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
    // Set the Vector Table base location at 0x20000000
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  // VECT_TAB_FLASH  
    // Set the Vector Table base location at 0x08000000
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

    //����NVIC���ȼ�����ΪGroup2��0-3��ռʽ���ȼ���0-3����Ӧʽ���ȼ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //����1�����жϴ�    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);  

}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/***************�������IO��ʼ��*********************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;  //������ LED1  LED2
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //������
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

	/*************��������IO��ʼ��*********************/	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_6;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
    

	/****************USART1��ʼ��************************/
	//USART1_TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //USART1_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}




/*******************************************************************************
* Function Name  : USART1_Configuration
* Description    : NUSART1����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void USART1_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܴ���1�Ľ����ж�	
 
    USART_Cmd(USART1, ENABLE);
}


//*******************��ʼ���������Ź�*************************************
//��������: void IWDG_Configuration(void) 
//��    ������ʼ���������Ź�
//��ڲ�������
//���ڲ�������
//��    ע����Ƶ����=4*2^prer.�����ֵֻ����256!ʱ�����(���):Tout=40K/((4*2^prer)*rlr)ֵ	 2S��ʱ
//Editor��liuqh 2013-1-16  Company: BXXJS
//*******************************************************************
static void IWDG_Configuration(void) 
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//ʹ�ܶ�IWDG->PR��IWDG->RLR��д
	IWDG_SetPrescaler(IWDG_Prescaler_64);//64��Ƶ
	IWDG_SetReload(1300);
	IWDG_ReloadCounter();
	IWDG_Enable();		
}
//*******************ι�������Ź�*************************************
//��������: void IWDG_Feed(void)
//��    ������ʼ���������Ź�
//��ڲ�������
//���ڲ�����prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)��rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//��    ע����Ƶ����=4*2^prer.�����ֵֻ����256!ʱ�����(���):Tout=40K/((4*2^prer)*rlr)ֵ
//Editor��liuqh 2013-1-16  Company: BXXJS
//*******************************************************************

void IWDG_Feed(void)
{
	IWDG_ReloadCounter();//reload											   
}


/*******************************************************************************
 * Function Name  : SysTick_Configuration
 * Description    : Configures the SysTick for OS tick.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void  SysTick_Configuration(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	rt_uint32_t         cnts;

	RCC_GetClocksFreq(&rcc_clocks);

	cnts = (rt_uint32_t)rcc_clocks.HCLK_Frequency / RT_TICK_PER_SECOND;

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}
/**
 * This is the timer interrupt service routine.
 *
 */
void rt_hw_timer_handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
	RCC_Configuration();
	NVIC_Configuration();
	SysTick_Configuration();
	GPIO_Configuration();	
	USART1_Configuration();
// 	IWDG_Configuration();
}
//****************************����ʱ����********************************
//��������: uint8_t AvoidTimeout(uint32_t TimeOfTimeout,uint32_t Period,uint8_t (*DetectCondition)())
//��    ������TimeOfTimeoutʱ���ڣ�ÿPeriodʱ����һ��DetectCondition()���ص�ֵ�Ƿ���Ч
//��ڲ�����TimeOfTimeout������ʱ��ʱ�䣨��λ��systick��
//          Period       ��ÿPeriodʱ����һ�Σ���ʱ�����ӣ���λ��systick��
//          (*DetectCondition)()���������������ConditionValue���������㣬��������������ʱPeriodʱ��������
//          ConditionValue      ������������ֵ
//���ڲ�����0����TimeOfTimeoutʱ���ڣ���⵽��������
//          1����TimeOfTimeoutʱ���ڣ�û�м�⵽��������
//��    ע��Editor��Armink 2012-03-09    Company: BXXJS
//**********************************************************************
uint8_t AvoidTimeout(uint32_t TimeOfTimeout,uint32_t Period,uint8_t (*DetectCondition)(),uint8_t ConditionValue)
{
	uint32_t LastTimeLocal, CurTimeLocal;
	uint8_t ConditionValueLocal;
	LastTimeLocal = rt_tick_get();
	CurTimeLocal  =  LastTimeLocal;
	while(CurTimeLocal - LastTimeLocal < TimeOfTimeout)
	{	 
		CurTimeLocal = rt_tick_get();
		ConditionValueLocal = DetectCondition();
		if (ConditionValueLocal == ConditionValue) return 0;
		rt_thread_delay(Period);
	}	
	return 1;
} 


//************************************��ʱ����**************************************
//��������: void Delay(vu32 nCount)
//��ڲ�����nCount ����ʱ�����У�ѭ���Ĵ���
//���ڲ�������
//��    ע��Editor��Armink 2011-03-18    Company: BXXJS
//**********************************************************************************
void Delay(vu32 nCount)
{
  for(; nCount!= 0;nCount--);
}
