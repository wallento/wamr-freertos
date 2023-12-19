#pragma once

#ifdef STM32F429
#define USART_RCC      RCC_USART3
#define USART_GPIO_RCC RCC_GPIOD
#define USART          USART3
#define USART_PORT     GPIOD
#define USART_PINS     (GPIO8 | GPIO9)
#define USART_AF       GPIO_AF7
#endif 

#ifdef STM32F401
#define USART_RCC      RCC_USART2
#define USART_GPIO_RCC RCC_GPIOA
#define USART          USART2
#define USART_PORT     GPIOA
#define USART_PINS     (GPIO2 | GPIO3)
#define USART_AF       GPIO_AF7
#endif

void init(void);