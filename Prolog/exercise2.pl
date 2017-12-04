% Exercise 1

s1 --> [2].
s1 --> u, s1, v.

u --> t, [0], t.
t --> [].
t --> [1], p.
p --> [].
p --> t.

v --> x, [1], x.
x --> [].
x --> [0], y.
y --> [].
y --> x.

/*
deleteall([],A,[]).
deleteall([H|T],A,Result) :- H=A, deleteall(T,A,Result).
deleteall([H|T],A,[H|Result]) :- deleteall(T,A,Result).
*/

% Exercise 2

s2 --> c(C1), n(N1), p(P1), c([C1],C2), n([N1],N2), p([P1], P2), c([C1, C2], _), n([N1, N2], _), p([P1, P2], _).

c(blue) --> [blue].
c(red) --> [red].
c(green) --> [green].

n(english) --> [english].
n(japanese) --> [japanese].
n(spanish) --> [spanish].

p(jaguar) --> [jaguar].
p(zebra) --> [zebra].
p(snail) --> [snail].

c(L, Z) --> [Z], {subtract([red, blue, green], L, T), getElm(T, Z)}.
n(L, Z) --> [Z], {subtract([english, japanese, spanish], L, T), getElm(T, Z)}.
p(L, Z) --> [Z], {subtract([jaguar, zebra, snail], L, T), getElm(T, Z)}.

getElm([X1, _], X1).
getElm([_, X2], X2).
getElm([X1], X1).
getElm([], _).

% Exercise 3

s3(Sum) --> getPermutations(L), {mkList(Sum, L)}.

getPermutations(L) --> getPermutation(L), {}
getPermutation(L, ) --> , {}
getPermutation(L, ) --> []

mkList(1, [1]).
mkList(Sum, [Sum|T]) :-
  X is Sum - 1,
  X >= 1,
  mkList(X, T).

prefix(P,L) :-
  append(P,_,L).

suffix(S, L) :-
  append(_,S,L).

sublist(Sub, List) :-
  suffix(Suffix, List),
  prefix(Sub, Suffix).

getPerm(L, Sum, Res) :-
  (sublist(Sub, L),
  sum(Sub, R),
  R = Sum,
  Res is Sub);
  getPerm(L, Sum, Res).

sum(L,R) :-
    sum(L,0,R).
sum([],F,F).
sum([H|T],F,R) :-
    F2 is F+H,
    sum(T,F2,R).

/*
nAb(0) --> [english]
nAb(1) --> [japanese].
nAb(2) --> [spanish].

pAb(0) --> [jaguar].
pAb(1) --> [zebra].
pAb(2) --> [snail].

c(X) :- cAb(X, R, []).
n(X) :- nAb(X, R, []).
p(X) :- pAb(X, R, []).

checkForEquality(C1, C2, C3, N1, N2, N3, P1, P2, P3) :-
  C1 =\= C2,
  C2 =\= C3,
  N1 =\= N2,
  N2 =\= N3,
  P1 =\= P2,
  P2 =\= P3.
*/


/*
mkList(1, [1]).
mkList(Sum, [Sum|T]) :-
  X is Sum - 1,
  X >= 1,
  mkList(X, T).

prefix(P,L) :-
  append(P,_,L).

suffix(S, L) :-
  append(_,S,L).

sublist(Sub, List) :-
  suffix(Suffix, List),
  prefix(Sub, Suffix).

getPerm(L, Sum, Res) :-
  (sublist(Sub, L),
  sum(Sub, R),
  R = Sum,
  Res is Sub);
  getPerm(L, Sum, Res).

sum(L,R) :-
    sum(L,0,R).
sum([],F,F).
sum([H|T],F,R) :-
    F2 is F+H,
    sum(T,F2,R).
*/

/*
cAb(0) --> [red].
cAb(1) --> [blue].
cAb(2) --> [green].

nAb(0) --> [english].
nAb(1) --> [japanese].
nAb(2) --> [spanish].

pAb(0) --> [jaguar].
pAb(1) --> [zebra].
pAb(2) --> [snail].

c(X) :- cAb(X).
n(X) :- nAb(X).
p(X) :- pAb(X).

generateString(0) :-
  (c(C1),
  n(N1),
  p(P1),
  c(C2),
  n(N2),
  p(P2),
  c(C3),
  n(N3),
  p(P3),
  C1 =\= C2,
  C2 =\= C3,
  N1 =\= N2,
  N2 =\= N3,
  P1 =\= P2,
  P2 =\= P3);
  generateString(0).

s2 --> [], {generateString(X)}.
*/
