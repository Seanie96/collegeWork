numeral(0).
numeral(s(X)) :- numeral(X).

add(0,X,X).
add(s(X),Y,s(Z)) :- add(X,Y,Z).

numeral(X+Y) :- numeral(X), numeral(Y).

 :- add2(X,Y,Z).
