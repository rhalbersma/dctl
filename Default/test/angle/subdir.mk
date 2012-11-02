################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/angle/degrees.cpp \
../test/angle/traits.cpp \
../test/angle/transform.cpp 

OBJS += \
./test/angle/degrees.o \
./test/angle/traits.o \
./test/angle/transform.o 

CPP_DEPS += \
./test/angle/degrees.d \
./test/angle/traits.d \
./test/angle/transform.d 


# Each subdirectory must supply rules for building sources it contributes
test/angle/%.o: ../test/angle/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\MinGW\include\boost" -I"C:\Users\Reinepein\workspace\dctl" -O3 -g -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


