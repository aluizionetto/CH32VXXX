#ifndef HW_SPI_H
#define HW_SPI_H

// SD CS
#define SD_CS_GPIO GPIOC
#define SD_CS_PIN 0

void SPI_init();
void SPI_begin_8();
void SPI_end();
uint8_t SPI_transfer_8(uint8_t data);
void SPI_set_prescaler(uint8_t presc);

#endif
