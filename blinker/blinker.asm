;====================================================================
; Main.asm file generated by New Project wizard
;
; Created:   Fri Sep 6 2024
; Processor: AT89C51
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

; Main program
Start:
    mov P2, #0  
main_loop:
    cpl P2.0         
    acall delay    
    sjmp main_loop 

delay:
    mov R2, #255 
outer_loop:
    mov R3, #255  
inner_loop:
    nop            
    nop            
    nop            
    nop            
    djnz R3, inner_loop  
    djnz R2, outer_loop  
    ret            
 ;====================================================================
      END
