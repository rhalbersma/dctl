################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/board/coordinates.cpp \
../test/board/dimensions.cpp \
../test/board/grid.cpp 

OBJS += \
./test/board/coordinates.o \
./test/board/dimensions.o \
./test/board/grid.o 

CPP_DEPS += \
./test/board/coordinates.d \
./test/board/dimensions.d \
./test/board/grid.d 


# Each subdirectory must supply rules for building sources it contributes
test/board/%.o: ../test/board/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/rein/workspace/dctl" -O3 -g3 -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


