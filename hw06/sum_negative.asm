;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Bojun Yang
;;====================================

.orig x3000

  ;; YOUR CODE HERE :D
AND R1, R1, 0 ; ANSWER = 0
LD R3, ARRAY_ADDR ; addr
ADD R3, R3, -1 ; addr--
; STR ARRAY_ADDR, R3, 0 ; R3 = addr = ARRAY_ADDR
LD R0, ARRAY_LEN ; counter
LOOP BRz PASS
    LDR R2, R3, 1 ; curr = MEM[addr++]
    BRzp SKIP
        NEXT ADD R1, R1, R2; ANSWER = ANSWER + curr
    SKIP ADD R3, R3, 1 ; addr++
    ADD R0, R0, -1 ; counter--
    BR LOOP
PASS ST R1, ANSWER

HALT
ARRAY_ADDR .fill x4000
ARRAY_LEN  .fill 10

ANSWER     .blkw 1

.end

.orig x4000
  .fill 7
  .fill -18
  .fill 0
  .fill 5
  .fill -9
  .fill 25
  .fill 1
  .fill -2
  .fill 10
  .fill -6
.end
