/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */

#include <stdio.h>

#include "init.h"

int main(void)
{
    init();

    puts("Booted..");
    vTaskStartScheduler();

    for( ;; );
}
