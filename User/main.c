# include "main.h"

extern float Pitch,Roll,Yaw; 

uint8_t main_flag = 0;
float Angle_Balance; 

uint16_t Data1;

int main(void){
	
		Car_Init();
		Ultrasound_Servo_Init();
    OLED_Init();
    DEBUG_UART_Config();
    Servo_Init();
		Ultrasound_Init();
	  Infrared_Init();
	
	  OLED_ShowString(1, 1, "RightHand: X-Y-Z");
    OLED_ShowString(2, 1, "Pitch: ");
    OLED_ShowString(3, 1, "Roll: ");
		OLED_ShowString(4, 1, "Yaw: ");
	
		
    while(1){

        Read_DMP();
				
        Servo_SetAngle1(90+Pitch);
        Servo_SetAngle2(90+Roll);
        if (main_flag){
            OLED_Showfloat(2, 7, Pitch);
            OLED_Showfloat(3, 8, Roll);
						OLED_Showfloat(4, 6, Yaw);
            main_flag = 0;
        }
			
								// Ñ­¼£ÅÐ¶¨
				
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
		{
			Go_Ahead();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1){
			Car_Stop();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1){
				Self_Right();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0){
				Turn_Right();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1){
				Turn_Right();
			}
			else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0){
				Turn_Left();
			}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0){
				Self_Left();
			}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0){
				Turn_Left();
			}
	
				
    }
}

// ´®¿ÚÖÐ¶Ïº¯Êý

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		Data1=USART_ReceiveData(USART1);
		if(Data1==0x30)Car_Stop();
		if(Data1==0x31)Go_Ahead();
		if(Data1==0x32)Go_Back();
		if(Data1==0x33)Turn_Left();
		if(Data1==0x34)Turn_Right();
		if(Data1==0x35)Self_Left();
		if(Data1==0x36)Self_Right();
		if(Data1==0x37)Servo_SetAngle(0);
		if(Data1==0x38)Servo_SetAngle(90);
		if(Data1==0x39)Servo_SetAngle(180);
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

		/*
		//±ÜÕÏÄ£Ê½
		if(Data1==0x40){
			while (1)
	{
		Go_Ahead();
		uint16_t a = Test_Distance();
		Serial_SendNumber(a,3);
		if(a<15){
			Car_Stop();
			Servo_SetAngle(0);	delay_ms(1000);
			uint16_t b= Test_Distance();
		
			Serial_SendNumber(b,3);
			if(b>15){
				Servo_SetAngle(90);
				delay_ms(1000);
				Self_Right();
				delay_ms(1000);
				Go_Ahead();
			
			}
			else {
				Servo_SetAngle(180);
				delay_ms(1000);
				uint16_t c= Test_Distance();
				Serial_SendNumber(c,3);
				if(c>15){	
					Servo_SetAngle(90);
					delay_ms(1000);
					Self_Left();
					delay_ms(1000);
					Go_Ahead();
				}else{
					Servo_SetAngle(90);
					while(1){};
				}
			}
		}
		delay_ms(100);
		// ½â³ý±ÜÕÏ
		if(Data1==0x41) break;
	}

	}
				// Ñ­¼£Ä£Ê½
		if(Data1==0x42){
			while(1){
								// Ñ­¼£ÅÐ¶¨
				
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
		{
			Go_Ahead();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1){
			Car_Stop();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1){
				Self_Right();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0){
				Turn_Right();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1){
				Turn_Right();
			}
			else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0){
				Turn_Left();
			}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0){
				Self_Left();
			}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==1&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0){
				Turn_Left();
			}
			// ½â³ýÑ­¼£
			if(Data1==0x43) break;
			}
		}
			*/
			