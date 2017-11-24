    LDR     R5, =512
    ADD     R2, BP, #16
    LDR     R1, =0  ; popo
    ADD     R2, R2, R1, LSL #2
    STR     R5, [R2]        ; limit
    LDR     R5, =543
    ADD     R2, BP, #16
    LDR     R1, =1  ; popo
    ADD     R2, R2, R1, LSL #2
    STR     R5, [R2]        ; tmp
; Procedure Subtract
SubtractBody
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R5, [R2] ; i
    MOV     R2, BP          ; load current base pointer
    LDR     R2, [R2,#8]
    ADD     R2, R2,#16
    LDR     R6, [R2]        ; limit
    SUB     R5, R5, R6
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R5, [R2] ; i
    MOV     TOP, BP         ; reset top of stack
    LDR     BP, [TOP,#12]   ; and stack base pointers
    LDR     PC, [TOP]       ; return from Subtract
Subtract
    LDR     R0, =2          ; current lexic level
    LDR     R1, =0          ; number of local variables
    BL      enter           ; build new stack frame
    B       SubtractBody
; Procedure Add
AddBody
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R5, [R2] ; i
    LDR     R6, =0
    CMP     R5, R6
    MOVGT   R5, #1
    MOVLE   R5, #0
    MOVS    R5, R5          ; reset Z flag in CPSR
    BEQ     L1              ; jump on condition false
    MOV     R2, BP          ; load current base pointer
    LDR     R2, [R2,#8]
    ADD     R2, R2, #16
    LDR     R1, =3; opop
    ADD     R2, R2, R1, LSL #2
    LDR     R5, [R2]        ; sum
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R6, [R2] ; i
    ADD     R5, R5, R6
    MOV     R2, BP          ; load current base pointer
    LDR     R2, [R2,#8]
    ADD     R2, R2, #16
    LDR     R1, =3; opop
    ADD     R2, R2, R1, LSL #2
    STR     R5, [R2]        ; sum
    ADD     R0, PC, #4      ; store return address
    STR     R0, [TOP]       ; in new stack frame
    B       Subtract
    ADD     R0, PC, #4      ; store return address
    STR     R0, [TOP]       ; in new stack frame
    B       Add
    B       L2
L1
L2
    MOV     TOP, BP         ; reset top of stack
    LDR     BP, [TOP,#12]   ; and stack base pointers
    LDR     PC, [TOP]       ; return from Add
Add
    LDR     R0, =2          ; current lexic level
    LDR     R1, =0          ; number of local variables
    BL      enter           ; build new stack frame
    B       AddBody
; Procedure SumUp
SumUpBody
    LDR     R5, =12
 LDR R2, =101
 ADD R2, R4, R2, LSL #2
 STR R5, [R2] ; arr
    ADD     R2, BP, #16
    LDR     R1, =0  ; popo
    ADD     R2, R2, R1, LSL #2
    LDR     R5, [R2]        ; limit
 LDR R2, =124998
 ADD R2, R4, R2, LSL #2
 STR R5, [R2] ; arrTwo
 LDR R2, =13
 ADD R2, R4, R2, LSL #2
 LDR R5, [R2] ; arr
 LDR R2, =1000
 ADD R2, R4, R2, LSL #2
 STR R5, [R2] ; arrTwo
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R6, [R2] ; i
    ADD     R2, BP, #16
    LDR     R1, =2  ; popo
    ADD     R2, R2, R1, LSL #2
    STR     R6, [R2]        ; j
    ADD     R2, BP, #16
    LDR     R1, =0  ; popo
    ADD     R2, R2, R1, LSL #2
    LDR     R5, [R2]        ; limit
    ADD     R2, BP, #16
    LDR     R1, =3  ; popo
    ADD     R2, R2, R1, LSL #2
    STR     R5, [R2]        ; sum
    ADD     R0, PC, #4      ; store return address
    STR     R0, [TOP]       ; in new stack frame
    B       Add
    ADD     R0, PC, #4      ; string address
    BL      TastierPrintString
    B       L3
    DCB     "The sum of the values from 1 to ", 0
    ALIGN
L3
    ADD     R2, BP, #16
    LDR     R1, =2  ; popo
    ADD     R2, R2, R1, LSL #2
    LDR     R5, [R2]        ; j
    MOV     R0, R5
    BL      TastierPrintInt
    ADD     R0, PC, #4      ; string address
    BL      TastierPrintString
    B       L4
    DCB     " is ", 0
    ALIGN
L4
    ADD     R2, BP, #16
    LDR     R1, =3  ; popo
    ADD     R2, R2, R1, LSL #2
    LDR     R5, [R2]        ; sum
    MOV     R0, R5
    BL      TastierPrintIntLf
    MOV     TOP, BP         ; reset top of stack
    LDR     BP, [TOP,#12]   ; and stack base pointers
    LDR     PC, [TOP]       ; return from SumUp
SumUp
    LDR     R0, =1          ; current lexic level
    LDR     R1, =6          ; number of local variables
    BL      enter           ; build new stack frame
    B       SumUpBody
;Name: limit, Type: integer, Kind: constant, Level: local
;Name: tmp, Type: integer, Kind: constant, Level: local
;Name: j, Type: integer, Kind: var, Level: local
;Name: sum, Type: integer, Kind: var, Level: local
;Name: Subtract, Type: undef, Kind: proc, Level: local
;Name: Add, Type: undef, Kind: proc, Level: local
MainBody
    ADD     R0, PC, #4      ; string address
    BL      TastierPrintString
    B       L5
    DCB     "Enter value for i (or 0 to stop): ", 0
    ALIGN
L5
    BL      TastierReadInt
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R0, [R2] ; i
L6
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 LDR R5, [R2] ; i
    LDR     R6, =0
    CMP     R5, R6
    MOVGT   R5, #1
    MOVLE   R5, #0
    MOVS    R5, R5          ; reset Z flag in CPSR
    BEQ     L7              ; jump on condition false
    ADD     R0, PC, #4      ; store return address
    STR     R0, [TOP]       ; in new stack frame
    B       SumUp
    ADD     R0, PC, #4      ; string address
    BL      TastierPrintString
    B       L8
    DCB     "Enter value for i (or 0 to stop): ", 0
    ALIGN
L8
    BL      TastierReadInt
 LDR R2, =0
 ADD R2, R4, R2, LSL #2
 STR R0, [R2] ; i
    B       L6
L7
StopTest
    B       StopTest
Main
    LDR     R0, =1          ; current lexic level
    LDR     R1, =0          ; number of local variables
    BL      enter           ; build new stack frame
    B       MainBody
;Name: i, Type: integer, Kind: var, Level: global
;Name: arr, Type: integer, Kind: constant, Level: global
;Name: arrTwo, Type: integer, Kind: constant, Level: global
;Name: SumUp, Type: undef, Kind: proc, Level: global
;Name: main, Type: undef, Kind: proc, Level: global
