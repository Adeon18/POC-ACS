; implement calling your 'func' function from fasm main.
include 'format/format.inc'

format ELF64

section '.text' executable

 public main
 extrn func
 extrn printf
 extrn exit

 main:
     push rbp                      ; align stack
     mov rdi, lst                  ; put list of values to rdi
     mov rsi, [size]               ; put its size to rsi
     mov rdx, min                  ; put pointer to future min value to rdx
     mov rcx, max                  ; put pointer to future max value to rcx
     mov r8, mean                  ; put pointer to future mean value to r8
     mov r9, variance              ; put pointer to future variance value to r9

     call func                     ; call func and calculate all the data

     mov rdi, msg_1                ; move message to rdi to print it
     mov rsi, QWORD [min]          ; move min value to rsi to print it

     call printf

     mov rdi, msg_2
     mov rsi, QWORD [max]

     call printf

     mov rdi, msg_3
     movsd xmm0, QWORD [mean]      ; put value of mean to xmm0 to print it
                                   ; (apparently "printf" takes float values form there)
     call printf

     mov rdi, msg_4
     movsd xmm0, QWORD [variance]

     call printf

     xor     edi, edi              ; exit code
     call    exit

section '.data' writeable

lst dd -2, -2, -3, -4, -5, -6, -7
size dq 7
min dd ?
max dd ?
mean dq ?
variance dq ?

msg_1 db "Min: %d",0xA,0
msg_2 db "Max: %d",0xA,0
msg_3 db "Mean: %f",0xA,0
msg_4 db "Variance: %f",0xA,0
