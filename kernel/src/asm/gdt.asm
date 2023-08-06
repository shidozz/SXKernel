global gdtr_install

gdtr_install:
    lgdt [rdi]
    ; met tout les segments avec leurs valeurs ciblants le segment de données
    mov ax, 0x10

    mov ds, ax
    mov es, ax
    mov ss, ax

    mov rax, qword .trampoline ; addresse de retour
    push qword 0x8 ; segment de code
    push rax 

    o64 retf  ; fait un far return

.trampoline:
    pop rbp
    ret
