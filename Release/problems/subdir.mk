################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../problems/BagProblem.cpp \
../problems/FunctionsProblem.cpp \
../problems/Image.cpp \
../problems/ParseFunctionProblem.cpp 

OBJS += \
./problems/BagProblem.o \
./problems/FunctionsProblem.o \
./problems/Image.o \
./problems/ParseFunctionProblem.o 

CPP_DEPS += \
./problems/BagProblem.d \
./problems/FunctionsProblem.d \
./problems/Image.d \
./problems/ParseFunctionProblem.d 


# Each subdirectory must supply rules for building sources it contributes
problems/%.o: ../problems/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -march=native -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


