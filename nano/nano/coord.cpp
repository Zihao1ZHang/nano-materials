#include "coord.h"



coord::coord()
{
	x = 0;
	y = 0;
	z = 0;
}

coord::coord(int a, int b, int c)
{
	x = a;
	y = b;
	z = c;
}

double coord::getX()
{
	return x;
}

double coord::getY()
{
	return y;
}

double coord::getZ()
{
	return z;
}

void coord::setX(double a)
{
	x = a;
}

void coord::setY(double b)
{
	y = b;
}

void coord::setZ(double c)
{
	z = c;
}

double coord::distance(coord co, double l)
{
	double a = (x - co.getX()) * (x - co.getX()) + (y - co.getY()) * (y - co.getY()) + (z - co.getZ()) * (z - co.getZ());
	double a1 = (x - co.getX()) * (x - co.getX()) + (y - co.getY() + l) * (y - co.getY() + l) + (z - co.getZ()) * (z - co.getZ());
	if (a >= a1)
		return pow(a1, 0.5);
	else
		return pow(a, 0.5);

}

void coord::move(double a, double b, double c)
{
	x += a;
	y += b; 
	z += c;
}

coord::~coord()
{
}
