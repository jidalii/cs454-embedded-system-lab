#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/Lab8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/Lab8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c adc.c touch.c time.c motor.c sorting.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/adc.o ${OBJECTDIR}/touch.o ${OBJECTDIR}/time.o ${OBJECTDIR}/motor.o ${OBJECTDIR}/sorting.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/adc.o.d ${OBJECTDIR}/touch.o.d ${OBJECTDIR}/time.o.d ${OBJECTDIR}/motor.o.d ${OBJECTDIR}/sorting.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/adc.o ${OBJECTDIR}/touch.o ${OBJECTDIR}/time.o ${OBJECTDIR}/motor.o ${OBJECTDIR}/sorting.o

# Source Files
SOURCEFILES=main.c adc.c touch.c time.c motor.c sorting.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Lab8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ256MC710
MP_LINKER_FILE_OPTION=,--script=p33FJ256MC710.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/b5351ca3c7a7863296f8fe37d319dae520b22e1b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/adc.o: adc.c  .generated_files/flags/default/c3baef0bae01c4469e29ad15d9dd7befefa328e3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.o.d 
	@${RM} ${OBJECTDIR}/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  adc.c  -o ${OBJECTDIR}/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/adc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/touch.o: touch.c  .generated_files/flags/default/75f4510455a7c185898511a44cc41ecfaf80f0b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/touch.o.d 
	@${RM} ${OBJECTDIR}/touch.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  touch.c  -o ${OBJECTDIR}/touch.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/touch.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/time.o: time.c  .generated_files/flags/default/80e7d62ea78af67491573eb225c7087a381028cd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/time.o.d 
	@${RM} ${OBJECTDIR}/time.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  time.c  -o ${OBJECTDIR}/time.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/time.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/motor.o: motor.c  .generated_files/flags/default/d103b8f6a3aeb3123e7f6e3cc60fe48f980f504a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/motor.o.d 
	@${RM} ${OBJECTDIR}/motor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  motor.c  -o ${OBJECTDIR}/motor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/motor.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/sorting.o: sorting.c  .generated_files/flags/default/e6ea73c848d5d73233c8f354b9a8cd7bc00f7f17 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sorting.o.d 
	@${RM} ${OBJECTDIR}/sorting.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sorting.c  -o ${OBJECTDIR}/sorting.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sorting.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/e364cc4c52652c2a35a120243e0666b2d600429a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/adc.o: adc.c  .generated_files/flags/default/d203b379a1eac580a60946b7d393b9fce598f075 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.o.d 
	@${RM} ${OBJECTDIR}/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  adc.c  -o ${OBJECTDIR}/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/adc.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/touch.o: touch.c  .generated_files/flags/default/687ff05e542632b62263cd0844a5048628545222 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/touch.o.d 
	@${RM} ${OBJECTDIR}/touch.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  touch.c  -o ${OBJECTDIR}/touch.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/touch.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/time.o: time.c  .generated_files/flags/default/9587fed9519805b7552eb166b984b2294b84b719 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/time.o.d 
	@${RM} ${OBJECTDIR}/time.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  time.c  -o ${OBJECTDIR}/time.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/time.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/motor.o: motor.c  .generated_files/flags/default/c0fb151f175b2ec94ac9d0c9f4051f5ecdd4541c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/motor.o.d 
	@${RM} ${OBJECTDIR}/motor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  motor.c  -o ${OBJECTDIR}/motor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/motor.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/sorting.o: sorting.c  .generated_files/flags/default/d4d5f0b682fea362616f4543d9c652b151c65ec4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sorting.o.d 
	@${RM} ${OBJECTDIR}/sorting.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sorting.c  -o ${OBJECTDIR}/sorting.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sorting.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Lab8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../Lab0/lcd.a  
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Lab8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ../Lab0/lcd.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Lab8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../Lab0/lcd.a 
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Lab8.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ../Lab0/lcd.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex ${DISTDIR}/Lab8.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=coff   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
