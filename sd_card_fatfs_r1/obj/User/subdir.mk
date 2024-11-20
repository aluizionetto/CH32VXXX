################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v00x_it.c \
../User/i2c_com.c \
../User/main.c \
../User/rtc_ds1307.c \
../User/sensor_aht20.c \
../User/system_ch32v00x.c 

OBJS += \
./User/ch32v00x_it.o \
./User/i2c_com.o \
./User/main.o \
./User/rtc_ds1307.o \
./User/sensor_aht20.o \
./User/system_ch32v00x.o 

C_DEPS += \
./User/ch32v00x_it.d \
./User/i2c_com.d \
./User/main.d \
./User/rtc_ds1307.d \
./User/sensor_aht20.d \
./User/system_ch32v00x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32ec -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\Debug" -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\Core" -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\User" -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


