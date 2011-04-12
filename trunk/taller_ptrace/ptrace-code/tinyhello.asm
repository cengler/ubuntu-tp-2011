section .text

hello db "Hola SO!",10
hello_len equ $-hello

global _start
_start:
  mov eax, 4 ; syscall write
  mov ebx, 1 ; stdout
  mov ecx, hello ; mensaje
  mov edx, hello_len
  int 0x80
  
  mov eax, 1 ; syscall exit
  mov ebx, 0 ;
  int 0x80

