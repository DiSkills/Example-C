global open
global close
global read
global write
global errno


section .bss
errno resd 1


section .text
generic_syscall_1:
        push ebp
        mov ebp, esp

        push ebx

        mov ebx, [ebp + 8]
        int 80h

        mov edx, eax
        and edx, 0fffff000h
        cmp edx, 0fffff000h
        jne .quit
        
        mov [errno], eax
        mov eax, -1
.quit:
        pop ebx

        mov esp, ebp
        pop ebp
        ret


generic_syscall_3:
        push ebp
        mov ebp, esp

        mov ecx, [ebp + 12]
        mov edx, [ebp + 16]

        push dword [ebp + 8]
        call generic_syscall_1
        add esp, 4
.quit:
        mov esp, ebp
        pop ebp
        ret


read:
        mov eax, 3
        jmp generic_syscall_3

write:
        mov eax, 4
        jmp generic_syscall_3

open:
        mov eax, 5
        jmp generic_syscall_3

close:
        mov eax, 6
        jmp generic_syscall_1
