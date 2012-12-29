################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/bit/bit.cpp \
../test/bit/intrinsic.cpp \
../test/bit/lookup.cpp \
../test/bit/loop.cpp 

OBJS += \
./test/bit/bit.o \
./test/bit/intrinsic.o \
./test/bit/lookup.o \
./test/bit/loop.o 

CPP_DEPS += \
./test/bit/bit.d \
./test/bit/intrinsic.d \
./test/bit/lookup.d \
./test/bit/loop.d 


# Each subdirectory must supply rules for building sources it contributes
test/bit/%.o: ../test/bit/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DNDEBUG -I"/home/rein/workspace/dctl" -O3 -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -std=c++11 -DUSE_BSFQ -DUSE_POPCNT -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


