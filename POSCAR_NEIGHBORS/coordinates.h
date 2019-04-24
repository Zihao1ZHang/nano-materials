#pragma once
class coordinates
{
private:
	double coord[3];
public:
	//constructor-------------------------------
	coordinates();
	coordinates(double x, double y, double z);
	~coordinates();

	//mutators and accessors -------------------
	void setX(double x);
	void setY(double y);
	void setZ(double z);
	double getX() { return coord[0]; };
	double getY() { return coord[1]; };
	double getZ() { return coord[2]; };
	double* getCoord() { return coord; };

	//overloaded operators----------------------
	coordinates operator + (coordinates &obj);
	//coordinates operator = (coordinates const &obj);
};

