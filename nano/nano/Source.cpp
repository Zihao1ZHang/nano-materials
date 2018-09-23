#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <math.h>
#include <stack>

#include "coord.h"
#include "atom.h"
using namespace std;
double distance(coord c0, coord c1);

int main()
{
	ifstream f("grapheneBulk.vasp");
	coord origin;
	coord temp;
	vector <coord> coords;
	string line;
	map<int, atom> atoms;
	stack<int> closeAtoms;

	double a;
	double length;
	int keys = 0;

	double boxX;
	double boxY;
	double boxZ;

	if (f.is_open())
	{
		int counter = 0;
		int count = 0;
		while (getline(f, line))
		{
			stringstream ss(line);
			string word;

			while (getline(ss, word, ' '))
			{
				if (word != "")
				{
					if (counter == 1)
						a = stod(word);
					else if (counter == 2)
					{
						boxX = stod(word);
					}
					else if (counter == 6)
					{
						boxY = stod(word);
					}
					else if (counter == 10)
					{
						boxZ = stod(word);
					}
					else if (counter == 12)
						length = stod(word);
					else if (counter >= 14)
					{
						if (count == 0)
						{
							temp.setX(stod(word));
							count++;
						}
						else if (count == 1)
						{
							temp.setY(stod(word));
							count++;
						}
						else if(count == 2)
						{
							temp.setZ(stod(word));
							count = 0;
							atom tempAtom(keys, 1, "000", temp);
							atoms[keys++] = tempAtom;
						}
					}
					counter++;
				}
			}
			
		}
		
		f.close();
	}
	
	int numOfAtom = keys; //number of atoms in one box
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			for (int k = -1; k < 2; k++)
			{
				for (int b = 0; b < numOfAtom; b++)
				{
					if (i != 0 || j != 0 || k != 0)
					{
						atom tempAtom(keys, 1, to_string(i) + to_string(j) + to_string(k), atoms.find(b)->second.getC());
						tempAtom.setPosition(i * boxX, j * boxY, k * boxZ);
						atoms[keys++] = tempAtom;
					}
				}
			}
		}
	}

	bool flag = true;
	double dis, min = 1E9;
	for (int i = 0; i < numOfAtom; i++)
	{
		for (map<int, atom>::iterator it = atoms.begin(); it != atoms.end(); it++)
		{
			dis = distance(atoms.find(i)->second.getC(), it->second.getC());
			
			if (dis < min && dis != 0)
			{
				min = dis;
				while (!closeAtoms.empty())
					closeAtoms.pop();
				closeAtoms.push(it->second.getNumber());
			}
			else if (dis == min && dis != 0)
			{
				closeAtoms.push(it->second.getNumber());
			}
		}
		flag = true;
		min = 1E9;
	
		while (!closeAtoms.empty())
		{
			atoms.find(i)->second.setCloseAtoms(closeAtoms.top());
			closeAtoms.pop();
		}
	}

	ofstream close_atoms;
	close_atoms.open("close atoms.txt");
	if (close_atoms.is_open()) 
	{

		for (int i = 0; i < numOfAtom; i++)
		{
			close_atoms << endl
				<< "number: " << atoms.find(i)->second.getNumber() << "      type: " << atoms.find(i)->second.getType() << "     box: " << atoms.find(i)->second.getBox() << "     coordinate: {"
				<< atoms.find(i)->second.getC().getX() << ", " << atoms.find(i)->second.getC().getY() << ", " << atoms.find(i)->second.getC().getZ() << "}" << endl;
			close_atoms << "The closest atoms: " << endl;
			for (int j = 0; j < atoms.find(i)->second.getCloseAtoms().size(); j++)
			{
				close_atoms << "number: " << atoms.find(atoms.find(i)->second.getCloseAtoms()[j])->second.getNumber()
					<< "      type: " << atoms.find(atoms.find(i)->second.getCloseAtoms()[j])->second.getType()
					<< "     box: " << atoms.find(atoms.find(i)->second.getCloseAtoms()[j])->second.getBox()
					<< "     coordinate: {"
					<< atoms.find(atoms.find(i)->second.getCloseAtoms()[j])->second.getC().getX() << ", "
					<< atoms.find(atoms.find(i)->second.getCloseAtoms()[j])->second.getC().getY() << ", "
					<< atoms.find(atoms.find(i)->second.getCloseAtoms()[j])->second.getC().getZ() << "}     "
					<< "distance: " <<distance(atoms.find(i)->second.getC(), atoms.find(atoms.find(i)->second.getCloseAtoms()[j])->second.getC())
					<< endl;
			}
			
		}
		close_atoms.close();
	}

	

	return 0;
}

double distance(coord c0, coord c1)
{
	return sqrt(pow((c1.getX() - c0.getX()), 2) + pow((c1.getY() - c0.getY()), 2) + pow((c1.getZ() - c0.getZ()), 2));
}

