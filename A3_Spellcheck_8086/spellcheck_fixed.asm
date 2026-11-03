; Spell check with 8086 string instructions - corrected version
; Changes from spellcheck.asm are marked with ; FIX
ASSUME CS: CODE, DS: DATA, SS: STK

STK SEGMENT STACK                ; FIX: stack segment removes LINK warning L4021
        DW 64 DUP(?)
STK ENDS

DATA SEGMENT
        CHECK_WORD DB 'APPEL     '
        DICT DB 'APPLE     '
             DB 'BANANA    '
             DB 'MANGO     '
             DB 'ORANGE    '
             DB 'GRAPE     '
        DICT_END LABEL BYTE
        MSG_CORRECT DB 'SPELLING CORRECT$'
        MSG_INCORRECT DB 'SPELLING INCORRECT$'
DATA ENDS


CODE SEGMENT
        START:
                MOV AX, DATA
                MOV DS, AX
                MOV ES, AX

                LEA SI, DICT
        CHECK:
                LEA DI, CHECK_WORD
                MOV CX, 10
                CLD
                PUSH SI          ; FIX: CMPSB moves SI, remember the start of this entry
                REPE CMPSB
                POP SI           ; FIX: SI is back at the start of the entry (flags unchanged)
                JE FOUND         ; FIX: ZF=1 only if all 10 bytes matched (JCXZ is wrong
                                 ;      when only the last byte differs, CX is 0 then too)

                ADD SI, 10
                CMP SI, OFFSET DICT_END
                JB CHECK

                LEA DX, MSG_INCORRECT
                JMP PRINT
        FOUND:
                LEA DX, MSG_CORRECT

        PRINT:
                MOV AH, 09H
                INT 21H

                MOV AH, 4CH
                INT 21H
CODE ENDS
END START
