section .text

hello db "Hola SO!",10
hello_len equ $-hello

global _start
_start:
  mov rax, 1 ; syscall write
  mov rdi, 1 ; stdout
  mov rsi, hello ; mensaje
  mov rdx, hello_len
  syscall
  
  mov rax, 60 ; syscall exit
  mov rdi, 0 ;
  syscall

