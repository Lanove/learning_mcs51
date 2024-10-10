;====================================================================
; Main.asm file generated by New Project wizard
;
; Created:   Fri Sep 6 2024
; Processor: AT89C51
; Compiler:  ASEM-51 (Proteus)
;====================================================================

$NOMOD51
$INCLUDE (8051.MCU)

;====================================================================
; DEFINITIONS
;====================================================================

;====================================================================
; VARIABLES
;====================================================================

;====================================================================
; RESET and INTERRUPT VECTORS
;====================================================================

      ; Reset Vector
      org   0000h
      jmp   Start

;====================================================================
; CODE SEGMENT
;====================================================================

      org   0100h

; Main program
Start:
    ; Initiate as 500 (1ms), should end at 2500 (2ms)
    mov r0, #0f4h ; Low byte
    mov r1, #001h ; High byte
    ; r2 and r3 is used to decrement or increment r0r1
    mov r2, #200d ; Low byte
    mov r3, #000h ; High byte
    ; Don't use r4 and r5 as it's used for remaining th and tl
    ; r6 used as increment prescaler
    ; p2.7 used as increment/decrement flag
    mov tmod,#011h ; Configure both as 16-bit timer
    setb p2.7

reload_polarity:
    mov r7, #10 ; switch the polarity of increment/decrement after r7*r6*20ms 

reload_prescaler:
   mov r6, #25
   djnz r7, main_loop
   cpl p2.7
   jmp reload_polarity

main_loop:
    acall load_th_tl
    clr tf1
    setb tr1
    setb p2.6
    jnb tf1,$
    clr p2.6
    acall remaining_th_tl
    clr tf1
    jnb tf1,$
    djnz r6, main_loop
    acall increment_or_decrement
    jmp reload_prescaler

increment_or_decrement:
    jb p2.7, increment_16b ; If p2.7 is 1, increment r0r1
    acall decrement_16b ; If p2.7 is 0, decrement r0r1
    ret

 ; increment r0r1 with value on r2r3
increment_16b:
    mov A, R0
    add A, R2
    mov R0, A
    mov A, R1
    addc A, R3
    mov R1, A
    ret

; decrement r0r1 with value on r2r3
decrement_16b:
    mov A, R0
    subb A, R2
    mov R0, A
    mov A, R1
    subb A, R3
    mov R1, A
    ret

load_th_tl:
    ; Calculate 65535 - (R0R1)
    mov A, R0
    cpl A
    mov tl1, A
    mov A, R1
    cpl A
    mov th1, A
    ret
    
remaining_th_tl:
    ; Calculate 65535 - (20000 - (R0R1))
    mov A, #020h ; Low byte of 20000
    subb A, R0
    mov R4, A
    mov A, #04eh ; High byte of 20000
    subb A, R1
    mov R5, A

    ; Calculate 65535 - (R4, R5)
    mov A, R4
    cpl A
    mov TL1, A
    mov A, R5
    cpl A
    mov TH1, A

    ret
 ;====================================================================
      END
; 0100 1110 0010 0000 +20000
; 0000 0011 1110 1000 +1000
; -
; 0100 1010 0011 1000 +19000
