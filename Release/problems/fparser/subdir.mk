################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../problems/fparser/example.cc \
../problems/fparser/fparser.cc 

CC_DEPS += \
./problems/fparser/example.d \
./problems/fparser/fparser.d 

OBJS += \
./problems/fparser/example.o \
./problems/fparser/fparser.o 


# Each subdirectory must supply rules for building sources it contributes
problems/fparser/%.o: ../problems/fparser/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -march=native -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


