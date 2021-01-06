#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SSD1322_SCREEN_WIDTH    256
#define SSD1322_SCREEN_HEIGHT   64
#define SSD1322_SCREEN_BPP      4

extern uint8_t ssd1322_screen_buffer[SSD1322_SCREEN_HEIGHT * SSD1322_SCREEN_WIDTH * SSD1322_SCREEN_BPP / 8];

void ssd1322_init(void (*sendCommand)(uint8_t command),
        void (*sendData)(uint8_t *data, int32_t length));

void ssd1322_flushBuffer(void);

#ifdef __cplusplus
}
#endif
