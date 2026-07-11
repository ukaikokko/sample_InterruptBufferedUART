// main code here!!

#include <main.h>
#include <usart.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // MARK:setup
    void user_setup(void)
    {
    }

    // MARK:loop
    void user_loop(void)
    {
        const uint32_t now = HAL_GetTick();
        static uint32_t pre = now;

        if (now - pre >= 10)
        {
            pre = now;
        }
    }

#ifdef __cplusplus
}
#endif
