/*
 * re7.c
 *
 * Created: 12/7/2019 4:11:55 PM
 * Author : Mahmoud Saad
 */ 

#include "softwareDelay.h"
#include "led.h"
typedef enum 
{
	STOP, GET_READY, GO
}ge_state;

void stateMachine(void)
{
	static ge_state state = STOP;
	switch(state)
	{
		case GO:
		Led_On(LED_1);
		Led_Off(LED_2);
		Led_Off(LED_3);
		state = STOP;
		break;
		
		case STOP:
		Led_Off(LED_1);
		Led_On(LED_2);
		Led_Off(LED_3);
		state = GET_READY;
		break;
		
		case GET_READY:
		Led_Off(LED_1);
		Led_Off(LED_2);
		Led_On(LED_3);
		state = GO;
		break;
		
		default:
		break;
	}
}
int main(void)
{
    Led_Init(LED_1);
	Led_Init(LED_2);
	Led_Init(LED_3);
	Led_Off(LED_1);
	Led_Off(LED_2);
	Led_Off(LED_3);
    while (1) 
    {
		SwDelay_ms(1000);
		stateMachine();
    }
}

