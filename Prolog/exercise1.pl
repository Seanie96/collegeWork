numeral(0).
numeral(s(X)) :- numeral(X).
numeral(p(X)) :- numeral(X).
numeral(-X) :- numeral(X).
numeral(X+Y) :- numeral(X), numeral(Y).
numeral(X-Y) :- numeral(X), numeral(Y).

add(0,X,X).
add(s(X),Y,s(Z)) :- add(X,Y,Z).
add(p(X),Y,p(Z)) :- add(X,Y,Z).

simpNum(0,0).
simpNum(s(p(X)), R) :- simpNum(X, R).
simpNum(p(s(X)), R) :- simpNum(X, R).
simpNum(s(X), s(R)) :- simpNum(X, R).
simpNum(p(X), p(R)) :- simpNum(X, R).

simpExpr(0,0).
simpExpr(s(X),Z) :- simpExpr(X,Y), simpNum(s(Y),Z), !.
simpExpr(p(X),Z) :- simpExpr(X,Y), simpNum(p(Y),Z), !.
simpExpr(X+Y,Z) :- simpExpr(X,A), simpExpr(Y,B), add(A,B,C), simpExpr(C,Z), !.
simpExpr(-X,Z) :- simpExpr(X,Y), minus(Y,Z), !.
simpExpr(X-Y,Z) :- simpExpr(X,A), simpExpr(Y,B), subtract(A,B,C), simpExpr(C,Z), !.
add2(X,Y,Z) :- simpExpr(X,A), simpExpr(Y,B), add(A,B,C), simpExpr(C,Z), !.

minusNum(0,0).
minusNum(p(X), Z) :- minusNum(X, R), simpNum(s(R), Z), !.
minusNum(s(X), Z) :- minusNum(X, R), simpNum(p(R), Z), !.
minus(X,Z) :- minusNum(X,R), simpNum(R,Z).

subtract(X,Y,P) :- simpExpr(X,A), simpExpr(Y,B), minus(B,R), add(A,R,C), simpExpr(C,P).
