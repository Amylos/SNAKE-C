################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Container/container.c 

OBJS += \
./Container/container.o 

C_DEPS += \
./Container/container.d 


# Each subdirectory must supply rules for building sources it contributes
Container/%.o: ../Container/%.c Container/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/thierry/SCOLAIRE-2021-2022/821-IR/EVALS-DS/DS3-2022-04-13/Code/snakeApp/Application" -I"/home/thierry/SCOLAIRE-2021-2022/821-IR/EVALS-DS/DS3-2022-04-13/Code/snakeApp/Snake" -I"/home/thierry/SCOLAIRE-2021-2022/821-IR/EVALS-DS/DS3-2022-04-13/Code/snakeApp/Scene" -I"/home/thierry/SCOLAIRE-2021-2022/821-IR/EVALS-DS/DS3-2022-04-13/Code/snakeApp/Container" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


