/**
 * @file main.c
 *
 * @brief Main function
 */

#include "mcu.h"
#include "tim.h"

// FEITO POR LUCAS HARADA

/*****************************************
 * Private Constant Definitions
 *****************************************/

#define LED_TOGGLE_DELAY_MS 15

#define MAX_ADC_VALUE 4995
#define MAX_PWM_PERIOD 0XFFF
#define MAX_PWM_DC 100

// INPUT -> PA0           X
// OUTPUTS -> PA1 e PA7   x
// ADC4_IN5 -> PB15    ADC X
// TIM8_CH1 -> PC6     PWM X
// TIM6 -> INTERRUPCAO     X

/*****************************************
 * Main Function
 *****************************************/

int main(void)
{
    mcu_init();

    MX_TIM6_Init();
    HAL_TIM_Base_Start(&htim6); // Inicializacao do timer de interrupcao

    MX_TIM8_Init(); // Iniciar a PWM

    MAX_ADC4_Init(); // Criando potenciometro pra regular o Duty Cycle, conversao analogico digital
    HAL_ADC_Start(&hadc4);

    for (;;)
    {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); // Primeira situacao: LED 1 ligado, LED 2 desligado
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // segunda situacao: LED 1 desligado, LED 2 ligado caso nao tenha input
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    uint16_t adc_read; // criacao da variavel adc_read pra armazenamento do ADC
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    __HAL_TIM_SET_AUTORELOAD(&htim8, 1000);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 1000 * 50 / 100);

    adc_read = HAL_ADC_GetValue(&hadc1); // Regular a PWM pelo potenciometro
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, map(adc_read, 0, MAX_ADC_VALUE, 0, MAS_PWM_DC));
}
