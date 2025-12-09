[BITS 64]

; rdi -> holds pointer to PML4
global set_cr3
set_cr3:
  mov cr3, rdi
  ret
