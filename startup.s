.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.section .isr_vector, "a", %progbits
.type isr_vector, %object
.size isr_vector, .-isr_vector

isr_vector:
    .word _estack
    .word Reset_Handler


.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    bl main
    b .

.size Reset_Handler, .-Reset_Handler

