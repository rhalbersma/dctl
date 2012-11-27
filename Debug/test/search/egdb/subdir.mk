################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/search/egdb/frisian.cpp \
../test/search/egdb/international.cpp \
../test/search/egdb/killer.cpp 

OBJS += \
./test/search/egdb/frisian.o \
./test/search/egdb/international.o \
./test/search/egdb/killer.o 

CPP_DEPS += \
./test/search/egdb/frisian.d \
./test/search/egdb/international.d \
./test/search/egdb/killer.d 


# Each subdirectory must supply rules for building sources it contributes
test/search/egdb/%.o: ../test/search/egdb/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/rhalbersma/workspace/dctl" -O3 -g3 -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


