#include <iostream>
#include <string>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "atom.h"
#include "coordinates.h"
using namespace std;

coordinates matrixMult(double x[], double y[], double z[], coordinates c);

int main()
{
	string filename1 = "2H_UNIT.vasp";
	string filename2 = "3C.vasp";

	ifstream file(filename1);
	map<int, int> elements;
	vector<atom> atoms; //storing atoms in one box
	string title;//title of the file
	char fChar;
	vector<bool> free;
	stringstream ss;
	string elem;
	string line;
	string tag;
	double mult;
	int count;
	int total = 0;
	double lattice1[3] = { 0,0,0 };
	double lattice2[3] = { 0,0,0 };
	double lattice3[3] = { 0,0,0 };
	const unsigned int NUM_ELEMENTS = 118;
	const string SYM[NUM_ELEMENTS] = { "H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","","","","","","","","","" };

	vector<atom>::iterator index;
	vector<atom> closeAtoms;
	
	//reading files
	if (file.is_open())
	{
		getline(file, title);
		file >> mult;
		for (int i = 0; i < 3; i++)
		{
			double temp;
			file >> temp;
			lattice1[i] += temp;	
		}
		for (int i = 0; i < 3; i++)
		{
			double temp;
			file >> temp;
			lattice2[i] += temp;
		}
		for (int i = 0; i < 3; i++)
		{
			double temp;
			file >> temp;
			lattice3[i] += temp;
		}

		do {
			getline(file, line);
		} while (line.length() <= 1);
		ss << line;
		while (ss >> elem)
		{
			for (int i = 0; i < NUM_ELEMENTS; i++)
			{
				if (elem == SYM[i])
				{
					elements[i] = 0;
				}
			}
		}

		for (map<int, int>::iterator eIt = elements.begin(); eIt != elements.end(); ++eIt)
		{
			file >> count;
			eIt->second = count;
			total += count;
		}

		file >> tag;

		for (map<int,int>::iterator elt = elements.begin(); elt != elements.end(); ++elt)	
		{
			for (int i = 0; i<elt->second; ++i)	
			{
				coordinates coord;
				for (int c = 0; c<3; ++c)			
					file >> coord.getCoord()[c];
				for (int c = 0; c<3; ++c)			
				{
					file >> fChar;
					free.push_back((fChar == 'T'));
				}
				atom temp(elt->first, "000", coord);
				atoms.push_back(temp);
			}
		}
		file.close();
	}
	else
	{
		cout << "Cannot open the file";
	}

	//setting atom number
	for (int i = 0; i < atoms.size(); i++)
		atoms[i].setautoN(i);
	
	//setting coordinates
	for (int i = 0; i < atoms.size(); i++)
	{
		atoms[i].setCoord(matrixMult(lattice1, lattice2, lattice3, atoms[i].getC()));
	}
	

	int size = atoms.size();
	for (int a = 0; a < size; a++)
	{
		closeAtoms.clear();
		for (int b = 0; b < size; b++)
		{
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					for (int k = -1; k < 2; k++)
					{
						double min = 10E9;
						coordinates temp = atoms[b].getC();
						temp.setX(temp.getX() + i * lattice1[0]);
						temp.setY(temp.getY() + j * lattice2[1]);
						temp.setZ(temp.getZ() + k * lattice3[2]);
						atom tempAtom(atoms[b].getAtomN(), atoms[b].getType(), to_string(i) + to_string(j) + to_string(k), temp);
							
						tempAtom.setDis(atoms[a].distance(tempAtom));
						if (closeAtoms.size() < 4)
						{
							closeAtoms.push_back(tempAtom);
						}
						else
						{
							closeAtoms.push_back(tempAtom);
							for (int i = 0; i < 5; i++)
							{
								if (min > closeAtoms[i].getDis())
									min = closeAtoms[i].getDis();
							}
							for (index = closeAtoms.begin(); index != closeAtoms.end(); index++)
							{
								if (min == index->getDis())
								{
									closeAtoms.erase(index);
									break;
								}
							}
						}
						
					}
				}
			}
		}
		atoms[a].setClose(closeAtoms);
	}
	cout << "";
	
	ofstream close_atoms;
	close_atoms.open("close atoms.txt");
	if (close_atoms.is_open())
	{

		for (int i = 0; i < size; i++)
		{
			close_atoms 
				<< "number: " << atoms[i].getAtomN() << "      type: " << atoms[i].getType() << "     box: " << atoms[i].getBoxN() << "     coordinate: {"
				<< atoms[i].getC().getX() << ", " << atoms[i].getC().getY() << ", " << atoms[i].getC().getZ() << "}" << endl;
			close_atoms << "The closest atoms: " << endl;
			for (int j = 0; j < atoms[i].getClose().size(); j++)
			{
				close_atoms << "number: " << atoms[i].getClose()[j].getAtomN()
					<< "      type: " << atoms[i].getClose()[j].getType()
					<< "     box: " << atoms[i].getClose()[j].getBoxN()
					<< "     coordinate: {"
					<< atoms[i].getClose()[j].getC().getX() << ", "
					<< atoms[i].getClose()[j].getC().getY() << ", "
					<< atoms[i].getClose()[j].getC().getZ() << "}     "
					<< "distance: " << atoms[i].getClose()[j].getDis()
					<< endl;
			}
			cout << endl;
		}
		close_atoms.close();
	}
	cout << "";
	return 0;
}

coordinates matrixMult(double x[], double y[], double z[], coordinates c)
{
	coordinates temp;
	temp.setX(c.getX() * x[0] + c.getY() * y[0] + c.getZ() * z[0]);
	temp.setY(c.getX() * x[1] + c.getY() * y[1] + c.getZ() * z[1]);
	temp.setZ(c.getX() * x[2] + c.getY() * y[2] + c.getZ() * z[2]);
	return temp;
}