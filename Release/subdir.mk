################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CLEvaluator.cpp \
../Config.cpp \
../Environment.cpp \
../GenotypeBit.cpp \
../GenotypeNumber.cpp \
../Individual.cpp \
../Island.cpp \
../OperatorFactory.cpp \
../Output.cpp \
../Population.cpp \
../Problem.cpp \
../Util.cpp \
../ga.cpp \
../main.cpp 

OBJS += \
./CLEvaluator.o \
./Config.o \
./Environment.o \
./GenotypeBit.o \
./GenotypeNumber.o \
./Individual.o \
./Island.o \
./OperatorFactory.o \
./Output.o \
./Population.o \
./Problem.o \
./Util.o \
./ga.o \
./main.o 

CPP_DEPS += \
./CLEvaluator.d \
./Config.d \
./Environment.d \
./GenotypeBit.d \
./GenotypeNumber.d \
./Individual.d \
./Island.d \
./OperatorFactory.d \
./Output.d \
./Population.d \
./Problem.d \
./Util.d \
./ga.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -march=native -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


