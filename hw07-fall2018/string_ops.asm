;;====================================
;; CS 2110 - Fall 2018
;; Homework 7
;; string_ops.asm
;;====================================
;; Name: Bojun Yang
;;====================================

; Little reminder from your pals: don't run this directly by pressing
; ``Run'' in complx, since (look below) there's nothing put at address
; x3000. Instead, load it and use ``Debug'' -> ``Simulate Subroutine
; Call'' and choose the ``strlen'' label.

.orig x3000

halt

strlen
    ADD R6, R6, -3   ;initialize space for everything
    STR R7, R6, 1    ;store RA
    STR R5, R6, 0    ;store old FP
    ADD R6, R6, -4   ;initialize space for registers
    ADD R5, R6, 3    ;place new FP
    STR R0, R6, 3    ;store R0 on the stack
    STR R1, R6, 2    ;store R1 on the stack
    STR R2, R6, 1    ;store R2 on the stack
    STR R3, R6, 0    ;store R3 on the stack

    LDR R0, R5, 4    ;load the address of string into R0
    AND R1, R1, 0    ;reset R1 to 0, it'll be the length

LOOPSTRING
    ADD R2, R0, R1   ;R2 -> address of current character
    LDR R2, R2, 0    ;R2 -> value of current character

    BRZ ENDSTRING    ;if value of character is 0, reached the end

    ADD R1, R1, 1    ;increment string length
    BR LOOPSTRING

ENDSTRING
    STR R1, R5, 3    ;store strlen at the return value spot


TEARDOWN
    LDR R3, R6, 0   ;reload R3
    LDR R2, R6, 1   ;reload R2
    LDR R1, R6, 2   ;reload R1
    LDR R0, R6, 3   ;reload R0
    ADD R6, R6, 4   ;add the stack pointer
    LDR R5, R6, 0   ;reload old FP
    LDR R7, R6, 1   ;reload return address
    ADD R6, R6, 2   ;put the stack pointer at the rv
    RET



count_occurrence
    ADD R6, R6, -3  ;initialize space for everything
    STR R7, R6, 1   ;store RA
    STR R5, R6, 0   ;store old FP
    ADD R6, R6, -5  ;initialize space for registers, an extra to put str addr for strlen
    ADD R5, R6, 3   ;place new FP
    STR R0, R6, 3   ;store R0 on the stack
    STR R1, R6, 2   ;store R1 on the stack
    STR R2, R6, 1   ;store R2 on the stack
    STR R3, R6, 0   ;store R3 on the stack

    LDR R0, R6, 8   ;R0 <- first char addr
    STR R0, R6, 0   ;store R0 for strlen use

    JSR strlen

    LDR R0, R5, 5   ;R0 <- current char addr (initialized to the first char)
    AND R1, R1, 0   ;reset R1 to 0, counter for num of occurences
    LDR R2, R5, 6   ;R2 <- char to look for
    NOT R2, R2
    ADD R2, R2, 1   ;R2 <- negative ascii of char to loock for, -c

LOOP
    LDR R3, R0, 0   ;R3 <- current char
    BRz DONE
    ADD R3, R3, R2  ;curr char - c
    BRnp PASS
    ADD R1, R1, 1   ;+1 occurence
PASS
    ADD R0, R0, 1   ;increment current char addr
    BR LOOP


DONE
    STR R1, R5, 4   ;store return value

COUNT_BREAKDOWN
    ADD R6, R6, 2   ;shift R6 to correct location
    LDR R3, R6, 0   ;reload R3
    LDR R2, R6, 1   ;reload R2
    LDR R1, R6, 2   ;reload R1
    LDR R0, R6, 3   ;reload R0
    ADD R6, R6, 4   ;add the stack pointer
    LDR R5, R6, 0   ;reload old FP
    LDR R7, R6, 1   ;reload return address
    ADD R6, R6, 2   ;put the stack pointer at the rv
    RET
; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
.end

; You should not have to LD from any label, take the
; address off the stack instead :)
