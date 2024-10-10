org   0000h
      jmp   Start

;====================================================================
; CODE SEGMENT
;====================================================================

      org   0100h
Start:
    mov th1, #253
    mov tmod, #20h
    mov scon, #50h
    setb tr1

loop:
    JNB RI, $     
    MOV A, SBUF   
    mov p2, A
    CLR RI        
    JMP loop      
end