#pragma once
#include <math.h>
class coord
{
private:
	double x;
	double y;
	double z;
public:
	coord();
	coord(int, int, int);
	~coord();

	//getter
	double getX();
	double getY();
	double getZ();

	//setter
	void setX(double a);
	void setY(double b);
	void setZ(double c);

	void move(double a, double b, double c);

	double distance(coord, double);

};

