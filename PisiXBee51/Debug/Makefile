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

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../drivers/ \
../drivers/drivers/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../drivers/adc.c \
../drivers/board.c \
../drivers/com.c \
../drivers/drivers/adc_driver.c \
../drivers/drivers/clksys_driver.c \
../drivers/drivers/dma_driver.c \
../drivers/drivers/port_driver.c \
../drivers/drivers/qdec_driver.c \
../drivers/drivers/tc_driver.c \
../drivers/drivers/twi_master_driver.c \
../drivers/drivers/usart_driver.c \
../drivers/gyro.c \
../drivers/motor.c \
../main.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
drivers/adc.o \
drivers/board.o \
drivers/com.o \
drivers/drivers/adc_driver.o \
drivers/drivers/clksys_driver.o \
drivers/drivers/dma_driver.o \
drivers/drivers/port_driver.o \
drivers/drivers/qdec_driver.o \
drivers/drivers/tc_driver.o \
drivers/drivers/twi_master_driver.o \
drivers/drivers/usart_driver.o \
drivers/gyro.o \
drivers/motor.o \
main.o

OBJS_AS_ARGS +=  \
drivers/adc.o \
drivers/board.o \
drivers/com.o \
drivers/drivers/adc_driver.o \
drivers/drivers/clksys_driver.o \
drivers/drivers/dma_driver.o \
drivers/drivers/port_driver.o \
drivers/drivers/qdec_driver.o \
drivers/drivers/tc_driver.o \
drivers/drivers/twi_master_driver.o \
drivers/drivers/usart_driver.o \
drivers/gyro.o \
drivers/motor.o \
main.o

C_DEPS +=  \
drivers/adc.d \
drivers/board.d \
drivers/com.d \
drivers/drivers/adc_driver.d \
drivers/drivers/clksys_driver.d \
drivers/drivers/dma_driver.d \
drivers/drivers/port_driver.d \
drivers/drivers/qdec_driver.d \
drivers/drivers/tc_driver.d \
drivers/drivers/twi_master_driver.d \
drivers/drivers/usart_driver.d \
drivers/gyro.d \
drivers/motor.d \
main.d

C_DEPS_AS_ARGS +=  \
drivers/adc.d \
drivers/board.d \
drivers/com.d \
drivers/drivers/adc_driver.d \
drivers/drivers/clksys_driver.d \
drivers/drivers/dma_driver.d \
drivers/drivers/port_driver.d \
drivers/drivers/qdec_driver.d \
drivers/drivers/tc_driver.d \
drivers/drivers/twi_master_driver.d \
drivers/drivers/usart_driver.d \
drivers/gyro.d \
drivers/motor.d \
main.d

OUTPUT_FILE_PATH +=PisiXBee51.elf

OUTPUT_FILE_PATH_AS_ARGS +=PisiXBee51.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler





























drivers/%.o: ../drivers/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=32000000  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega32a4u -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

drivers/drivers/%.o: ../drivers/drivers/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=32000000  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega32a4u -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=32000000  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega32a4u -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="PisiXBee51.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atxmega32a4u  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "PisiXBee51.elf" "PisiXBee51.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "PisiXBee51.elf" "PisiXBee51.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "PisiXBee51.elf" > "PisiXBee51.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "PisiXBee51.elf" "PisiXBee51.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-size.exe" "PisiXBee51.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "PisiXBee51.elf" "PisiXBee51.a" "PisiXBee51.hex" "PisiXBee51.lss" "PisiXBee51.eep" "PisiXBee51.map" "PisiXBee51.srec" "PisiXBee51.usersignatures"
	