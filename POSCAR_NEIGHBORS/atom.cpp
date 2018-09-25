#include "atom.h"



atom::atom()
{
	number = 0;
	type = 0;
	box = "000";
	coord(0, 0, 0);
}

atom::atom(int n, int TYPE, string BOX, coord C) 
{
	number = n;
	type = TYPE;
	box = BOX;
	c = C;
}

void atom::setPosition(double a, double b, double C)
{
	c.move(a, b, C);
}

double atom::distances(coord c0)
{
	return sqrt(pow((c.getX() - c0.getX()), 2) + pow((c.getY() - c0.getY()), 2) + pow((c.getZ() - c0.getZ()), 2));
}




atom::~atom()
{
}


