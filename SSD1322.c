#include "SSD1322.h"
#include <math.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SSD1322_SETCOMMANDLOCK           = 0xFD,
    SSD1322_DISPLAYOFF               = 0xAE,
    SSD1322_DISPLAYON                = 0xAF,
    SSD1322_SETCLOCKDIVIDER          = 0xB3,
    SSD1322_SETDISPLAYOFFSET         = 0xA2,
    SSD1322_SETSTARTLINE             = 0xA1,
    SSD1322_SETREMAP                 = 0xA0,
    SSD1322_FUNCTIONSEL              = 0xAB,
    SSD1322_DISPLAYENHANCE           = 0xB4,
    SSD1322_SETCONTRASTCURRENT       = 0xC1,
    SSD1322_MASTERCURRENTCONTROL     = 0xC7,
    SSD1322_SETPHASELENGTH           = 0xB1,
    SSD1322_DISPLAYENHANCEB          = 0xD1,
    SSD1322_SETPRECHARGEVOLTAGE      = 0xBB,
    SSD1322_SETSECONDPRECHARGEPERIOD = 0xB6,
    SSD1322_SETVCOMH                 = 0xBE,
    SSD1322_NORMALDISPLAY            = 0xA6,
    SSD1322_INVERSEDISPLAY           = 0xA7,
    SSD1322_SETMUXRATIO              = 0xCA,
    SSD1322_SETCOLUMNADDR            = 0x15,
    SSD1322_SETROWADDR               = 0x75,
    SSD1322_WRITERAM                 = 0x5C,
    SSD1322_ENTIREDISPLAYON          = 0xA5,
    SSD1322_ENTIREDISPLAYOFF         = 0xA4,
    SSD1322_SETGPIO                  = 0xB5,
    SSD1322_EXITPARTIALDISPLAY       = 0xA9,
    SSD1322_SELECTDEFAULTGRAYSCALE   = 0xB9,
} SSD1322Command;

#define SSD1322_MIN_SEG 0x1C
#define SSD1322_MAX_SEG 0x5B

// screen graphic buffer
uint8_t ssd1322_screen_buffer[SSD1322_SCREEN_HEIGHT * SSD1322_SCREEN_WIDTH * SSD1322_SCREEN_BPP / 8];

static void (*_sendCommand)(uint8_t command);
static void (*_sendDataArray)(uint8_t *data, int32_t length);

void _sendData(uint8_t data) {
    _sendDataArray(&data, 1);
}

void ssd1322_init(void (*sendCommand)(uint8_t command),
        void (*sendData)(uint8_t *data, int32_t length)) {

    // attach communication functions
    _sendCommand = sendCommand;
    _sendDataArray = sendData;

    // hw init
    _sendCommand(SSD1322_SETCOMMANDLOCK);// 0xFD
    _sendData(0x12);// Unlock OLED driver IC

    _sendCommand(SSD1322_DISPLAYOFF);// 0xAE

    _sendCommand(SSD1322_SETCLOCKDIVIDER);// 0xB3
    _sendData(0x91);// 0xB3

    _sendCommand(SSD1322_SETMUXRATIO);// 0xCA
    _sendData(0x3F);// duty = 1/64

    _sendCommand(SSD1322_SETDISPLAYOFFSET);// 0xA2
    _sendData(0x00);

    _sendCommand(SSD1322_SETSTARTLINE);// 0xA1
    _sendData(0x00);

    _sendCommand(SSD1322_SETREMAP);// 0xA0
    _sendData(0x14);//Horizontal address increment,Disable Column Address Re-map,Enable Nibble Re-map,Scan from COM[N-1] to COM0,Disable COM Split Odd Even
    _sendData(0x11);//Enable Dual COM mode

    _sendCommand(SSD1322_SETGPIO);// 0xB5
    _sendData(0x00);// Disable GPIO Pins Input

    _sendCommand(SSD1322_FUNCTIONSEL);// 0xAB
    _sendData(0x01);// selection external vdd

    _sendCommand(SSD1322_DISPLAYENHANCE);// 0xB4
    _sendData(0xA0);// enables the external VSL
    _sendData(0xFD);// 0xfFD,Enhanced low GS display quality;default is 0xb5(normal),

    _sendCommand(SSD1322_SETCONTRASTCURRENT);// 0xC1
    _sendData(0xFF);// 0xFF - default is 0x7f

    _sendCommand(SSD1322_MASTERCURRENTCONTROL);// 0xC7
    _sendData(0x0F);// default is 0x0F

    // Set grayscale
    _sendCommand(SSD1322_SELECTDEFAULTGRAYSCALE); // 0xB9

    _sendCommand(SSD1322_SETPHASELENGTH);// 0xB1
    _sendData(0xE2);// default is 0x74

    _sendCommand(SSD1322_DISPLAYENHANCEB);// 0xD1
    _sendData(0x82);// Reserved;default is 0xa2(normal)
    _sendData(0x20);//

    _sendCommand(SSD1322_SETPRECHARGEVOLTAGE);// 0xBB
    _sendData(0x1F);// 0.6xVcc

    _sendCommand(SSD1322_SETSECONDPRECHARGEPERIOD);// 0xB6
    _sendData(0x08);// default

    _sendCommand(SSD1322_SETVCOMH);// 0xBE
    _sendData(0x04);// 0.86xVcc;default is 0x04

    _sendCommand(SSD1322_NORMALDISPLAY);// 0xA6

    _sendCommand(SSD1322_EXITPARTIALDISPLAY);// 0xA9

    _sendCommand(SSD1322_DISPLAYON);// 0xAF

    // clear screen buffer
    memset(ssd1322_screen_buffer, 0, sizeof(ssd1322_screen_buffer));
    // flush graphics buffer to screen
    ssd1322_flushBuffer();
}

void ssd1322_flushBuffer(void) {
    _sendCommand(SSD1322_SETCOLUMNADDR);
    _sendData(SSD1322_MIN_SEG);
    _sendData(SSD1322_MAX_SEG);

    _sendCommand(SSD1322_SETROWADDR);
    _sendData(0);
    _sendData(63);

    _sendCommand(SSD1322_WRITERAM);

    _sendDataArray(ssd1322_screen_buffer, sizeof(ssd1322_screen_buffer));
}

#ifdef __cplusplus
}
#endif
