numeral(0).
numeral(s(X)) :- numeral(X).
numeral(X+Y) :- numeral(X), numeral(Y).

add(0,X,X).
add(s(X),Y,s(Z)) :- add(X,Y,Z).

add2(0,0,0).
add2(0,s(Y),s(Z)) :- add2(0,Y,Z).
add2(0, X+T, A) :- add(X,T,A),add2(0,A,A).
add2(s(X),Y,s(R)) :- add2(X,Y,R).
add2(X+Y,Z,R) :- add(X,Y,A),add2(A,Z,R).