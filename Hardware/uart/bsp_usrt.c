# include "./uart/bsp_uart.h"

/*调试串口配置*/
void DEBUG_UART_Config(void){

    GPIO_InitTypeDef    GPIO_InitStructure;
    USART_InitTypeDef    USART_InitStructure;

    /*第一步：初始化GPIO*/
    /*打开GPIO 端口的时钟*/
    DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);

    /*将UART Tx 的GPIO 配置为推挽复用模式*/
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
    /*复用模式 推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    /*将UART Rx 的GPIO 配置为浮空输入模式*/
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
    /*浮空输入模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);


    /*第二步：配置串口的初始化结构体*/
    /*打开串口外设时钟*/
    DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
    
    // 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	// 配置NVIC优先级
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
    /*第三步：使能串口*/
    USART_Cmd(DEBUG_USARTx, ENABLE);	    
}

// 自定义串口发送函数
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/*重定向c 库函数printf 到串口，重定向后可使用printf*/
int fputc(int ch, FILE *f){

    /*发送一个字节数据到串口*/
    USART_SendData(DEBUG_USARTx, (uint8_t) ch);

    /*等待发送完毕*/
    while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);

    return (ch);
}

/*重定向c 库函数scanf 到串口，重写向后使用scanf ， getchar 等函数*/
int fgetc(FILE* f){

    /*等待串口输入数据*/
    while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(DEBUG_USARTx);
}
