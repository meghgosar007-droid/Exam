ASSUME CS: CODE, DS: DATA

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
                REPE CMPSB
                JCXZ FOUND

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
