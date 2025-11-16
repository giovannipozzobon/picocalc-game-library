#include "lcd_ili9488.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include <string.h>
#include <stdlib.h>


static int dma_chan;


void lcd_clear(lcd_t *lcd, uint16_t color) {
    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        lcd->framebuffer[i] = color;
    }
}

// Get framebuffer pointer
uint16_t *lcd_get_framebuffer(lcd_t *lcd) {
    return lcd->framebuffer;
}

// --- SPI support functions ---
static inline void lcd_cmd(uint8_t cmd) {
    gpio_put(LCD_PIN_DC, 0);
    spi_write_blocking(LCD_SPI_PORT, &cmd, 1);
}

static inline void lcd_data(const uint8_t *data, size_t len) {
    gpio_put(LCD_PIN_DC, 1);
    spi_write_blocking(LCD_SPI_PORT, data, len);
}

static void lcd_reset() {
    gpio_put(LCD_PIN_RST, 0);
    sleep_ms(50);
    gpio_put(LCD_PIN_RST, 1);
    sleep_ms(120);
}

// --- ILI9488 Initialization ---
void lcd_init(lcd_t *lcd) {
    lcd->width = LCD_WIDTH;
    lcd->height = LCD_HEIGHT;
    lcd->framebuffer = calloc(LCD_WIDTH * LCD_HEIGHT, sizeof(uint16_t));

    // Initialize SPI at 20 MHz (safer than 40 MHz)
    spi_init(LCD_SPI_PORT, 20000000);
    spi_set_format(LCD_SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(LCD_PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(LCD_PIN_MOSI, GPIO_FUNC_SPI);
    gpio_init(LCD_PIN_CS);
    gpio_set_dir(LCD_PIN_CS, GPIO_OUT);
    gpio_put(LCD_PIN_CS, 0);

    gpio_init(LCD_PIN_DC);
    gpio_set_dir(LCD_PIN_DC, GPIO_OUT);
    gpio_init(LCD_PIN_RST);
    gpio_set_dir(LCD_PIN_RST, GPIO_OUT);

    lcd_reset();
    sleep_ms(10);

    // ILI9488P initialization sequence for RGB565
    lcd_cmd(0xE0); uint8_t data1[] = {0x00,0x03,0x09,0x08,0x16,0x0A,0x3F,0x78,0x4C,0x09,0x0A,0x08,0x16,0x1A,0x0F}; lcd_data(data1, sizeof(data1));
    lcd_cmd(0xE1); uint8_t data2[] = {0x00,0x16,0x19,0x03,0x0F,0x05,0x32,0x45,0x46,0x04,0x0E,0x0D,0x35,0x37,0x0F}; lcd_data(data2, sizeof(data2));
    lcd_cmd(0xC0); uint8_t data3[] = {0x17,0x15}; lcd_data(data3, 2);
    lcd_cmd(0xC1); uint8_t data4[] = {0x41}; lcd_data(data4, 1);
    lcd_cmd(0xC5); uint8_t data5[] = {0x00,0x12,0x80}; lcd_data(data5, 3);

    // MADCTL: MX=0, MY=0, MV=0, ML=0, BGR=1, MH=0 -> 0x08
    // BGR bit=1 to use BGR ordering in display
    lcd_cmd(0x36); uint8_t data6[] = {0x08}; lcd_data(data6, 1);

    // Pixel Format: RGB565 16-bit
    lcd_cmd(0x3A); uint8_t data7[] = {0x55}; lcd_data(data7, 1);

    // Exit sleep mode
    lcd_cmd(0x11);
    sleep_ms(150);

    // Display ON
    lcd_cmd(0x29);
    sleep_ms(50);

    // Initial screen clear (black)
    memset(lcd->framebuffer, 0, LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
    lcd_update_dma(lcd);
    sleep_ms(50);
}

// --- Set write area ---
void lcd_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t data[4];
    lcd_cmd(0x2A);
    data[0] = x0 >> 8; data[1] = x0 & 0xFF; data[2] = x1 >> 8; data[3] = x1 & 0xFF;
    lcd_data(data, 4);
    lcd_cmd(0x2B);
    data[0] = y0 >> 8; data[1] = y0 & 0xFF; data[2] = y1 >> 8; data[3] = y1 & 0xFF;
    lcd_data(data, 4);
    lcd_cmd(0x2C);
}

// --- Draw a single pixel ---
void lcd_draw_pixel(lcd_t *lcd, uint16_t x, uint16_t y, uint16_t color) {
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;
    lcd->framebuffer[y * LCD_WIDTH + x] = color;
}

// --- Fill a rectangle ---
void lcd_fill_rect(lcd_t *lcd, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t j = y; j < y + h; j++) {
        for (uint16_t i = x; i < x + w; i++) {
            lcd_draw_pixel(lcd, i, j, color);
        }
    }
}

// --- Update display via DMA ---
void lcd_update_dma(lcd_t *lcd) {
    lcd_set_address_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    gpio_put(LCD_PIN_DC, 1);
    gpio_put(LCD_PIN_CS, 0);

    uint8_t *fb_bytes = (uint8_t *)lcd->framebuffer;
    size_t total_bytes = LCD_WIDTH * LCD_HEIGHT * 2;

    // Swap bytes for big-endian (in-place)
    for (size_t i = 0; i < total_bytes; i += 2) {
        uint8_t temp = fb_bytes[i];
        fb_bytes[i] = fb_bytes[i + 1];
        fb_bytes[i + 1] = temp;
    }

    // SPI transfer
    spi_write_blocking(LCD_SPI_PORT, fb_bytes, total_bytes);

    // Wait for SPI to finish
    while (spi_is_busy(LCD_SPI_PORT)) tight_loop_contents();

    // Re-swap for next frame
    for (size_t i = 0; i < total_bytes; i += 2) {
        uint8_t temp = fb_bytes[i];
        fb_bytes[i] = fb_bytes[i + 1];
        fb_bytes[i + 1] = temp;
    }
}
