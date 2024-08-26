
.intel_syntax noprefix

// RDI: faulting IP
.global returnFromSignalTramp2
.type returnFromSignalTramp2, %function
returnFromSignalTramp2:
        .cfi_startproc

        push    rbp
        .cfi_adjust_cfa_offset 8
        .cfi_rel_offset rbp, 0

        mov     rbp, rsp
        .cfi_def_cfa_register rbp
        .cfi_def_cfa_offset 16

        call returnFromSignalHandlerWorker

        int 3

        .size returnFromSignalTramp2, .-returnFromSignalTramp2
        .cfi_endproc

// RDI: faulting IP
.global returnFromSignalTramp
.type returnFromSignalTramp, %function
returnFromSignalTramp:
        .cfi_startproc simple
        .cfi_signal_frame
        
        mov     rax, rsp        // save the faulting RSP

        // Align the stack towards zero
        and     rsp, -16

        push    rdi
        push    rax
        .cfi_def_cfa %rsp, 0
        .cfi_offset %rip, 8
        .cfi_offset %rsp, 0

        call returnFromSignalTramp2

        int 3

        .size returnFromSignalTramp, .-returnFromSignalTramp
        .cfi_endproc

.section        .note.GNU-stack,"",@progbits
