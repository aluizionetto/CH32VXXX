################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/hw_spi/hw_spi.c 

OBJS += \
./User/hw_spi/hw_spi.o 

C_DEPS += \
./User/hw_spi/hw_spi.d 


# Each subdirectory must supply rules for building sources it contributes
User/hw_spi/%.o: ../User/hw_spi/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32ec -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\Debug" -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\Core" -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\User" -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


