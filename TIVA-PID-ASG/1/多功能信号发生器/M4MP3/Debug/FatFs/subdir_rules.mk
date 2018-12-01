################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
FatFs/ff.obj: ../FatFs/ff.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577/third_party/FreeRTOS/Source/include" --include_path="C:/ti/TivaWare_C_Series-2.0.1.11577/third_party/FreeRTOS/Source/portable/RVDS/ARM_CM4F" --include_path="C:/Documents and Settings/Administrator/workspace_v6_0/M4MP3" --include_path="C:/ti/tirtos_tivac_2_00_01_23/packages" --include_path="C:/ti/tirtos_tivac_2_00_01_23/products/bios_6_40_01_15/packages" --include_path="C:/ti/xdctools_3_30_01_25_core/packages" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --define=USE_DY_PB_2 --define=USE_DY_PB_3 --define=USE_DY_PB_FINAL --define=_M4_FREERTOS --define=M4_TIRTOS --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="FatFs/ff.pp" --obj_directory="FatFs" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


