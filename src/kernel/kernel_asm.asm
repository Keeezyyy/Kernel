[BITS 64]




; rdi hold void * to stack top
global setStack
setStack:
  pop rax
  mov rsp, rdi
  mov rbp, rsp
  push rax
  ret
