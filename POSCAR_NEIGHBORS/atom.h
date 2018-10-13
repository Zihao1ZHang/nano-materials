#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <stack>
using namespace std;

#include "coordinates.h"
class atom
{
private:
	int atomNumber;
	int type;
	string boxN;
	coordinates c;
	vector<atom> close;
	double dis;
public:
	//constructors------------------------------
	atom();
	atom(int, string, coordinates);
	atom(int, int, string, coordinates);
	~atom();

	//mutators and accessors--------------------
	void setautoN(int n) { atomNumber = n; };
	void setType(int t) { type = t; };
	void setBoxN(string b) { boxN = b; };
	void setCoord(coordinates C) { c = C; };
	void setClose(vector<atom> x);
	void setDis(double d) { dis = d; };

	int getAtomN() { return atomNumber; };
	int getType() { return type; };
	string getBoxN() { return boxN; };
	coordinates getC() { return c; };
	vector<atom> getClose() { return close; };
	double getDis() { return dis; };

	//functions---------------------------------
	double distance(atom a);
};

