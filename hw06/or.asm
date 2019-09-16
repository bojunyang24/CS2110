;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Bojun Yang
;;====================================

.orig x3000

  ;; YOUR CODE HERE :D
LD R0, A ; R0 = A
LD R1, B; R1 = B
NOT R0, R0 ; R0 = A'
NOT R1, R1 ; R1 = B'
AND R2, R0, R1 ; R2 = A' & B'
NOT R2, R2 ; R2 = (A' & B')'
ST R2, ANSWER
HALT

A      .fill x1010
B      .fill x0404

ANSWER .blkw 1

.end
