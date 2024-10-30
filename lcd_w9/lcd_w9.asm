;====================================================================
; Main.asm file generated by New Project wizard
;
; Created:   Thu Apr 25 2024
; Processor: 80C31
; Compiler:  ASEM-51 (Proteus)
;====================================================================

$NOMOD51
$INCLUDE (80C52.MCU)

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

LCD_RS equ p1.0
LCD_EN equ p1.1
LCD_PORT equ p3

Start:	
    call lcd_initialize
    mov r0, #'H'
    call lcd_send_data
    mov r0, #'e'
    call lcd_send_data
    mov r0, #'l'
    call lcd_send_data
    mov r0, #'l'
    call lcd_send_data
    mov r0, #'o'
    call lcd_send_data
    mov r0, #' '
    call lcd_send_data
    mov r0, #'F'
    call lcd_send_data
    mov r0, #'i'
    call lcd_send_data
    mov r0, #'g'
    call lcd_send_data
    mov r0, #'o'
    call lcd_send_data
Loop:	
    jmp Loop

lcd_initialize:
      mov r0,#00111000b
      call lcd_send_instruction
      mov r0,#00000110b
      call lcd_send_instruction
      mov r0,#00001100b
      call lcd_send_instruction
      mov r0,#00000001b
      call lcd_send_instruction
      ret

lcd_clear:
      mov r0,#00000001b
      call lcd_send_instruction
      ret

lcd_send_instruction:
      clr LCD_RS
      jmp lcd_clock
lcd_send_data:
      setb LCD_RS
lcd_clock:
      setb LCD_EN
      mov LCD_PORT,r0
      call lcd_delay
      clr LCD_EN
      setb LCD_EN
      call lcd_delay
      ret

lcd_delay:
    mov r6, #10      
outer_loop:
    mov r7, #200     
inner_loop:
    djnz r7, inner_loop
    djnz r6, outer_loop
    ret
;====================================================================
      END