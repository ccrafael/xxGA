################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../exception/ConfigurationException.cpp \
../exception/NotFoundException.cpp \
../exception/OperatorException.cpp \
../exception/ProblemException.cpp 

OBJS += \
./exception/ConfigurationException.o \
./exception/NotFoundException.o \
./exception/OperatorException.o \
./exception/ProblemException.o 

CPP_DEPS += \
./exception/ConfigurationException.d \
./exception/NotFoundException.d \
./exception/OperatorException.d \
./exception/ProblemException.d 


# Each subdirectory must supply rules for building sources it contributes
exception/%.o: ../exception/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -march=native -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


