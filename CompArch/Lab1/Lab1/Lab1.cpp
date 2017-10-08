// t1Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"         // pre-compiled headers
#include <iostream>         // cout
#include "conio.h"          // _getch


using namespace std;        // cout

							//
							// check
							//
void check(char *s, int v, int expected) {
	cout << s << " = " << v;
	if (v == expected) {
		cout << " OK";
	}
	else {
		cout << " ERROR: should be " << expected;
	}
	cout << endl;
}

int main()
{
	check("min(1, 2, 3)", min_IA32(1, 2, 3), 1);
	check("min(3, 1, 2)", min_IA32(3, 1, 2), 1);
	check("min(2, 3, 1)", min_IA32(2, 3, 1), 1);
	check("min(-1, -2, -3)", min_IA32(-1, -2, -3), -3);
	check("min(-3, -1, -2)", min_IA32(-3, -1, -2), -3);
	check("min(-2, -3, -1)", min_IA32(-2, -3, -1), -3);
	check("min(-1, 2, 3)", min_IA32(-1, 2, 3), -1);
	check("min(3, -1, 2)", min_IA32(3, -1, 2), -1);
	check("min(2, 3, -1)", min_IA32(2, 3, -1), -1);

	check("p(1, 2, 3)", p_IA32(1, 2, 3, 6), 1);
	check("p(5, 6, 7)", p_IA32(5, 6, 7, 8), 4);

	check("gcd(14, 21)", gcd_IA32(14, 21), 7);
	check("gcd(1406700, 164115)", gcd_IA32(1406700, 164115), 23445);

	cout << endl;
	_getch();

	return 0;
}