numeral(0).
numeral(s(X)) :- numeral(X).
numeral(X+Y) :- numeral(X), numeral(Y).

addTwo(0, 0, 0).
addTwo(s(X), Y, s(Z)) :- addTwo(X,Y,Z), numeral(X), numeral(Y).
addTwo(X, s(Y), s(Z)) :- addTwo(X,Y,Z), numeral(Y), numeral(X).
addTwo(s(0)+X, Y, s(Z)) :- addTwo(X,Y,Z), numeral(X), numeral(Y).
addTwo(X, s(0)+Y, s(Z)) :- addTwo(X,Y,Z), numeral(Y), numeral(X).
addTwo(X, s(0+Y), s(Z)) :- addTwo(X,Y,Z), numeral(Y), numeral(X).

add(0,X,X).
add(s(X),Y,s(Z)) :- add(X,Y,Z).
