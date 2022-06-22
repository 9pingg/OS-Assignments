section .bss
    reserveData resb 100
    reservePos resb 8
section .text    
    global B
    extern C 
    global _start
    
B:
    push rdi
    mov rax, 1
    mov rdi, 1
    mov rsi, text
    mov rdx, 50
    syscall
    pop rdi
    call print
    ;;;;;;;;;
 
 
print:
 
    mov rax, rdi
    call _methodShift
 
    mov rbp, $C
    push rbp
    ret
 
 
_methodShift:
    mov rcx, reserveData
    mov rbx, 10
    mov [rcx], rbx
    inc rcx
    mov [reservePos], rcx
 
_methodPrint1:
    push rax
    and rax, 0xFF
    mov rcx, [reservePos]
    mov [rcx], al
    inc rcx
    mov [reservePos], rcx
    pop rax
    shr rax, 8
    cmp rax, 0
    jne _methodPrint1
 
_methodPrint2:
    mov rcx, [reservePos]
 
    mov rax, 1
    mov rdi, 1
    mov rsi, rcx
    mov rdx, 1
    syscall
 
    mov rcx, [reservePos]
    dec rcx
    mov [reservePos], rcx
 
    cmp rcx, reserveData
    jge _methodPrint2
    ret
    ;;;;;;;;;
section .data
    text dw  "In B", 10,"printing the ASCII characters: "