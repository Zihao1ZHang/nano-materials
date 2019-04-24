#include "coordinates.h"
coordinates::coordinates()
{
	coord[0] = 0;
	coord[1] = 0;
	coord[2] = 0;
}
coordinates::coordinates(double x, double y, double z)
{
	coord[0] = x;
	coord[1] = y;
	coord[2] = z;
}
coordinates::~coordinates()
{
}


void coordinates::setX(double x)
{
	coord[0] = x;
}
void coordinates::setY(double y)
{
	coord[1] = y;
}
void coordinates::setZ(double z)
{
	coord[2] = z;
}


coordinates coordinates::operator + (coordinates &obj)
{
	coordinates c;
	this->setX(coord[0] + obj.getX());
	this->setY(coord[1] + obj.getY());
	this->setZ(coord[2] + obj.getZ());
	return c;
}
