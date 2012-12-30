################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/successor/rulebook/international.cpp \
../test/successor/rulebook/italian.cpp 

OBJS += \
./test/successor/rulebook/international.o \
./test/successor/rulebook/italian.o 

CPP_DEPS += \
./test/successor/rulebook/international.d \
./test/successor/rulebook/italian.d 


# Each subdirectory must supply rules for building sources it contributes
test/successor/rulebook/%.o: ../test/successor/rulebook/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/rein/workspace/dctl" -O3 -g3 -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


