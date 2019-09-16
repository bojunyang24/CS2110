;;====================================
;; CS 2110 - Fall 2018
;; Homework 7
;; buildheap.asm
;;====================================
;; Name:
;;====================================

; Little reminder from your pals: don't run this directly by pressing
; ``Run'' in complx, since (look below) there's nothing put at address
; x3000. Instead, load it and use ``Debug'' -> ``Simulate Subroutine
; Call'' and choose the ``strlen'' label.

.orig x3000

halt

heapify
    ADD R6, R6, -7  ;4 local vars R6,1
    STR R5, R6, 4   ;store old FP
    STR R7, R6, 5   ;store RA
    ADD R5, R6, 0   ;set current FP. locali = R5+3, locallargest = R5+2, locallength = R5+1, localinit = R5
    ADD R6, R6, -5  ;make space to store the registers R6.2
    STR R0, R6, 4   ;save R0
    STR R1, R6, 3   ;save R1
    STR R2, R6, 2   ;save R2
    STR R3, R6, 1   ;save R3
    STR R4, R6, 0   ;save R4

    LDR R0, R5, 7   ;R0 <- address of the first element in the array, init
    STR R0, R5, 0   ;localinit = address of first element of the array
    LDR R1, R5, 8   ;R1 <- length of the array, n
    STR R1, R5, 1   ;locallength = n
    LDR R2, R5, 9   ;R2 <- index of largest, largest
    STR R2, R5, 2   ;locallargest = i = largest
    STR R2, R5, 3   ;locali = i

    ADD R3, R2, R2  ;i*2
    ADD R3, R3, 1   ;R3 <- leftChild
    ADD R4, R2, R2  ;i*2
    ADD R4, R4, 2   ;R4 <-rightChild

    ;if leftChild < n && arr[leftChild] > arr[largest]
    NOT R1, R1      ;
    ADD R1, R1, 1   ;R1 <- (-n)
    ADD R1, R3, R1  ;R1 <- leftChild - n
    BRzp CASE2   ;if leftChild >= n then conditional fails
    ADD R2, R2, R0  ;init + largest. needs to be before arr[leftChild] b/c getting arr[leftChild] will overwrite init
    LDR R2, R2, 0   ;R2 <- arr[largest]
    NOT R2, R2
    ADD R2, R2, 1   ;R2 <- (-arr[largest])
    ADD R0, R0, R3  ;init + leftChild
    LDR R0, R0, 0   ;R0 <- array[leftChild]
    ADD R2, R0, R2  ;arr[leftChild] - arr[largest]
    BRnz CASE2   ;if arr[leftChild] <- arr[largest] then conditional fails
    ADD R2, R3, 0   ;largest = leftChild
    STR R2, R5, 2   ;store largest in respective locallargest so when loading in next case it is the correct largest

CASE2
    LDR R0, R5, 0   ;reload localinit, address of the first element
    LDR R1, R5, 1   ;reload locallength, n
    LDR R2, R5, 2   ;reload locallargest, largest
    ;if rightChild < n && arr[rightChild] > arr[largest]
    NOT R1, R1      ;
    ADD R1, R1, 1   ;R1 <- (-n)
    ADD R1, R4, R1  ;R1 <- rightChild - n
    BRzp CASE3
    ADD R2, R2, R0  ;init + largest. needs to be before arr[rightChild] b/c getting arr[rightChild] will overwrite init
    LDR R2, R2, 0   ;R2 <- arr[largest]
    NOT R2, R2
    ADD R2, R2, 1   ;R2 <- (-arr[largest])
    ADD R0, R0, R4  ;init + rightChild
    LDR R0, R0, 0   ;R0 <- array[rightChild]
    ADD R2, R0, R2  ;arr[rightChild] - arr[largest]
    BRnz CASE3
    ADD R2, R4, 0   ;largest = rightChild
    STR R2, R5, 2   ;store largest in respective locallargest so when loading in next case it is the correct largest

CASE3
    LDR R0, R5, 0   ;reload localinit, address of the first element
    LDR R1, R5, 1   ;reload locallength, n
    LDR R2, R5, 2   ;reload locallargest, largest
    ;at this point, we don't need leftChild or rightChild so R3 and R4 can be used
    LDR R3, R5, 3   ;load locali (original i) into R3
    NOT R3, R3      ;
    ADD R3, R3, 1   ; R3 = -i
    ADD R3, R2, R3  ;largest - i
    BRz STACK_BREAKDOWN
    ;swap(arr[i]), arr[largest]). R2 <- largest ; R3 <- i
    LDR R3, R5, 3   ;reload locali back into R3
    ADD R1, R0, R2  ;R1 = init + largest = addr for largest
    ADD R4, R0, R3  ;R4 = init + i = addr for i
    LDR R2, R1, 0   ;load arr[largest] into R2
    LDR R3, R4, 0   ;load arr[i] into R3
    STR R2, R4, 0   ;store arr[largest] at i
    STR R3, R1 ,0   ;store arr[i] at largest
    ;heapify(arr,n,largest)
    ADD R6, R6, -1  ;increment R6 to point above R4
    LDR R0, R5, 0   ;reload localinit, address of the first element
    STR R0, R6, -2  ;store array addr at top of stack, third param
    LDR R1, R5, 1   ;reload locallength, n
    STR R1, R6, -1  ;store array length below array addr, second param
    LDR R2, R5, 2   ;reload locallargest, largest
    STR R2, R6, 0   ;store largest as the first param
    ADD R6, R6, -2   ;point to correct place for next recursive call stack
    JSR heapify

STACK_BREAKDOWN
    LDR R4, R5, -5  ;restore R4
    LDR R3, R5, -4  ;restore R3
    LDR R2, R5, -3  ;restore R2
    LDR R1, R5, -2  ;restore R1
    LDR R0, R5, -1  ;restore R0
    ADD R6, R5, 0   ;bring R6 back down to R5 because we just popped the stored registers
    LDR R5, R6, 4   ;restore old FP
    LDR R7, R6, 5   ;restore RA
    ADD R6, R6, 6   ;set R6 to point to return value
    RET


buildheap
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

    ADD R6, R6, -1  ;point above R4

    LDR R0, R5, 4   ;R0 <- array addr, array
    LDR R1, R5, 5   ;R1 <- array.length, n

    STR R0, R6, -2  ;array
    STR R1, R6, -1  ;n

    ADD R4, R6, -2   ;make copy of R6
    ADD R2, R1, 0   ;initialize i = n

HEAPLOOP
    BRn HEAP_BREAKDOWN
    ADD R6, R4, 0   ;make sure R6 points to top of stack
    STR R2, R6, 2   ;set the i in heapify(arr, n, i)
    JSR heapify
    ADD R2, R2, -1  ;decrement i
    BR HEAPLOOP

HEAP_BREAKDOWN
    LDR R4, R5, -5  ;restore R4
    LDR R3, R5, -4  ;restore R3
    LDR R2, R5, -3  ;restore R2
    LDR R1, R5, -2  ;restore R1
    LDR R0, R5, -1  ;restore R0
    ADD R6, R5, 0   ;R6.6 bring R6 back down to R5 because we just popped the stored registers
    LDR R5, R6, 1   ;store old FP back into R5
    LDR R7, R6, 2   ;store old return address back into R7
    ADD R6, R6, 3   ;have R6 point to return value
    RET

; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
.end

; You should not have to LD from any label, take the
; address off the stack instead :)
