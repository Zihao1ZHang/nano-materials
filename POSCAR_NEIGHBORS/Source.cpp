#include <iostream>
#include <string>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <stack>

#include "atom.h"
#include "coordinates.h"
using namespace std;

coordinates matrixMult(double x[], double y[], double z[], coordinates c);
double EstimateDis(int x, int y);
bool compare(atom & l, atom & r);
bool visited(atom n, vector<atom> v);




int main()
{
	string filename1 = "2H_UNIT.vasp";
	string filename2 = "3C.vasp";
	string filename3 = "sich.vasp";
	string filename4 = "grapheneBulk.vasp";
	string filename5 = "test6.vasp";

	string filename24 = "test2.vasp";

	ifstream file("POSCARcsi216.vasp");
	map<int, int> elements;//number of each type of elements
	vector<atom> atoms; //storing atoms in one box
	string title;//title of the file

	stringstream ss;
	string elem;
	string line;
	string tag;

	double mult;//multiplication coefficient 
	int count;
	int total = 0;//number of atoms in the file
	vector<int> eles; //types of elements in this file


	double matrixSize;

	//sizes of peridoical box
	double lattice1[3] = { 0,0,0 };
	double lattice2[3] = { 0,0,0 };
	double lattice3[3] = { 0,0,0 };

	const unsigned int NUM_ELEMENTS = 118;
	const string SYM[NUM_ELEMENTS] = { "H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","","","","","","","","","" };
	const double COV_RAD[NUM_ELEMENTS]={ 0.32,0.93,1.23,0.9,0.82,0.77,0.75,0.73,0.72,0.71,1.54,1.36,1.18,1.11,1.06,1.02,0.99,0.98,2.03,1.74,1.44,1.32,1.22,1.18,1.17,1.17,1.16,1.15,1.17,1.25,1.26,1.22,1.2,1.16,1.14,1.12,2.16,1.91,1.62,1.45,1.34,1.3,1.27,1.25,1.25,1.28,1.34,1.48,1.44,1.41,1.41,1.36,1.33,1.31,2.35,1.98,1.69,1.65,1.65,1.64,1.63,1.62,1.85,1.61,1.59,1.59,1.58,1.57,1.56,1.74,1.56,1.44,1.34,1.3,1.28,1.26,1.27,1.3,1.34,1.49,1.48,1.47,1.46,1.46,1.45,0,0,0,0,1.65,0,1.42,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	vector<atom>::iterator index;
	vector<atom> closeAtoms;

	//reading files
	if (file.is_open())
	{
		getline(file, title);	//reading file title
		file >> mult;

		//reading the size of periodical box
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

		// reading types of elements
		while (ss >> elem)
		{
			for (int i = 0; i < NUM_ELEMENTS; i++)
			{
				if (elem == SYM[i])
				{
					elements[i] = 0;
					eles.push_back(i);
				}
			}
		}

		//reading the number of each type of 
		for(int i = 0; i < eles.size(); i++)
		{
			file >> count;
			elements[eles[i]] = count;
			total += count;
		}

		//reading the tag, does not matter to the result
		matrixSize = eles.size();
		file >> tag;

		//reading the coordinates of each atom
		for (int i = 0; i < eles.size(); i++)
		{
			for (int j = 0; j < elements[eles[i]]; j++)
			{
				coordinates coord;
				for (int c = 0; c < 3; ++c)
				{
					file >> line;
					coord.getCoord()[c] = stod(line);

				}
				atom temp(eles[i], "000", coord);
				atoms.push_back(temp);
			}
		}
		file.close();
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
	double max = 0;
	
	//finding close atoms of each atom
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
						double max = 0;
						double dist;
						coordinates temp = atoms[b].getC();
						temp.setX(temp.getX() + i * lattice1[0]);
						temp.setY(temp.getY() + j * lattice2[1]);
						temp.setZ(temp.getZ() + k * lattice3[2]);
						atom tempAtom(atoms[b].getAtomN(), atoms[b].getType(), to_string(i) + to_string(j) + to_string(k), temp);

						dist = mult * atoms[a].distance(tempAtom);
						double d = EstimateDis(atoms[a].getType(), atoms[b].getType());
						
						
						if (dist < EstimateDis(atoms[a].getType(), atoms[b].getType()) && dist != 0)
						{
							closeAtoms.push_back(tempAtom);
						}
					}
				}
			}
		}
		atoms[a].setClose(closeAtoms);
	}
	

	//length graph of two atoms
	const int intervals = 60;
	int BASE[intervals];
	vector<atom> visit;
	double range = EstimateDis(atoms[0].getType(), atoms[1].getType());
	double interval = range / intervals;

	for (int i = 0; i < intervals; i++)
		BASE[i] = 0;

	for (int i = 0; i < atoms.size(); i++)
	{
		for (int j = 0; j < atoms[i].getClose().size(); j++)
		{
			if (!visited(atoms[i].getClose()[j], visit))
			{
				int index = (mult * atoms[i].distance(atoms[i].getClose()[j])) / interval;
				BASE[index]++;
			}
			visit.push_back(atoms[i].getClose()[j]);
		}
	}
	visit.clear();



	//output the distance distribution 
	ofstream lengthDistribution;
	lengthDistribution.open("length graph.txt");
	for (int i = 1; i < intervals; i++)
		lengthDistribution << i * interval << " " << BASE[i]/(i*i*interval*interval) << endl;
	lengthDistribution.close();

	ofstream plot;


	//print the result to close atoms file
	stack<int> number;
	ofstream close_atoms;
	close_atoms.open("close atoms.txt"); 
	if (close_atoms.is_open())
	{
		bool flage = true;

		for (int i = 0; i < size; i++)
		{
			close_atoms 
				<< "number: " << atoms[i].getAtomN() << "      type: " << atoms[i].getType() << "     box: " << atoms[i].getBoxN() << "     coordinate: {"
				<< atoms[i].getC().getX() << ", " << atoms[i].getC().getY() << ", " << atoms[i].getC().getZ() << "}" << endl;
			close_atoms << "The closest atoms: " << endl;
			if (atoms[i].getClose().size() != 4 && (atoms[i].getType() == 13 || atoms[i].getType() == 5))
			{
				number.push(i);
			}
			if (atoms[i].getClose().size() != 1 && atoms[i].getType() == 0)
			{
				number.push(i);
			}
			for (int j = 0; j < atoms[i].getClose().size(); j++)
			{
				close_atoms << "number: " << atoms[i].getClose()[j].getAtomN()
					<< "      type: " << atoms[i].getClose()[j].getType()
					<< "     box: " << atoms[i].getClose()[j].getBoxN()
					<< "     coordinate: {"
					<< atoms[i].getClose()[j].getC().getX() << ", "
					<< atoms[i].getClose()[j].getC().getY() << ", "
					<< atoms[i].getClose()[j].getC().getZ() << "}     "
					<< "distance: " << atoms[i].distance(atoms[i].getClose()[j]) * mult
					<< endl;
				if (atoms[i].getClose().size() != 4)
				{
					number.push(i);
				}
			}
			close_atoms << endl;
		}
		
		while (!number.empty())
		{
			close_atoms << number.top() << " ";
			number.pop();
		}
		close_atoms.close();
	}

	//output the distance distrribution of each combination of elements. 
	string base(".txt");
	int flag = 0;
	for (int i = 0; i < eles.size(); i++)
	{
		for (int j = i; j < eles.size(); j++)
		{
			ofstream histo(SYM[eles[i]] + " - " + SYM[eles[j]] + base);
			{
				for (int k = flag; k < flag + elements[eles[i]]; k++)
				{
					for (int l = 0; l < atoms[k].getClose().size(); l++)
					{
						if(atoms[k].getClose()[l].getType() == eles[j] && !visited(atoms[k].getClose()[l], visit))
							histo << atoms[k].distance(atoms[k].getClose()[l]) * mult << endl;
					}
					visit.push_back(atoms[k]);
				}
			}
			histo.close();
			visit.clear();
		}
		flag += elements[eles[i]];
	}
	return 0;
}

//matrix multiplication 
coordinates matrixMult(double x[], double y[], double z[], coordinates c)
{
	coordinates temp;
	temp.setX(c.getX() * x[0] + c.getY() * y[0] + c.getZ() * z[0]);
	temp.setY(c.getX() * x[1] + c.getY() * y[1] + c.getZ() * z[1]);
	temp.setZ(c.getX() * x[2] + c.getY() * y[2] + c.getZ() * z[2]);
	return temp;
}

//finding the estimate distance
double EstimateDis(int x, int y)
{
	const double COV_RAD[118] = { 0.32,0.93,1.23,0.9,0.82,0.77,0.75,0.73,0.72,0.71,1.54,1.36,1.18,1.11,1.06,1.02,0.99,0.98,2.03,1.74,1.44,1.32,1.22,1.18,1.17,1.17,1.16,1.15,1.17,1.25,1.26,1.22,1.2,1.16,1.14,1.12,2.16,1.91,1.62,1.45,1.34,1.3,1.27,1.25,1.25,1.28,1.34,1.48,1.44,1.41,1.41,1.36,1.33,1.31,2.35,1.98,1.69,1.65,1.65,1.64,1.63,1.62,1.85,1.61,1.59,1.59,1.58,1.57,1.56,1.74,1.56,1.44,1.34,1.3,1.28,1.26,1.27,1.3,1.34,1.49,1.48,1.47,1.46,1.46,1.45,0,0,0,0,1.65,0,1.42,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	//return 1.6 * (COV_RAD[x] + COV_RAD[y]);
	//return 2.809573136;
	return 4;
}

bool compare( atom & l,  atom & r)
{
	return l.getDis() < r.getDis();
}

//checking whether an atom is visited
bool visited(atom n, vector<atom> v)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (n.getAtomN() == v[i].getAtomN() && n.getBoxN() == v[i].getBoxN())
			return true;
	}
	return false;
}

