#ifndef __INTEL_MASHINE_SPESIFIC_REGISTERS_H__
#define __INTEL_MASHINE_SPESIFIC_REGISTERS_H__


#define IA32_BIOS_SIGN_ID 0x08B
#define IA32_MCG_EAX 0x180
#define A32_MCG_EBX 0x181
#define A32_MCG_ECX 0x182
#define A32_MCG_EDX 0x183
#define A32_MCG_ESI 0x184
#define A32_MCG_EDI 0x185
#define A32_MCG_EBP 0x186
#define A32_MCG_ESP 0x187
#define A32_MCG_EFLAGS 0x188
#define A32_MCG_EIP 0x189
#define A32_MCG_MISC 0x18A

#define IA32_MCG_RAX 0x180// Contains state of the RAX register at the time of the machinecheck error.
#define IA32_MCG_RBX 0x181// Contains state of the RBX register at the time of the machinecheck error.
#define IA32_MCG_RCX 0x182// Contains state of the RCX register at the time of the machinecheck error.
#define IA32_MCG_RDX 0x183// Contains state of the RDX register at the time of the machinecheck error.
#define IA32_MCG_RSI 0x184// Contains state of the RSI register at the time of the machinecheck error.
#define  IA32_MCG_RDI 0x185// Contains state of the RDI register at the time of the machinecheck error.
#define IA32_MCG_RBP 0x186// Contains state of the RBP register at the time of the machinecheck error.
#define IA32_MCG_RSP 0x187// Contains state of the RSP register at the time of the machinecheck error.
#define IA32_MCG_RFLAGS 0x188// Contains state of the RFLAGS register at the time of the machine-check error.
#define IA32_MCG_RIP 0x189// Contains state of the RIP register at the time of the machinecheck error.
#define IA32_MCG_MISC 0x18A//IA32_MCG_RSERVED[1:5]18BH-18FHThese registers, if present, are reserved.
#define IA32_MCG_R8 0x190// Contains state of the R8 register at the time of the machinecheck error.
#define IA32_MCG_R9 0x191// Contains state of the R9 register at the time of the machinecheck error.
#define IA32_MCG_R10 0x192// Contains state of the R10 register at the time of the machinecheck error.
#define IA32_MCG_R11 0x193// Contains state of the R11 register at the time of the machinecheck error.
#define IA32_MCG_R12 0x194// Contains state of the R12 register at the time of the machinecheck error.
#define IA32_MCG_R13 0x195// Contains state of the R13 register at the time of the machinecheck error.
#define IA32_MCG_R14 0x196// Contains state of the R14 register at the time of the machinecheck error.
#define IA32_MCG_R15 x197// Contains state of the R15 register at the time of the machinecheck error.

//Table 4-17. MSRs Used By the SYSENTER and SYSEXIT Instructions MSR Address
#define IA32_SYSENTER_CS 0x174
#define IA32_SYSENTER_ESP 0x175
#define IA32_SYSENTER_EIP 0x176

#define IA32_TSC_DEADLINE 0x6E0H
#define IA32_BIOS_SIGN_ID 0x8B
#define IA32_TIME_STAMP_COUNTER 0x10

//IA-32 Architectural MSRs
#define IA32_MISC_ENABLE 0x1A0//  Allows a variety of processor functions to be enabled and disabled.
#define IA32_PEBS_ENABLE 0x3F1// PEBS Control (R/W)
#define IA32_X2PIC_APICID 0x802
#define IA32_X2APIC_VERSION 0x803
#define IA32_X2APIC_TPR 0x808
#define IA32_X2APIC_PPR 0x80A
#define IA32_X2APIC_EOI 0x80B
#define IA32_X2APIC_LDR 0x80D
#define IA32_X2APIC_SIVR 0x80F
#define IA32_X2APIC_ISR0 0x810
#define IA32_X2APIC_ISR1 0x811
#define IA32_X2APIC_ISR2 0x812
#define IA32_X2APIC_ISR3 0x813
#define IA32_X2APIC_ISR4 0x814
#define IA32_X2APIC_ISR5 0x815
#define IA32_X2APIC_ISR6 0x816
#define IA32_X2APIC_ISR7 0x817
#define IA32_X2APIC_TMR0 0x818
#define IA32_X2APIC_TMR1 0x819
#define IA32_X2APIC_TMR2 0x81A
#define IA32_X2APIC_TMR3 0x81B
#define IA32_X2APIC_TMR4 0x81C
#define IA32_X2APIC_TMR5 0x81D
#define IA32_X2APIC_TMR6 0x81E
#define IA32_X2APIC_TMR7 0x81F
#define IA32_X2APIC_IRR0 0x820
#define IA32_X2APIC_IRR1 0x821
#define IA32_X2APIC_IRR2 0x822
#define IA32_X2APIC_IRR3 0x823
#define IA32_X2APIC_IRR4 0x824
#define IA32_X2APIC_IRR5 0x825
#define IA32_X2APIC_IRR6 0x826
#define IA32_X2APIC_IRR7 0x827
#define IA32_X2APIC_ESR  0x828
#define IA32_X2APIC_LVT_CMCI 0x82F
#define IA32_X2APIC_ICR 0x830
#define IA32_X2APIC_LVT_TIMER 0x832
#define IA32_X2APIC_LVT_THERMAL 0x833
#define IA32_X2APIC_LVT_PMI 0x834
#define IA32_X2APIC_LVT_LINT0 0x835
#define IA32_X2APIC_LVT_LINT1 0x836
#define IA32_X2APIC_LVT_ERROR 0x837
#define IA32_X2APIC_INIT_COUNT 0x838
#define IA32_X2APIC_CUR_COUNT 0x839
#define IA32_X2APIC_DIV_CONF 0x83E
#define IA32_X2APIC_SELF_IPI 0x83F
#define MSR_LASTBRANCH_TOS 0x1C9
#define MSR_PEBS_ENABLE 0x3F1

//	ON NEHALEM
#define MSR_PEBS_LD_LAT 0x3F6 
#define MSR_PKG_C3_RESIDENCY 0x3F8
#define MSR_PKG_C6_RESIDENCY 0x3F9
#define MSR_PKG_C7_RESIDENCY 0x3FA 
#define MSR_CORE_C3_RESIDENCY 0x3FC
#define MSR_CORE_C6_RESIDENCY 0x3FD
#define MSR_PKG_CST_CONFIG_CONTROL 0xE2
#define MSR_PMG_IO_CAPTURE_BASE E4H
#define IA32_ENERGY_PERF_BIAS 1B0H
#define IA32_PACKAGE_THERM_INTERRUPT 1B2H
#define POWER_CTL 1FCH
#define IA32_MC0_CTL2 280H
#define IA32_MC1_CTL2 281H
#define IA32_MC2_CTL2 282H
#define IA32_MC3_CTL2 283H
#define MSR_MC4_CTL2 284H
#define IA32_MC0_CTL 400H
#define IA32_MC0_STATUS 401H
#define IA32_MC0_ADDR 402H
#define IA32_MC0_MISC 403H
#define IA32_MC1_CTL 404H
#define IA32_MC1_STATUS 405H
#define IA32_MC1_ADDR 46H
#define IA32_MC1_MISC 407H
#define IA32_MC2_CTL 408H
#define IA32_MC2_STATUS 409H
#define IA32_MC2_ADDR 40AH
#define IA32_MC2_MISC 40BH
#define IA32_MC3_CTL 40CH
#define IA32_MC3_STATUS 40DH
#define IA32_MC3_ADDR 40EH
#define IA32_MC3_MISC 40FH
#define MSR_MC4_CTL 410H
#define IA32_MC4_STATUS 411H
#define MSR_RAPL_POWER_UNIT 606H
#define MSR_PKGC3_IRTL 60AH
#define MSR_PKGC6_IRTL 60BH
#define MSR_PKGC7_IRTL 60CH
#define MSR_PKG_RAPL_POWER_LIMIT 610H
#define MSR_PKG_ENERY_STATUS 611H
#define MSR_PKG_PERF_STATUS 613H
#define MSR_PKG_POWER_INFO 614H
#define MSR_PP0_POWER_LIMIT 638H
#define MSR_PP0_ENERY_STATUS 639H
#define MSR_PP0_POLICY 63AH
#define MSR_PP0_PERF_STATUS 63BH
#define IA32_TSC_DEADLINE 6E0H
#define IA32_MC5_CTL2 285H
#define IA32_MC6_CTL2 286H
#define IA32_MC7_CTL2 0x287
#define IA32_MC8_CTL2 0x288
#define A32_MC9_CTL2 0x289
#define IA32_MC10_CTL2 0x28A
#define IA32_MC11_CTL2 0x28B
#define IA32_MC12_CTL2 0x28C
#define IA32_MC13_CTL2 0x28D
#define IA32_MC14_CTL2 0x28E
#define IA32_MC15_CTL2 0x28F
#define IA32_MC16_CTL2 0x290
#define IA32_MC17_CTL2 0x291
#define IA32_MC18_CTL2 0x292
#define IA32_MC19_CTL2 0x293
#define MSR_MC5_CTL 0x414
#define MSR_MC5_STATUS 0x415
#define MSR_MC5_ADDR 0x416
#define MSR_MC5_MISC 0x417
#define MSR_MC6_CTL 0x418
#define SR_MC6_STATUS 0x419
#define SR_MC6_ADDR 0x41A
#define SR_MC6_MISC 0x41B
#define MSR_MC7_CTL 0x41C
#define MSR_MC7_STATUS 0x41D
#define MSR_MC7_ADDR 0x41E
#define MSR_MC7_MISC 0x41F
#define MSR_MC8_CTL 0x420
#define MSR_MC8_STATUS 0x421
#define MSR_MC8_ADDR 0x422
#define MSR_MC8_MISC 0x423
#define MSR_MC9_CTL 0x424
#define MSR_MC9_STATUS 0x425
#define MSR_MC9_ADDR 0x426
#define MSR_MC9_MISC 0x427
#define MSR_MC10_CTL 0x428
#define MSR_MC10_STATUS 0x429
#define MSR_MC10_ADDR 0x42A
#define MSR_MC10_MISC 0x42B
#define MSR_MC11_CTL 0x42C
#define MSR_MC11_STATUS 0x42D
#define MSR_MC11_ADDR 0x42E
#define MSR_MC11_MISC 0x42F
#define MSR_MC12_CTL 0x430
#define MSR_MC12_STATUS 0x431
#define MSR_MC12_ADDR 0x432
#define MSR_MC12_MISC 0x433
#define MSR_MC13_CTL 0x434
#define MSR_MC13_STATUS 0x435
#define MSR_MC13_ADDR 0x436
#define MSR_MC13_MISC 0x437
#define MSR_MC14_CTL 0x438
#define MSR_MC14_STATUS 0x439
#define MSR_MC14_ADDR 0x43A
#define MSR_MC14_MISC 0x43B
#define MSR_MC15_CTL 0x43C
#define MSR_MC15_STATUS 0x43D
#define MSR_MC15_ADDR 0x43E
#define MSR_MC15_MISC 0x43F
#define MSR_MC16_CTL 0x440
#define MSR_MC16_STATUS 0x441
#define MSR_MC16_ADDR 0x442
#define MSR_MC16_MISC 0x443
#define MSR_MC17_CTL 0x444
#define MSR_MC17_STATUS 0x445
#define MSR_MC17_ADDR 0x446
#define MSR_MC17_MISC 0x447
#define MSR_MC18_CTL 0x448
#define MSR_MC18_STATUS 0x449
#define MSR_MC18_ADDR 0x44A
#define MSR_MC18_MISC 0x44B
#define MSR_MC19_CTL 0x44C
#define MSR_MC19_STATUS 0x44D
#define MSR_MC19_ADDR 0x44E
#define MSR_MC19_MISC 0x44F
#define MSR_DRAM_POWER_LIMIT 0x618
#define MSR_DRAM_ENERGY_STATUS 0x619
#define MSR_DRAM_PERF_STATUS 0x61B
#define MSR_DRAM_POWER_INFO 0x61C
#define MSR_LASTBRANCH_TOS 0x1C9
	
#endif	