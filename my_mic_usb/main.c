//STM32F4 USB MICROPHONE
//by ILIASAM

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4_discovery_audio_codec.h"
#include "usbd_audio_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_audio_core.h"
#include "waverecorder.h"
#include "stm32f4xx_spi.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

__IO uint8_t UserButtonPressed = 0;

__IO uint32_t TimingDelay;

/* Buffer for 24-bit audio data */
// int32_t audiodata[FRAME_SIZE/4];  // Buffer for 24-bit audio data (3 bytes per sample)

extern int32_t RecBuf0[MIC_FILTER_RESULT_LENGTH]; // buffer for filtered PCM data from MIC
extern int32_t RecBuf1[MIC_FILTER_RESULT_LENGTH]; // buffer for filtered PCM data from MIC
extern uint8_t buffer_ready;

void init_audio_dac(void);

int main(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  
  /* Initialize LEDs and User_Button on STM32F4-Discovery --------------------*/
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO); 
  
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  
  RCC_GetClocksFreq(&RCC_Clocks);
  
  /* Initialize audio with 24-bit resolution and 48kHz sampling rate */
  EVAL_AUDIO_Init(OUTPUT_DEVICE_HEADPHONE, 86, I2S_AudioFreq_48k);
  
  USBD_Init(&USB_OTG_dev,      
            USB_OTG_FS_CORE_ID,
            &USR_desc, 
            &AUDIO_cb, 
            &USR_cb);
  
  /* Initialize and start microphone recording with 24-bit resolution */
  WaveRecorderInit(I2S_AudioFreq_48k, AUDIO_BIT_RESOLUTION, 1);
  simple_rec_start();
  
  while(1)
  {
    asm("nop");
  }
}


void delay_ms(uint32_t ms)
{
  volatile uint32_t nCount;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);
  nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
  for (; nCount!=0; nCount--);
}


void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}


void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

//speaker CallBack - optional
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t* pBuffer, uint32_t Size)
{
//   uint8_t i;
//   //called after speaker data TransferComplete
//   asm("nop");
// #ifdef AUDIO_MAL_MODE_NORMAL
//   for (i=0;i<(MIC_FILTER_RESULT_LENGTH*2);i++)
//   {
//     if (buffer_ready == 1) {
//        audiodata[i] = RecBuf1[i>>1];
//     } else { 
// 	RecBuf0[i>>1];
//     }//make pseudo-stereo
//   }
  
//   EVAL_AUDIO_Play((uint16_t*)(&audiodata[0]),MIC_FILTER_RESULT_LENGTH*2*2);
// #endif
}
