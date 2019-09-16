;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Bojun Yang
;;====================================

.orig x3000

  ;; YOUR CODE HERE :D

        LD R0, ARRAY_ADDR   ; address of first element in R0. initial i
        ADD R0, R0, -1      ; increment at beginning so need to decremenet once
        LD R1, ARRAY_LEN    ; length of array in R1, also the counter
        ADD R1, R1, -1
LOOPI   ADD R0, R0, 1       ; increment i
        ADD R4, R0, 0       ; represents minIdx. will be the first i at the beginning of each LOOPI
        LDR R2, R0, 0       ; load first element in this iteration of i, represents the current minValue

        ST R0, VARI         ; store the current ith address so we can use R0 as j
        ST R1, VARC         ; store current counter so we can use R1 for j's counter

LOOPJ   ADD R0, R0, 1       ; R0 becomes j. increments
        LDR R3, R0, 0       ; load the jth element in the array
        NOT R5, R2          ; NOT the current min value
        ADD R5, R5, 1       ; R5 = (- min value)
        ADD R6, R3, R5      ; jth element - min value; use this to check if jth value is smaller
        BRzp CONT
        ADD R4, R0, 0       ; stores new minIdx in the minIdx register (R4)
        ADD R2, R3, 0       ; stores new minvalue
CONT    ADD R1, R1, -1      ; decrease counter for j
        BRp LOOPJ           ; if counter > 0 then LOOPJ, if not LOOPI
        LD R0, VARI         ; laod the saved i back into R0, R0 is i again
        LD R1, VARC         ; load the saved counter back into R1, R1 is counter for i again
        ; R4 holds the address to new min val
        ; R2 holds the value of the new min val
        ; R0 holds the address to old min val
        LDR R7, R0, 0       ; loads the value of old min val into R7
        STR R7, R4, 0       ; stores the old min value into the address where the new min value was
        STR R2, R0, 0       ; stores the new min value into the address of old min vals
        ADD R1, R1, -1      ; decrease counter
        BRp LOOPI


PASS HALT

ARRAY_ADDR .fill x4000
ARRAY_LEN  .fill 10
VARI       .fill x7FFF
VARC       .fill x8000
.end

.orig x4000
  .fill 7
  .fill 18
  .fill 0
  .fill 5
  .fill 9
  .fill 25
  .fill 1
  .fill 2
  .fill 10
  .fill 6
.end
