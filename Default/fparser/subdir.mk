################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../fparser/example.cc \
../fparser/fparser.cc \
../fparser/fpoptimizer.cc 

CC_DEPS += \
./fparser/example.d \
./fparser/fparser.d \
./fparser/fpoptimizer.d 

OBJS += \
./fparser/example.o \
./fparser/fparser.o \
./fparser/fpoptimizer.o 


# Each subdirectory must supply rules for building sources it contributes
fparser/%.o: ../fparser/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


