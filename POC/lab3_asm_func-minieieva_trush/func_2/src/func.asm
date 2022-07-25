; implement function in fasm.
; IMPORTANT! Function name should be 'func'!

include 'format/format.inc' 

format ELF64

section '.text' executable

 public func

 func:
    mov r10, 0                       ; counter register

    mov eax, [rdi]                   ; put 1st value from array to temporary reg
    mov [rdx], eax                   ; and then but it to the pointer to min value
    mov [rcx], eax                   ; and to max


    min_max:                         ; find min and values of the array
        inc r10                      ; increment loop counter
        cmp r10, rsi                 ; check if counter equals number of elements in the array
        je mean_prep                 ; if so continue with search of max value

        mov eax, [rdi + (r10*4)]     ; put next value of array to eax register

        cmp eax, [rdx]               ; compare if it is less than the previous smallest value
        jl change                    ; if so jump to "change" and change min value

        cmp eax, [rcx]               ; compare if it is greater than the prevoius greatest value
        jg change_max                ; if so jump to "change_max" and change max value

        jmp min_max

    change:
        mov [rdx], eax
        jmp min_max


    change_max:
        mov [rcx], eax
        jmp min_max

    mean_prep:
        mov r10, 0
        cvtsi2sd xmm0, dword [rdi]             ; here will be stored mean value
        cvtsi2sd xmm1, dword [rdi]             ; here will be stored mean of squared values
        mulsd xmm1, xmm1                       ; first value to the power of 2
        jmp mean

    mean:                                      ; fing mean value and mean of squared values
        inc r10                                ; just loop counter
        cmp r10, rsi
        je finish

        cvtsi2sd xmm2, dword [rdi + (r10*4)]   ; temp register for storing values for mean
        movsd xmm3, xmm2                       ; temp register for storing values for mean of squared values

        mulsd xmm3, xmm3                       ; value to the power of 2
        addsd xmm1, xmm3                       ; add to mean of squared values

        addsd xmm0, xmm2                       ; add to mean

        jmp mean

    finish:
        cvtsi2sd xmm2, rsi                     ; convert array size from 64-bit integer to 64-bit float
        divsd xmm0, xmm2                       ; find mean
        movsd [r8], xmm0                       ; move mean to the value of given pointer

        divsd xmm1, xmm2                       ; find mean of squared values
        mulsd xmm0, xmm0                       ; take mean to the power of 2
        subsd xmm1, xmm0                       ; find variance

        movsd [r9], xmm1                       ; move variance to the value of given pointer

        ret

section '.data' writeable
