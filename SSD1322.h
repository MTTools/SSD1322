#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 64
#define SCREEN_BPP 4

void ssd1322_init(void (*sendCommand)(uint8_t command),
        void (*sendData)(uint8_t data),
        void (*sendDataArray)(uint8_t *data, int32_t length));

void ssd1322_flushBuffer(void);
void ssd1322_drawPixel(int x, int y, uint8_t color);
void ssd1322_fill(uint8_t color);

#ifdef __cplusplus
}
#endif
