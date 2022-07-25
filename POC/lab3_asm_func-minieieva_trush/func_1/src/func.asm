; implement function in fasm.
; IMPORTANT! Function name should be 'func'!

include 'format/format.inc' 

format ELF64

section '.text' executable

 public func


 func:
    push RBP
    push RBX; Push the RBX since it is the register that we need to save

    ; R8 = i, R9 = j, R10 = A[j], R11 = A[j+1]
    ; Clear the possible trash in registers
    mov     R8, 0
    mov     R9, 0
    mov     R10, 0
    mov     R11, 0

    ; Move Array pointer and length to different registers to be able to call swap
    mov     RCX, RDI
    mov     RDX, RSI
    dec     RDX         ; Decrenent the array len by 1

    outer_loop:

        ; R9 or j is 0 at the start of each loop
        mov     R9, 0
        ; We compare R8 to the length of the array, if equal => we end the program
        cmp     R8, RDX
        je end_of_program

        ; The inner loop of a program
        inner_loop:

            cmp     R9, RDX
            je prepare_for_outer_loop

            ; Compare A[j] and A[j+1]
            mov     R10D, [RCX + 4*R9]
            mov     R11D, [RCX + 4*R9 + 4]

            cmp     R10D, R11D
            jg prepare_for_swap

            ; If couldn't swap, just move on
            inc     R9
            jmp     inner_loop
    
    prepare_for_outer_loop:
        ; just move to another iteration
        inc R8
        jmp outer_loop

    ; Prepare for callig swap function by writing to RDI and RSI
    prepare_for_swap:
        lea     RDI, [RCX + 4*R9]
        lea     RSI, [RCX + 4*R9 + 4]
        call    swap
        inc     R9
        jmp     inner_loop

    ; End the program
    end_of_program:
        pop RBX ; Get the value of pushed RBX
        pop RBP
        ret


; A swap function which accepts to pointers and switches them places
swap:
    mov     EAX, [RDI]
    mov     EBX, [RSI]

    mov     [RDI], EBX
    mov     [RSI], EAX

    ret
