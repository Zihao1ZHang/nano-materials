#pragma once
#include"coord.h"
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;
class atom
{
private:
	int number;
	int type;
	string box;
	coord c;
	vector<int> closeAtoms;
public:
	atom();
	atom(int, int, string, coord);
	~atom();

	int getType() { return type; };
	string getBox() { return box; };
	coord getC() { return c; };
	vector<int> getCloseAtoms() { return closeAtoms; };
	int getNumber() { return number; };


	void setType(int TYPE) { type = TYPE; };
	void setNumber(int n) { number = n; };
	void setBox(int Box) { box = Box; };
	void setC(coord C) { c = C; };
	void setCloseAtoms(int a) { closeAtoms.push_back(a); };
	void setPosition(double a, double b, double c);

	void print();
	double distances(coord c0);

};

