#include "atom.h"



atom::atom()
{
	atomNumber = -1;
	type = -1;
	boxN = "";
	c = coordinates();
}

atom::atom(int t, string b, coordinates C)
{
	type = t;
	boxN = b;
	c = C;
}

atom::atom(int n, int t, string b, coordinates C)
{
	atomNumber = n;
	type = t;
	boxN = b;
	c = C;
}

atom::~atom()
{
}

void atom::setClose(vector<atom> x)
{
	for (int i = 0; i < x.size(); i++)
	{
		close.push_back(x[i]);
	}
		
}

double atom::distance(atom a)
{
	double x = (this->getC().getX() - a.getC().getX()) * (this->getC().getX() - a.getC().getX()) + (this->getC().getY() - a.getC().getY()) * (this->getC().getY() - a.getC().getY()) + (this->getC().getZ() - a.getC().getZ())*(this->getC().getZ() - a.getC().getZ());
	double y = sqrt(x);
	return y;
}