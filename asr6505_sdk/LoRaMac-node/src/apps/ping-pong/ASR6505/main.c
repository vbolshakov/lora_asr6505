/*!
 * \file      main.c
 *
 * \brief     Ping-Pong implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#include <stdio.h>
#include <string.h>
#include "stm8l15x_gpio.h"              //必须置顶，否则提示错误？？？？？？
#include "board.h"
#include "gpio.h"
#include "delay.h"
#include "timer.h"
#include "radio.h"
#include  "asr6505evb-board.h"
#include "sx126x.h"

#include "adc.h"
//#include "e2prom.h"
#include "ds18b20.h"

#if defined( REGION_AS923 )

#define RF_FREQUENCY                                923000000 // Hz

#elif defined( REGION_AU915 )

#define RF_FREQUENCY                                915000000 // Hz

#elif defined( REGION_CN470 )

#define RF_FREQUENCY                                470300000 // Hz
//#define RF_FREQUENCY                                434000000 // Hz

#elif defined( REGION_CN779 )

#define RF_FREQUENCY                                779000000 // Hz

#elif defined( REGION_EU433 )

#define RF_FREQUENCY                                433000000 // Hz

#elif defined( REGION_EU868 )

#define RF_FREQUENCY                                868000000 // Hz

#elif defined( REGION_KR920 )

#define RF_FREQUENCY                                920000000 // Hz

#elif defined( REGION_IN865 )

#define RF_FREQUENCY                                865000000 // Hz

#elif defined( REGION_US915 )

#define RF_FREQUENCY                                915000000 // Hz

#elif defined( REGION_US915_HYBRID )

#define RF_FREQUENCY                                915000000 // Hz

#else
    #error "Please define a frequency band in the compiler options."
#endif

#define TX_OUTPUT_POWER                             22        // dBm

#if defined( USE_MODEM_LORA )

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       10         // [SF7..SF12]
#define LORA_CODINGRATE                             2         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

#elif defined( USE_MODEM_FSK )

#define FSK_FDEV                                    25000     // Hz
#define FSK_DATARATE                                50000     // bps
#define FSK_BANDWIDTH                               1000000   // Hz >> DSB in sx126x
#define FSK_AFC_BANDWIDTH                           1000000   // Hz
#define FSK_PREAMBLE_LENGTH                         5         // Same for Tx and Rx
#define FSK_FIX_LENGTH_PAYLOAD_ON                   false

#else
    #error "Please define a modem in the compiler options."
#endif

typedef enum
{
    LOWPOWER,
    RX,
    RX_TIMEOUT,
    RX_ERROR,
    TX,
    TX_TIMEOUT
}States_t;

#define RX_TIMEOUT_VALUE                            5000
#define BUFFER_SIZE                                 64 // Define the payload size here

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];

volatile States_t State = LOWPOWER;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void );

void SendDataTimerEvent( void );

/**
 * Main application entry point.
 */
TimerEvent_t SendDataTimer;
bool isMaster = true;
uint8_t buf[] = "ars6505 send0";
uint16_t tx_count = 0;
uint16_t rx_count = 0;
int main( void )
{

    
    // Target board initialization
    BoardInitMcu( );
    BoardInitPeriph( );
    printf("PingPong test Start!\r\n");
 //   while(1);
 //   halt();
    
//    adc_init();
//    vol_measure();
//    printf("vrifint=1.224v, adc1_value=%d, pwr_vol=%f", adc1_value, VoltageValue);
   
    tempc=DS18B20_Get_Temp();
//    
//    for(uint8_t j=0; j<10; j++)
//      e2prom_byte_rxtx(0x1000+j, j);
    
 /* TX led init: GPIO set in output */
 //   GPIO_Init(LED_TX_PORT, LED_TX_PIN, GPIO_Mode_Out_PP_High_Fast);
//    GPIO_Init(LED_RX_PORT, LED_RX_PIN, GPIO_Mode_Out_PP_Low_Slow);
 
/* TX led init: GPIO set in output */
    GPIO_Init(LED_RX_PORT, LED_RX_PIN, GPIO_Mode_Out_PP_High_Fast);
     GPIO_LOW(LED_RX_PORT, LED_RX_PIN);
     
//    GPIO_Init(GPIOF, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);
//    GPIO_LOW(GPIOF, GPIO_Pin_4);
//    GPIO_HIGH(GPIOF, GPIO_Pin_4);

    // Radio initialization
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;

    Radio.Init( &RadioEvents );

    Radio.SetChannel( RF_FREQUENCY );


#if defined( USE_MODEM_LORA )

    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

#elif defined( USE_MODEM_FSK )

    Radio.SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                                  FSK_DATARATE, 0,
                                  FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                                  true, 0, 0, 0, 3000 );

    Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                                  0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                                  0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                                  0, 0,false, true );

#else
    #error "Please define a frequency band in the compiler options."
#endif

 //   Radio.Rx( RX_TIMEOUT_VALUE );
 //   Radio.Send(buf, 12);

//    while( 1 )
//    {
//#if 0      
//        switch( State )
//        {
//        case RX:
//	    GPIO_HIGH(LED_RX_PORT, LED_RX_PIN);
//            printf("received:%s, rssi=%d, snr=%d, rx_count=%d\n", Buffer, RssiValue, SnrValue, ++rx_count);
// //           Radio.SetRxDutyCycle( RX_TIME, SLEEP_TIME);
//	    Radio.Rx(5000);	    
//            State = LOWPOWER;
//	    GPIO_LOW(LED_RX_PORT, LED_RX_PIN);
//	    memset(Buffer, 0, BufferSize);
//	    BufferSize = 0;
//            break;
//            
//        case TX:
//            Radio.Rx( RX_TIMEOUT_VALUE );
//            State = LOWPOWER;
//            break;
//        case RX_TIMEOUT:
//        case RX_ERROR:
//            Radio.Rx(5000);
//            State = LOWPOWER;
//            break;
//        case TX_TIMEOUT:       
//            Radio.Rx( RX_TIMEOUT_VALUE );
//            State = LOWPOWER;
//            break;
//        case LOWPOWER:
//        default:
//            // Set low power
//            break;
//        }
//#else
//	
//        switch( State )
//        {
//	case RX:    
//            State = LOWPOWER;
//            break;
//            
//        case TX:
//	    GPIO_HIGH(LED_RX_PORT, LED_TX_PIN);
//            Radio.Send(buf, 12);
//            State = LOWPOWER;
//	    GPIO_LOW(LED_RX_PORT, LED_TX_PIN);
//            break;
//        case RX_TIMEOUT:
//        case RX_ERROR:
//            State = LOWPOWER;
//            break;
//        case TX_TIMEOUT: 
//	    GPIO_HIGH(LED_RX_PORT, LED_RX_PIN);
//            Radio.Send(buf, 12);
//
//            State = LOWPOWER;
//	    GPIO_LOW(LED_RX_PORT, LED_RX_PIN);
//            break;
//        case LOWPOWER:
//        default:
//            // Set low power
//            break;
//        }
//#endif
//        TimerLowPowerHandler( );
//        // Process Radio IRQ
//        Radio.IrqProcess( );
//    }
    if(isMaster){
    	TimerInit(&SendDataTimer, &SendDataTimerEvent);
	TimerSetValue(&SendDataTimer, 10000);
	TimerStart(&SendDataTimer);
	DelayMs(1);
	Radio.Send(buf, 13);
    }
    else{
    	Radio.Rx(5000);
    }
    while(1){
    	TimerLowPowerHandler( ); 
 //       halt();
	Radio.IrqProcess( );
    }
}
    
void OnTxDone( void )
{
    if(!isMaster)
        GPIO_LOW(LED_RX_PORT, LED_RX_PIN);//slave led off after data sending finished
    tx_count++;
    printf("OnTxDone, tx_count:%d\n", tx_count);
    Radio.Sleep();
//    State = TX;
    if(!isMaster)
        Radio.Rx(5000);
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep();
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    RssiValue = rssi;
    SnrValue = snr;
//    State = RX;
    if(isMaster){
    	printf("master received:%s, size=%d, rssi=%d, snr=%d, rx_count=%d\n", Buffer, BufferSize, RssiValue, SnrValue, ++rx_count);
	Radio.Sleep();
    }
    else{
    	printf("slave received:%s, size=%d, rssi=%d, snr=%d, rx_count=%d\n", Buffer, BufferSize, RssiValue, SnrValue, ++rx_count);
        GPIO_HIGH(LED_RX_PORT, LED_RX_PIN);//slave led on while sending data
	Radio.Send(Buffer, BufferSize);
    }
}

void OnTxTimeout( void )
{
    printf("OnTxTimeout\n");
//    Radio.Sleep();
    Radio.Sleep();
//    SX126xCheckDeviceReady( );
 //   State = TX_TIMEOUT;
    Radio.Rx(5000);
    
}

void OnRxTimeout( void )
{
    printf("OnRxTimeout\r\n");
    Radio.Sleep();
 //   State = RX_TIMEOUT;
    Radio.Rx(5000);
}

void OnRxError( void )
{
    printf("OnRxError\n");
    Radio.Sleep();
 //   State = RX_ERROR;
    Radio.Rx(5000);
}

void SendDataTimerEvent( void ){
    GPIO_HIGH(LED_RX_PORT, LED_RX_PIN);
    TimerStop(&SendDataTimer);
    buf[12]++;
    if(buf[12] >= 58)
      buf[12] = '0';
    tempc=DS18B20_Get_Temp();
//    buf[11] = tempc;
    uint8_t u8temp = (uint8_t) tempc;
    buf[12] = u8temp + 48;
    Radio.Send(buf, 13);
    TimerStart(&SendDataTimer);
    GPIO_LOW(LED_RX_PORT, LED_RX_PIN);
}
