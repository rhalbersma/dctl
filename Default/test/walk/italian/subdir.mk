################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/walk/italian/gilbert.cpp 

OBJS += \
./test/walk/italian/gilbert.o 

CPP_DEPS += \
./test/walk/italian/gilbert.d 


# Each subdirectory must supply rules for building sources it contributes
test/walk/italian/%.o: ../test/walk/italian/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\MinGW\include\boost" -I"C:\Users\Reinepein\workspace\dctl" -O3 -g -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


