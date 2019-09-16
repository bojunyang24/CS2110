;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Bojun Yang
;;====================================

.orig x3000

  ;; YOUR CODE HERE :D

            LD R1, MAX_INT      ; sets R1 max temporarily
            LD R2, MIN_INT      ; sets R2 min temporarily
            LD R0, HEAD_ADDR    ; points to the next of the node
            BRz INITDONE        ; if null then return
            ; LDR R1, R0, 1
            ; LDR R2, R0, 1
            ; LDR R1, R0, 0       ; gets the address of the next node
            ; BRz INITDONE
            BR CONTINUE

INITDONE    ST R1, ANSWER_MAX
            ST R2, ANSWER_MIN
            BR PASS

CONTINUE    LDR R6, R0, 1       ; gets the value of the fist node and sets as max (R6)
            LDR R7, R0, 1       ; gets the value of the fist node and sets as min (R7)
            BR NEXT

NEXT        LDR R0, R0, 0       ; gets the address of the next node
            BRz DONE
            LDR R3, R0, 1       ; gets value of curr node
            NOT R4, R3          ; NOTs the value
            ADD R4, R4, 1       ; R4 contains negative curr node's value
            ADD R5, R6, R4      ; MAX - new
            BRn REPLACEMAX
            ADD R5, R7, R4      ; MIN - new
            BRp REPLACEMIN
            BR NEXT

DONE        ST R6, ANSWER_MAX
            ST R7, ANSWER_MIN
            BR PASS

REPLACEMAX  ADD R6, R3, 0
            BR NEXT

REPLACEMIN  ADD R7, R3, 0
            BR NEXT

PASS        HALT

HEAD_ADDR  .fill x4000

MAX_INT    .fill x7FFF
MIN_INT    .fill x8000

ANSWER_MAX .blkw 1
ANSWER_MIN .blkw 1

.end

.orig x4000
  .fill x4002         ;; Node 1
  .fill 4
  .fill x4004         ;; Node 2
  .fill 5
  .fill x4006         ;; Node 3
  .fill 25
  .fill x4008         ;; Node 4
  .fill 1
  .fill x0000         ;; Node 5
  .fill 10
.end
