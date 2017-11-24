; Procedure test
testBody
    LDR     R5, =5
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R5, [R2] ; n
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R5, [R2] ; n
    LDR     R7, =4
    CMP     R7, R5
    MOVEQ   R7, #1
    MOVNE   R7, #0
    MOVS    R7, R7          ; reset Z flag in CPSR
    BEQ     L2              ; jump on condition false
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R8, [R2] ; n
    LDR     R9, =0
    SUB     R8, R8, R9
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R8, [R2] ; n
    B       L2
L2
    LDR     R6, =6
    CMP     R6, R5
    MOVEQ   R6, #1
    MOVNE   R6, #0
    MOVS    R6, R6          ; reset Z flag in CPSR
    BEQ     L3              ; jump on condition false
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R7, [R2] ; n
    LDR     R8, =2
    SUB     R7, R7, R8
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R7, [R2] ; n
    B       L3
L3
    LDR     R6, =7
    CMP     R6, R5
    MOVEQ   R6, #1
    MOVNE   R6, #0
    MOVS    R6, R6          ; reset Z flag in CPSR
    BEQ     L4              ; jump on condition false
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R7, [R2] ; n
    LDR     R8, =3
    SUB     R7, R7, R8
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R7, [R2] ; n
    B       L4
L4
    LDR     R6, =8
    CMP     R6, R5
    MOVEQ   R6, #1
    MOVNE   R6, #0
    MOVS    R6, R6          ; reset Z flag in CPSR
    BEQ     L5              ; jump on condition false
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R7, [R2] ; n
    LDR     R8, =4
    SUB     R7, R7, R8
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R7, [R2] ; n
    B       L5
L5
    LDR     R6, =9
    CMP     R6, R5
    MOVEQ   R6, #1
    MOVNE   R6, #0
    MOVS    R6, R6          ; reset Z flag in CPSR
    BEQ     L6              ; jump on condition false
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R7, [R2] ; n
    LDR     R8, =5
    SUB     R7, R7, R8
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R7, [R2] ; n
    B       L6
L6
    LDR     R6, =10
    CMP     R6, R5
    MOVEQ   R6, #1
    MOVNE   R6, #0
    MOVS    R6, R6          ; reset Z flag in CPSR
    BEQ     L7              ; jump on condition false
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R7, [R2] ; n
    LDR     R8, =6
    SUB     R7, R7, R8
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R7, [R2] ; n
    B       L7
L7
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R5, [R2] ; n
    LDR     R6, =1
    SUB     R5, R5, R6
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R5, [R2] ; n
L1
    MOV     TOP, BP         ; reset top of stack
    LDR     BP, [TOP,#12]   ; and stack base pointers
    LDR     PC, [TOP]       ; return from test
test
    LDR     R0, =1          ; current lexic level
    LDR     R1, =0          ; number of local variables
    BL      enter           ; build new stack frame
    B       testBody
