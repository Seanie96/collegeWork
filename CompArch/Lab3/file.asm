// int g = 4; global variable g: r9

add r0, # 4, r9

//int min(int a, int b, int c) {  parameters a: r26, b: 27, c: 28
//int v = a;
//if (b < v)
//v = b;
//if (c < v)
//v = c;
//return v;
//}
min: add r26, r0, r1      ; use r1 for local(function returned in r1)
sub r27, r1, r0 {C}       ; b < v
jge min0                  ;
xor r0, r0, r0            ; nop in delay slot
add r27, r0, r1           ; v = b
min0: cmp r28, r1, r0 {C} ; c < v
jge min1                  ;
xor r0, r0, r0            ; nop in delay slot
add r28, r0, r1           ; v = b
min1: ret r25, 0          ; return 
xor r0, r0, r0            ; delay slot
