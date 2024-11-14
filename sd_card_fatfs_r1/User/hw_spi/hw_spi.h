#ifndef HW_SPI_H
#define HW_SPI_H

// SD CS
#define SD_CS_GPIO GPIOC
#define SD_CS_PIN 0

//0 -> HCLK/2
//1 -> HCLK/4
//2 -> HCLK/8
//3 -> HCLK/16
//4 -> HCLK/32
//5 -> HCLK/64
//6 -> HCLK/128
//7 -> HCLK/256
#define SPI_PREESCALER (3)  //HCLK/32

void SPI_init();
void SPI_begin_8();
void SPI_end();
uint8_t SPI_transfer_8(uint8_t data);
void SPI_set_prescaler(uint8_t presc);

#endif
