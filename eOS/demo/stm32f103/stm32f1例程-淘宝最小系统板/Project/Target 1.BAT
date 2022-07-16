SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;C:\Program Files (x86)\NetSarang\Xshell 7\;C:\Program Files (x86)\NetSarang\Xftp 7\;C:\Program Files\PuTTY\;C:\Users\86151\AppData\Local\Microsoft\WindowsApps;C:\Users\86151\AppData\Local\Programs\Microsoft VS Code\bin
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\led.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\objects\startup_stm32f10x_hd._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\core_cm3.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\system_stm32f10x.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\misc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\eos.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\eos_main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\objects\eos_port1._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\eos_port2.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\eos_plugin_shell.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\Objects\eOS.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\Objects\eOS.axf" --i32combined --output ".\Objects\eOS.hex"
