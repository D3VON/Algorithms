################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Program9_MaxFlow_DevonMcBride.cpp \
../src/program9.cpp 

OBJS += \
./src/Program9_MaxFlow_DevonMcBride.o \
./src/program9.o 

CPP_DEPS += \
./src/Program9_MaxFlow_DevonMcBride.d \
./src/program9.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


