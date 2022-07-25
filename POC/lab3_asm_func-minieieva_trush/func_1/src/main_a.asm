; implement calling your 'func' function from fasm main.
include 'format/format.inc'

format ELF64

section '.text' executable

 public main
 extrn func
 extrn printf
 extrn exit

main:
    push RBP                      ; align stack

    ; Basically print the message before the sort
    mov RDI, msg_before           ; Move the message before to rdi
    mov RSI, QWORD [lst]          
    mov RDX, QWORD [lst + 4]
    mov RCX, QWORD [lst + 8] 
    mov R8, QWORD [lst + 12] 

    call printf

    ; Prepare for sort, pointer to list in rdi, size in rsi
    mov RDI, lst                  ; put list of values to rdi
    mov RSI, QWORD [size]               ; put its size to rsi

    call func                     ; call func and sort the array

    ; Call print on the sorted array, the same principle
    mov RDI, msg_before
    mov RSI, QWORD [lst]
    mov RDX, QWORD [lst + 4]
    mov RCX, QWORD [lst + 8] 
    mov R8, QWORD [lst + 12] 


    call printf

    xor     EDI, EDI              ; exit code
    call    exit

section '.data' writeable

lst dd 9, 7, 3, 4
size dq 4

msg_before db "Before sort Arr: [%d, %d, %d, %d]",0xA,0
msg_after db "Before sort Arr: [%d, %d, %d, %d]",0xA,0

