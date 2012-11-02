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
	g++ -I"C:\MinGW\include\boost" -I"C:\Users\Reinepein\workspace\dctl" -O3 -g -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


