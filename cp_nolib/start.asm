global _start
extern main

section .text
_start:
        mov ecx, [esp]          ; argc in ecx
        mov eax, esp
        add eax, 4              ; argv in eax

        push eax
        push ecx
        call main               ; main(argc, argv)
        add esp, 8

        mov ebx, eax
        mov eax, 1
        int 80h                 ; call _exit
