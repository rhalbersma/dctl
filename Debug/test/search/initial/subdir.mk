################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/search/initial/international.cpp 

OBJS += \
./test/search/initial/international.o 

CPP_DEPS += \
./test/search/initial/international.d 


# Each subdirectory must supply rules for building sources it contributes
test/search/initial/%.o: ../test/search/initial/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/rein/workspace/dctl" -O3 -g3 -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -std=c++11 -DUSE_BSFQ -DUSE_POPCNT -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


