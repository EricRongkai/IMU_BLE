#define  Rled_on    GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define  Rled_off   GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define  Gled_on    GPIO_ResetBits(GPIOB, GPIO_Pin_14)
#define  Gled_off   GPIO_SetBits(GPIOB, GPIO_Pin_14)
#define  Bled_on    GPIO_ResetBits(GPIOB, GPIO_Pin_15)
#define  Bled_off   GPIO_SetBits(GPIOB, GPIO_Pin_15)

void LED_Config(void);
