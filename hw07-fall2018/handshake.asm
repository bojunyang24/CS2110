;;====================================
;; CS 2110 - Fall 2018
;; Homework 7
;; handshake.asm
;;====================================
;; Name: Bojun Yang
;;====================================

; Little reminder from your pals: don't run this directly by pressing
; ``Run'' in complx, since (look below) there's nothing put at address
; x3000. Instead, load it and use ``Debug'' -> ``Simulate Subroutine
; Call'' and choose the ``strlen'' label.

.orig x3000

halt
;R7 is return address. R6 is stack pointer. R5 is current frame pointer.
handshake
    ADD R6, R6, -4  ;from top down: local(if needed), old FP, RA, RV
    ;local placeholder
    STR R5, R6, 1   ;store old FP
    STR R7, R6, 2   ;store return address
    ADD R5, R6, 0   ;set current FP
    ADD R6, R6, -5  ;make space to store the registers
    STR R0, R6, 4   ;save R0
    STR R1, R6, 3   ;save R1
    STR R2, R6, 2   ;save R2
    STR R3, R6, 1   ;save R3
    STR R4, R6, 0   ;save R4

    LDR R0, R5, 4   ;retrieves param. n = R0 <- # of ppl
    BRz BASE

    ADD R6, R6, -1  ;R6.4 make R6 point 1 above where R4 is stored
    ADD R0, R0, -1  ;n-1
    STR R0, R6, 0   ;store n-1 at R6, which is the location of the parameter for the next recursive call
    JSR handshake   ;handshake(n-1)

    LDR R1, R6, 0   ;when the above called subroutine ends R6 will point to the return value R1 <- handshake(n-1)
    ADD R6, R6, 1   ;R6.5 want R6 to be pointing to the top of the register stack of the current stack
    ADD R2, R0, R1  ;(n-1) + handshake(n-1)
    STR R2, R5, 3   ;store the return value
    BR STACK_BREAKDOWN

BASE
    STR R0, R5, 3   ;place return value in RV location of the stack. 3 below R5
    BR STACK_BREAKDOWN

STACK_BREAKDOWN
    LDR R4, R5, -5  ;restore R4
    LDR R3, R5, -4  ;restore R3
    LDR R2, R5, -3  ;restore R2
    LDR R1, R5, -2  ;restore R1
    LDR R0, R5, -1  ;restore R0
    ADD R6, R5, 0   ;R6.6 bring R6 back down to R5 because we just popped the stored registers
    LDR R5, R6, 1   ;store old FP back into R5
    LDR R7, R6, 2   ;store old return address back into R7
    ADD R6, R6, 3   ;R6.7 have R6 point to return value
    RET


; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
.end

; You should not have to LD from any label, take the
; address off the stack instead :)
