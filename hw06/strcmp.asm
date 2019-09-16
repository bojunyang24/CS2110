;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Bojun Yang
;;====================================

.orig x3000

  ;; YOUR CODE HERE :D


            LD R0, STR_ADDR_1   ; starting address of str1
            ADD R0, R0, -1      ; decremement
            LD R1, STR_ADDR_2   ; starting address of str2
            ADD R1, R1, -1      ; decremement
            AND R4, R4, 0       ; clear the R4 so we can use it as the return

BEGIN       ADD R0, R0, 1       ; increment idx of str1
            LDR R2, R0, 0           ; load the letter from str1
            BRz STR1FIN         ; str1 finished
            BRnp STR1CONT       ; str1 not finished
STR1FIN     ADD R1, R1, 1       ; increment idx of str2
            LDR R3, R1, 0           ; load letter from str2
            BRz EQUAL           ; str2 == str1 return 0
            BRnp STR1WIN        ; str1 < str2 return -1

STR1CONT    ADD R1, R1, 1       ; increment idx of str2
            LDR R3, R1, 0           ; load letter from str2
            BRz STR2WIN         ; str2 < str1 return 1
            BRnp COMPARE        ; need to compare the chars

EQUAL       ST R4, ANSWER
            BR PASS

STR1WIN     ADD R4, R4, -1
            ST R4, ANSWER
            BR PASS

STR2WIN     ADD R4, R4, 1
            ST R4, ANSWER
            BR PASS
; R2 str1 and R3 is str2
COMPARE     NOT R3, R3              ; not R3 so we can subtract it
            ADD R3, R3, 1                   ; R3 is now negative
            ADD R5, R2, R3                  ; if R2 - R3
            BRn STR1WIN                     ; < 0 then STR2 is first
            BRp STR2WIN                     ; > 0 then STR1 is first
            BRz BEGIN                       ; == 0 then STR1 and STR2 are equal at this index

PASS        HALT

STR_ADDR_1 .fill x4000
STR_ADDR_2 .fill x4050

ANSWER     .blkw 1

.end

.orig x4000
  .stringz "This is a test"
.end

.orig x4050
  .stringz "This is a rest"
.end
