################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/walk/international/fmjd.cpp 

OBJS += \
./test/walk/international/fmjd.o 

CPP_DEPS += \
./test/walk/international/fmjd.d 


# Each subdirectory must supply rules for building sources it contributes
test/walk/international/%.o: ../test/walk/international/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DNDEBUG -I"/home/rein/workspace/dctl" -O3 -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -std=c++11 -DUSE_BSFQ -DUSE_POPCNT -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


