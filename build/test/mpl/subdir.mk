################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/mpl/algorithm.cpp 

OBJS += \
./test/mpl/algorithm.o 

CPP_DEPS += \
./test/mpl/algorithm.d 


# Each subdirectory must supply rules for building sources it contributes
test/mpl/%.o: ../test/mpl/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\dev\MinGW\include\boost" -I"C:\Users\Reinepein\workspace\dctl-make" -O3 -g -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


