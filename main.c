#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define ESPI_GPIO_IMPL
#include "espi_gpio.h"

_inline void check_button_press(volatile GPIO_Mem *mem, size_t button_pin, size_t led_pin);

int main(void)
{
    /* hardware pin numbers */
    size_t PU_led_pin = ;
    size_t PD_led_pin = ;
    size_t PU_button_pin = ;
    size_t PD_button_pin = ;

    /* pointer into GPIO address space */
    volatile GPIO_Mem *gpio = map_gpio();

    GPIO_OUTPUT(gpio, PU_led_pin);
    GPIO_OUTPUT(gpio, PD_led_pin);
    GPIO_INPUT(gpio, PU_button_pin);
    GPIO_INPUT(gpio, PD_button_pin);

    while (1) {
        /* see if one button is pressed and handle the event */
        check_button_press(gpio, PU_button_pin, PU_led_pin);
        check_button_press(gpio, PD_button_pin, PD_led_pin);
    }

    return EXIT_SUCCESS;
}

_inline void check_button_press(volatile GPIO_Mem *mem, size_t button_pin, size_t led_pin)
{
    if (GPIO_READ(mem, button_pin))
        GPIO_SET(mem, led_pin);
    else
        GPIO_CLR(mem, led_pin);
}
