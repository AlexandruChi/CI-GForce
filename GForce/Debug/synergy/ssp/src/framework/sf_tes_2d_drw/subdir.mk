################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/framework/sf_tes_2d_drw/sf_tes_2d_drw_base.c \
../synergy/ssp/src/framework/sf_tes_2d_drw/sf_tes_2d_drw_irq.c \
../synergy/ssp/src/framework/sf_tes_2d_drw/sf_tes_2d_drw_memory.c 

C_DEPS += \
./synergy/ssp/src/framework/sf_tes_2d_drw/sf_tes_2d_drw_base.d \
./synergy/ssp/src/framework/sf_tes_2d_drw/sf_tes_2d_drw_irq.d \
./synergy/ssp/src/framework/sf_tes_2d_drw/sf_tes_2d_drw_memory.d 

OBJS += \
./synergy/ssp/src/framework/sf_tes_2d_drw/sf_tes_2d_drw_base.o \
./synergy/ssp/src/framework/sf_tes_2d_drw/sf_tes_2d_drw_irq.o \
./synergy/ssp/src/framework/sf_tes_2d_drw/sf_tes_2d_drw_memory.o 

SREC += \
GForce.srec 

MAP += \
GForce.map 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/framework/sf_tes_2d_drw/%.o: ../synergy/ssp/src/framework/sf_tes_2d_drw/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_SYNERGY_ -I"C:/Users/alexa/Facultate/CI/GForce/synergy_cfg/ssp_cfg/bsp" -I"C:/Users/alexa/Facultate/CI/GForce/synergy_cfg/ssp_cfg/driver" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp/inc" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp/inc/bsp" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp/inc/bsp/cmsis/Include" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp/inc/driver/api" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp/inc/driver/instances" -I"C:/Users/alexa/Facultate/CI/GForce/src" -I"C:/Users/alexa/Facultate/CI/GForce/src/synergy_gen" -I"C:/Users/alexa/Facultate/CI/GForce/synergy_cfg/ssp_cfg/framework" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp/inc/framework/api" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp/inc/framework/instances" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp/inc/framework/tes" -I"C:/Users/alexa/Facultate/CI/GForce/synergy_cfg/ssp_cfg/framework/el" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp/inc/framework/el" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp/src/framework/el/tx" -I"C:/Users/alexa/Facultate/CI/GForce/synergy/ssp_supplemental/inc/framework/instances" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

