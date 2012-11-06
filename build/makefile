################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := del

# All of the sources participating in the build are defined here
-include sources.mk
-include test/walk/italian/subdir.mk
-include test/walk/international/subdir.mk
-include test/walk/initial/subdir.mk
-include test/utility/subdir.mk
-include test/successor/rulebook/subdir.mk
-include test/successor/regressions/subdir.mk
-include test/setup/subdir.mk
-include test/search/initial/subdir.mk
-include test/search/egdb/subdir.mk
-include test/search/subdir.mk
-include test/mpl/subdir.mk
-include test/group/subdir.mk
-include test/dxp/subdir.mk
-include test/board/subdir.mk
-include test/bit/subdir.mk
-include test/angle/subdir.mk
-include test/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: dctl-make

# Tool invocations
dctl-make: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MinGW C++ Linker'
	g++ -L"C:\dev\MinGW\lib" -o "dctl-make" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) dctl-make
	-@echo ' '

post-build:
	-dctl-make.exe --result_code=yes --report_level=detailed
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
