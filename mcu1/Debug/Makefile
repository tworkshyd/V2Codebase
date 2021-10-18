################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../__vm/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../dataBase.cpp \
../gauge_pressure.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
dataBase.o \
gauge_pressure.o

OBJS_AS_ARGS +=  \
dataBase.o \
gauge_pressure.o

C_DEPS +=  \
dataBase.d \
gauge_pressure.d

C_DEPS_AS_ARGS +=  \
dataBase.d \
gauge_pressure.d

OUTPUT_FILE_PATH +=mcu1.elf

OUTPUT_FILE_PATH_AS_ARGS +=mcu1.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
./dataBase.o: .././dataBase.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAM3X8E__ -D__AVR_ATmega2560__ -D_VMDEBUG=1 -DARDUINO=108015 -DARDUINO_MAIN -DF_CPU=16000000L -D__AVR__ -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR  -I"../../../mcu1" -I"C:/Program Files (x86)/Arduino/hardware/arduino/avr/cores/arduino" -I"C:/Program Files (x86)/Arduino/hardware/arduino/avr/variants/mega" -I"C:\Users\abdul\AppData\Local\ARDUIN~1\packages\arduino\tools\avr-gcc\730-AT~1.1-A\\lib\gcc\avr\733F76~1.0\include" -I"C:\Users\abdul\AppData\Local\ARDUIN~1\packages\arduino\tools\avr-gcc\730-AT~1.1-A\avr\include" -I"C:\Users\abdul\AppData\Local\arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\avr\include-fixed" -I"C:\Users\abdul\AppData\Local\ARDUIN~1\packages\arduino\tools\avr-gcc\730-AT~1.1-A\avr\include\avr" -I"C:\Users\abdul\AppData\Local\arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\lib\gcc\avr\4.9.2\include" -I"C:\Users\abdul\AppData\Local\arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\lib\gcc\avr\4.9.3\include" -I"{vm.intellisense.add-paths}"  -Os -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -Wall -mcpu=cortex-m3 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./gauge_pressure.o: .././gauge_pressure.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAM3X8E__ -D__AVR_ATmega2560__ -D_VMDEBUG=1 -DARDUINO=108015 -DARDUINO_MAIN -DF_CPU=16000000L -D__AVR__ -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR  -I"../../../mcu1" -I"C:/Program Files (x86)/Arduino/hardware/arduino/avr/cores/arduino" -I"C:/Program Files (x86)/Arduino/hardware/arduino/avr/variants/mega" -I"C:\Users\abdul\AppData\Local\ARDUIN~1\packages\arduino\tools\avr-gcc\730-AT~1.1-A\\lib\gcc\avr\733F76~1.0\include" -I"C:\Users\abdul\AppData\Local\ARDUIN~1\packages\arduino\tools\avr-gcc\730-AT~1.1-A\avr\include" -I"C:\Users\abdul\AppData\Local\arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\avr\include-fixed" -I"C:\Users\abdul\AppData\Local\ARDUIN~1\packages\arduino\tools\avr-gcc\730-AT~1.1-A\avr\include\avr" -I"C:\Users\abdul\AppData\Local\arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\lib\gcc\avr\4.9.2\include" -I"C:\Users\abdul\AppData\Local\arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\lib\gcc\avr\4.9.3\include" -I"{vm.intellisense.add-paths}"  -Os -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -Wall -mcpu=cortex-m3 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="mcu1.map" -Wl,--start-group -lm  -Wl,--end-group -L"../cmsis/linkerScripts"  -Wl,--gc-sections -mcpu=cortex-m3 -Tsamd21e18a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "mcu1.elf" "mcu1.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "mcu1.elf" "mcu1.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "mcu1.elf" "mcu1.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "mcu1.elf" > "mcu1.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "mcu1.elf" "mcu1.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "mcu1.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "mcu1.elf" "mcu1.a" "mcu1.hex" "mcu1.bin" "mcu1.lss" "mcu1.eep" "mcu1.map" "mcu1.srec"
	