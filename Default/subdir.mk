################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Config.cpp \
../Environment.cpp \
../Genotype.cpp \
../Individual.cpp \
../Phenotype.cpp \
../Population.cpp \
../Util.cpp \
../ga.cpp 

OBJS += \
./Config.o \
./Environment.o \
./Genotype.o \
./Individual.o \
./Phenotype.o \
./Population.o \
./Util.o \
./ga.o 

CPP_DEPS += \
./Config.d \
./Environment.d \
./Genotype.d \
./Individual.d \
./Phenotype.d \
./Population.d \
./Util.d \
./ga.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


