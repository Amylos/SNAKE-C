################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Scene/scene.c 

OBJS += \
./Scene/scene.o 

C_DEPS += \
./Scene/scene.d 


# Each subdirectory must supply rules for building sources it contributes
Scene/%.o: ../Scene/%.c Scene/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"D:\SCOLAIRE-2021-2022\821-IR\EVALS-DS\DS-03-2022-04-13\Code\SnakeApp_base\Application" -I"D:\SCOLAIRE-2021-2022\821-IR\EVALS-DS\DS-03-2022-04-13\Code\SnakeApp_base\Snake" -I"D:\SCOLAIRE-2021-2022\821-IR\EVALS-DS\DS-03-2022-04-13\Code\SnakeApp_base\Scene" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


