#ifndef _RYM6505_BOARD_H_
#define _RYM6505_BOARD_H_

/* MACROs for SET, RESET or TOGGLE Output port */


#define GPIO_HIGH(a,b) 		a->ODR|=b
#define GPIO_LOW(a,b)		a->ODR&=~b
#define GPIO_TOGGLE(a,b) 	a->ODR^=b

//SPI1 MISO_TXDONE
#define LED_TX_PORT		GPIOF
#define LED_TX_PIN		GPIO_Pin_0 //F0

//SPI1 MOSI_RXDONE
#define LED_RX_PORT		GPIOF
#define LED_RX_PIN		GPIO_Pin_1 //F1


#endif
