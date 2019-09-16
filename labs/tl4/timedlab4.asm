
;;====================================================
;; CS 2110 - Fall 2018
;; Timed Lab 4
;; converge.asm
;;====================================================
;; Name: Bojun Yang
;;====================================================

.orig x3000

;; Don't try to run this code directly, since it only contains
;; subroutines that need to be invoked using the LC-3 calling
;; convention. Use Debug > Setup Test or Simulate Subroutine
;; Call in complx instead.
;;
;; Do not remove this line or you will break...
;; 'Simulate Subroutine Call'

halt

converge
    ADD R6, R6, -4  ;from top down: local(if needed), old FP, RA, RV
    STR R5, R6, 1   ;store old FP
    STR R7, R6, 2   ;store return address
    ADD R5, R6, 0   ;set current FP
    ADD R6, R6, -5  ;make space to store the registers
    STR R0, R6, 4   ;save R0
    STR R1, R6, 3   ;save R1
    STR R2, R6, 2   ;save R2
    STR R3, R6, 1   ;save R3
    STR R4, R6, 0   ;save R4

    AND R2, R2, 0
    ADD R2, R2, 2   ;initialize 2 in R2

    LD R3, DIV_ADDR

    LDR R0, R5, 4   ;retrieves param n. R0 = n
    ADD R1, R0, -1  ;check if n==1
    BRz BASE

    ADD R6, R6, -1
    STR R2, R6, 0   ;store 2 for next stack call
    ADD R6, R6, -1
    STR R0, R6, 0   ;store n in mem above n for next stack call
    JSRR R3         ;R6 will point to n/2
    LDR R1, R6, 0   ;R1 = div

    AND R4, R0, 1   ;R4 = mod
    BRz MODZERO
    ADD R1, R0, R0  ;R1 = n+n
    ADD R1, R1, R0  ;R1 = n+n+n
    ADD R1, R1, 1   ;R1 = (3n) + 1
    ADD R6, R6, -1
    STR R1, R6, 0   ;store param for the next converge call
    JSR converge
   	LDR R2, R6, 0
    ADD R1, R2, 1  ;converge((3n) + 1) + 1
    BR BASE


MODZERO
    ADD R6, R6, -1
    STR R1, R6, 0   ;store param for the next converge call
    JSR converge 	;R6 points to converge(div) after
    LDR R2, R6, 0	;converge(div)
    ADD R1, R2, 1   ;converge(div) + 1
    BR BASE

BASE
    STR R1, R5, 3   ;place return value in RV location of the stack
    BR CONVERGE_BREAKDOWN

CONVERGE_BREAKDOWN
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

; [ implement converge here ]

STACK    .fill xF000
DIV_ADDR .fill x6000 ;; Call the divide subroutine at
                     ;; this address!

.end

;;====================================================
;;   divide(n, d): Takes a numerator (n) and
;;                 denominator (d), returning n / d.
;;====================================================
.orig x6000
divide  ;; DO NOT call JSR with this label! Use DIV_ADDR instead!
  .fill x1DBD
  .fill x7F81
  .fill x7B80
  .fill x1BBF
  .fill x1DBB
  .fill x7140
  .fill x737F
  .fill x757E
  .fill x777D
  .fill x797C
  .fill x6144
  .fill x6345
  .fill x54A0
  .fill x1620
  .fill x987F
  .fill x1921
  .fill x1903
  .fill x0805
  .fill x14A1
  .fill x987F
  .fill x1921
  .fill x16C4
  .fill x0FF7
  .fill x7543
  .fill x6140
  .fill x637F
  .fill x657E
  .fill x677D
  .fill x697C
  .fill x1D61
  .fill x6B80
  .fill x1DA1
  .fill x6F80
  .fill x1DA1
  .fill xC1C0
.end
