#ifndef _ASR6505EVB_BOARD_H_
#define _ASR6505EVB_BOARD_H_

/* MACROs for SET, RESET or TOGGLE Output port */


#define GPIO_HIGH(a,b) 		a->ODR|=b
#define GPIO_LOW(a,b)		a->ODR&=~b
#define GPIO_TOGGLE(a,b) 	a->ODR^=b

//GPIO3 PE6 LED_TXDONE
#define LED_TX_PORT		GPIOE
#define LED_TX_PIN		GPIO_Pin_6 //GPIO3 STATE

//GPIO2 PC4 LED_RXDONE
#define LED_RX_PORT		GPIOC
#define LED_RX_PIN		GPIO_Pin_4 //GPIO2 WAKE


#endif