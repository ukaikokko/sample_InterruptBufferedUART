// main code here!!

#include <main.h>
#include <usart.h>
#include <stdio.h>
#include <ukaikokko/ukaikokko.h>

ukaikokko::InterruptBufferedUART<256, 64, 1024> pc(&huart2);
ukaikokko::GPOutput led(DebugLED_GPIO_Port, DebugLED_Pin);

static bool setupDone = false;

#ifdef __cplusplus
extern "C"
{
#endif

    // MARK:setup
    void user_setup(void)
    {
        printf("Hello, world!\r\n");

        pc.begin();
        setupDone = true;
    }

    // MARK:loop
    void user_loop(void)
    {
        const uint32_t now = HAL_GetTick();
        static uint32_t pre = now;

        if (now - pre >= 10)
        {
            pc.periodic();
            while (pc.available())
            {
                const int16_t data = pc.read();
                pc.write(static_cast<uint8_t>(data));
            }
            // if (pc.getError())
            // {
            //     led.write(1);
            // }
            if (pc.getUserRxOverflowCount())
            {
                led.write(1);
            }
            pre = now;
        }
    }

    // MARK:_write
    int _write(int file, char* ptr, int len)
    {
        if (!setupDone)
        {
            HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, 10); // ブロッキング
            return len;
        }
        else
        {
            pc.write((uint8_t*)ptr, len);
            return len;
        }
    }

    // MARK:UART_RxCpltCallback
    void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
    {
        pc.RxCplt(huart);
    }

    // MARK:UART_TxCpltCallback
    void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
    {
        pc.TxCplt(huart);
    }

#ifdef __cplusplus
}
#endif
