#include "automation.h"
#include <iostream>
automation A1(6);
int main() {
	A1[0] = "no";
	A1[1] = "1";
	A1[2] = "11";
	A1[3] = "0";
	A1[4] = "01";
	A1[5] = "011";
	A1.setinit(0);
	A1.addac(3);
	A1.addedge(0, 1, "1");
	A1.addedge(0, 3, "0");
	A1.addedge(1, 2, "1");
	A1.addedge(1, 4, "0");
	A1.addedge(2, 0, "1");
	A1.addedge(2, 5, "0");
	A1.addedge(3, 0, "0");
	A1.addedge(3, 4, "1");
	A1.addedge(4, 1, "0");
	A1.addedge(4, 5, "1");
	A1.addedge(5, 2, "0");
	A1.addedge(5, 3, "1");
	A1.dot("1.dot");
	cout << A1.RE() << endl;
	system("pause");
	return 0;
}