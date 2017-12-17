################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../problems/fparser/example.cc \
../problems/fparser/fparser.cc \
../problems/fparser/fpoptimizer.cc 

CC_DEPS += \
./problems/fparser/example.d \
./problems/fparser/fparser.d \
./problems/fparser/fpoptimizer.d 

OBJS += \
./problems/fparser/example.o \
./problems/fparser/fparser.o \
./problems/fparser/fpoptimizer.o 


# Each subdirectory must supply rules for building sources it contributes
problems/fparser/%.o: ../problems/fparser/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -march=native -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


