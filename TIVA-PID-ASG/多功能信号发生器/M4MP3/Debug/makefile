################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := c:/ti/ccsv6/tools/compiler/arm_5.1.5

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./FatFs/ff.obj" \
"./uc1701.obj" \
"./tmp100.obj" \
"./tm4c123gh6pm_startup_ccs.obj" \
"./tlv320aic23b.obj" \
"./tasks.obj" \
"./sdc.obj" \
"./queue.obj" \
"./portasm.obj" \
"./port.obj" \
"./m4_uart.obj" \
"./m4_tirtos.obj" \
"./m4_temp.obj" \
"./m4_pwm.obj" \
"./m4_player.obj" \
"./m4_pinmux.obj" \
"./m4_main.obj" \
"./m4_lcd.obj" \
"./m4_i2c.obj" \
"./m4_gpio.obj" \
"./m4_freertos.obj" \
"./m4_dac.obj" \
"./m4_codec.obj" \
"./m4_app_gui.obj" \
"./m4_app_cli.obj" \
"./m4_adc.obj" \
"./list.obj" \
"./heap_2.obj" \
"./gt20l16s1y.obj" \
"./dac7512.obj" \
"../tm4c123gh6pm.cmd" \
-l"libc.a" \
-l"C:\ti\TivaWare_C_Series-2.0.1.11577\driverlib\ccs\Debug\driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include FatFs/subdir_vars.mk
-include subdir_rules.mk
-include FatFs/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
M4MP3.out \

EXE_OUTPUTS__QUOTED += \
"M4MP3.out" \

BIN_OUTPUTS += \
M4MP3.hex \

BIN_OUTPUTS__QUOTED += \
"M4MP3.hex" \


# All Target
all: M4MP3.out

# Tool invocations
M4MP3.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --define=USE_DY_PB_2 --define=USE_DY_PB_3 --define=USE_DY_PB_FINAL --define=_M4_FREERTOS --define=M4_TIRTOS --display_error_number --diag_warning=225 --diag_wrap=off -z -m"M4MP3.map" --heap_size=0 --stack_size=512 -i"c:/ti/ccsv6/tools/compiler/arm_5.1.5/lib" -i"c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="M4MP3_linkInfo.xml" --rom_model -o "M4MP3.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

M4MP3.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armhex"  -o "M4MP3.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "m4_app_gui.pp" 
	-$(RM) "dac7512.pp" "gt20l16s1y.pp" "heap_2.pp" "list.pp" "m4_adc.pp" "m4_app_cli.pp" "m4_codec.pp" "m4_dac.pp" "m4_freertos.pp" "m4_gpio.pp" "m4_i2c.pp" "m4_lcd.pp" "m4_main.pp" "m4_pinmux.pp" "m4_player.pp" "m4_pwm.pp" "m4_temp.pp" "m4_tirtos.pp" "m4_uart.pp" "port.pp" "queue.pp" "sdc.pp" "tasks.pp" "tlv320aic23b.pp" "tm4c123gh6pm_startup_ccs.pp" "tmp100.pp" "uc1701.pp" "FatFs\ff.pp" 
	-$(RM) "dac7512.obj" "gt20l16s1y.obj" "heap_2.obj" "list.obj" "m4_adc.obj" "m4_app_cli.obj" "m4_app_gui.obj" "m4_codec.obj" "m4_dac.obj" "m4_freertos.obj" "m4_gpio.obj" "m4_i2c.obj" "m4_lcd.obj" "m4_main.obj" "m4_pinmux.obj" "m4_player.obj" "m4_pwm.obj" "m4_temp.obj" "m4_tirtos.obj" "m4_uart.obj" "port.obj" "portasm.obj" "queue.obj" "sdc.obj" "tasks.obj" "tlv320aic23b.obj" "tm4c123gh6pm_startup_ccs.obj" "tmp100.obj" "uc1701.obj" "FatFs\ff.obj" 
	-$(RM) "portasm.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

