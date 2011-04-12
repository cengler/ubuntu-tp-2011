section .text

global _start
_start:
  mov eax, 1  ; syscall exit
  mov ebx, 42 ;
  int 0x80

