################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ff/ff.c \
../User/ff/sdmm.c 

OBJS += \
./User/ff/ff.o \
./User/ff/sdmm.o 

C_DEPS += \
./User/ff/ff.d \
./User/ff/sdmm.d 


# Each subdirectory must supply rules for building sources it contributes
User/ff/%.o: ../User/ff/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32ec -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\Debug" -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\Core" -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\User" -I"D:\RISC-V\mrs_workspace\sd_card_fatfs_r1\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


