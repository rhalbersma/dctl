################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/setup/layout.cpp \
../test/setup/wesselink.cpp 

OBJS += \
./test/setup/layout.o \
./test/setup/wesselink.o 

CPP_DEPS += \
./test/setup/layout.d \
./test/setup/wesselink.d 


# Each subdirectory must supply rules for building sources it contributes
test/setup/%.o: ../test/setup/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DNDEBUG -I"/home/rein/workspace/dctl" -O3 -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -std=c++11 -DUSE_BSFQ -DUSE_POPCNT -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


