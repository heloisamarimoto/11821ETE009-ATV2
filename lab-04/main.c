#include <stdlib.h>
#include <stdint.h>

/* AHB1 Base Addresses ******/

#define STM32_RCC_BASE 0x40023800 /* 0x40023800-0x40023bff: Reset
                                    and Clock control RCC */

/* AHB2 Base Addresses ******/

#define STM32_GPIOA_BASE 0x40020000 /* 0x40020000-0x400203FF: GPIO
                                        Port A */



#define STM32_GPIOC_BASE 0x48000800U /* 0x48000800-0x48000bff: GPIO
                                        Port C */

/* Register Offsets *******/

#define STM32_RCC_AHB1ENR_OFFSET 0x0030 /* AHB1 Peripheral Clock enable
                                            register */
#define STM32_GPIO_MODER_OFFSET 0x0000 /* GPIO port mode register */
#define STM32_GPIO_OTYPER_OFFSET 0x0004 /* GPIO port output type register */

#define STM32_GPIO_IDR_OFFSET 0x0010 /* GPIO port output data register */

#define STM32_GPIO_PUPDR_OFFSET 0x000c /* GPIO port pull-up/pull-down
                                            register */
#define STM32_GPIO_BSRR_OFFSET 0x0018 /* GPIO port bit set/reset register */

/* Register Addresses *******/

#define STM32_RCC_AHB1ENR (STM32_RCC_BASE+STM32_RCC_AHB1ENR_OFFSET)
#define STM32_GPIOC_MODER (STM32_GPIOC_BASE+STM32_GPIO_MODER_OFFSET)

#define STM32_GPIOA_MODER (STM32_GPIOA_BASE+STM32_GPIO_MODER_OFFSET) 

#define STM32_GPIOA_PUPDR (STM32_GPIOA_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOA_IDR (STM32_GPIOA_BASE+STM32_GPIO_IDR_OFFSET)


#define STM32_GPIOC_OTYPER (STM32_GPIOC_BASE+STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOC_PUPDR (STM32_GPIOC_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_BSRR (STM32_GPIOC_BASE + STM32_GPIO_BSRR_OFFSET)

/* AHB1 Peripheral Clock enable register */

#define RCC_AHB1ENR_GPIOAEN (1 << 0) /* Bit 2: IO port A clock enable */
#define RCC_AHB1ENR_GPIOCEN (1 << 2) /* Bit 2: IO port C clockenable */

/* GPIO port mode register */

#define GPIO_MODER_SHIFT(n) ((n) << 1) 
#define GPIO_MODER_MASK(n) (3 << GPIO_MODER_SHIFT(n))

#define GPIO_MODER_INPUT (0) /* Input */
#define GPIO_MODER_OUTPUT (1) /* General purpose output mode */
#define GPIO_MODER_ALT (2) /* Alternate mode */
#define GPIO_MODER_ANALOG (3) /* Analog mode */


/* GPIO port output type register */

#define GPIO_OTYPER_PP (0) /* 0=Output push-pull */
#define GPIO_OTYPER_OD (1) /* 1=Output open-drain */
#define GPIO_OT13_SHIFT (13)
#define GPIO_OT13_MASK (1 << GPIO_OT13_SHIFT)

/* GPIO port pull-up/pull-down register */

#define GPIO_PUPDR_NONE (0) /* No pull-up, pull-down */
#define GPIO_PUPDR_PULLUP (1) /* Pull-up */
#define GPIO_PUPDR_PULLDOWN (2) /* Pull-down */

#define GPIO_PUPDR_SHIFT(n) ((n) << 1) 
#define GPIO_PUPDR_MASK(n) (3 << GPIO_PUPDR_SHIFT(n))

/*GPIO port input data register*/

#define GPIO_IDR(n) (1 << (n))

/* GPIO port bit set/reset register */

#define GPIO_BSRR_SET(n) (1 << (n))
#define GPIO_BSRR_RST(n) (1 << (n + 16))
#define LED_DELAY 5000

static const char fw_version[] = {'V', '1', '.', '0'};
static uint32_t led_status;

int main(int argc, char *argv[])
{
    uint32_t reg;

    /* Ponteiros para registradores */

    uint32_t *pRCC_AHB1ENR = (uint32_t *)STM32_RCC_AHB1ENR;
    uint32_t *pGPIOC_MODER = (uint32_t *)STM32_GPIOC_MODER;

    uint32_t *pGPIOA_MODER = (uint32_t *)STM32_GPIOA_MODER; 

        
    uint32_t *pGPIOA_PUPDR = (uint32_t *)STM32_GPIOA_PUPDR; 
    uint32_t *pGPIOA_IDR = (uint32_t *)STM32_GPIOA_IDR;

    uint32_t *pGPIOC_OTYPER = (uint32_t *)STM32_GPIOC_OTYPER;
    uint32_t *pGPIOC_PUPDR = (uint32_t *)STM32_GPIOC_PUPDR;
    uint32_t *pGPIOC_BSRR = (uint32_t *)STM32_GPIOC_BSRR;

    /* Habilita clock GPIOA e GPIOC */

    reg = *pRCC_AHB1ENR;
    reg |= (RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN );
    *pRCC_AHB1ENR = reg;

    /* Configura PA0 p/ entrada pull-up ON e pulldown OFF */

    //pull_down
    reg = *pGPIOA_MODER;
    reg &= ~(GPIO_MODER_MASK(0)); 
    reg |= (GPIO_MODER_INPUT << GPIO_MODER_SHIFT(0));
    *pGPIOA_MODER = reg;

    //pull_up
    reg = *pGPIOA_PUPDR;
    reg &= ~(GPIO_PUPDR_MASK(0));
    reg |= (GPIO_PUPDR_PULLUP << GPIO_PUPDR_SHIFT(0));
    *pGPIOA_PUPDR = reg;


    /* Configura PC13 como saida pull-up off e pull-down off */

    reg = *pGPIOC_MODER;
    reg &= ~(GPIO_MODER_MASK(13));
    reg |= (GPIO_MODER_OUTPUT << GPIO_MODER_SHIFT(13));
    *pGPIOC_MODER = reg;

    reg = *pGPIOC_OTYPER;
    reg &= ~(GPIO_OT13_MASK);
    reg |= (GPIO_OTYPER_PP << GPIO_OT13_SHIFT);
    *pGPIOC_OTYPER = reg;

    reg = *pGPIOC_PUPDR;
    reg &= ~(GPIO_PUPDR_MASK(13));
    reg |= (GPIO_PUPDR_NONE << GPIO_PUPDR_SHIFT(13));
    *pGPIOC_PUPDR = reg;

    static const uint32_t led_value[] = {200,600};
    uint8_t cont;
    
    while(1)
    {
        reg = *pGPIOA_IDR;
        if ( (reg & GPIO_IDR(0)) == 0)
        {
            cont++;
            if (cont > 1)
            {
                cont = 0;
            }    
        }
        *pGPIOC_BSRR = GPIO_BSRR_SET(13);
        for (uint32_t i = 0; i < led_value[cont]; i++);
        *pGPIOC_BSRR = GPIO_BSRR_RST(13);
        for (uint32_t i = 0; i < led_value[cont]; i++);

    }
    /* Nao deveria chegar aqui */

    return EXIT_SUCCESS;
    
}