/*
                                                                            
                          3D Zernike Moments                                
    Copyright (C) 2003 by Computer Graphics Group, University of Bonn       
           http://www.cg.cs.uni-bonn.de/project-pages/3dsearch/             
                                                                            
Code by Marcin Novotni:     marcin@cs.uni-bonn.de
       
for more information, see the paper:

@inproceedings{novotni-2003-3d,
    author = {M. Novotni and R. Klein},
    title = {3{D} {Z}ernike Descriptors for Content Based Shape Retrieval},
    booktitle = {The 8th ACM Symposium on Solid Modeling and Applications},
    pages = {216--225},
    year = {2003},
    month = {June},
    institution = {Universit\"{a}t Bonn},
    conference = {The 8th ACM Symposium on Solid Modeling and Applications, June 16-20, Seattle, WA}
}        
 *---------------------------------------------------------------------------* 
 *                                                                           *
 *                                License                                    *
 *                                                                           *
 *  This library is free software; you can redistribute it and/or modify it  *
 *  under the terms of the GNU Library General Public License as published   *
 *  by the Free Software Foundation, version 2.                              *
 *                                                                           *
 *  This library is distributed in the hope that it will be useful, but      *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU        *
 *  Library General Public License for more details.                         *
 *                                                                           *
 *  You should have received a copy of the GNU Library General Public        *
 *  License along with this library; if not, write to the Free Software      *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *                                                                           *
\*===========================================================================*/

/*
    This is a demonstration of how the classes may be used to generate the
    3D Zernike descriptors from a given input binary file containing the 
    voxel grid representation of the object.

    Notice that in the below case, the file contains a cubic grid, i.e. the 
    x-, y-, and z-dimensions are equal (such setting should satisfy most needs).
*/

// ---- local includes ----
#include "ZernikeDescriptor.h"

// ---- std includes ----
#include <vector>
#include <complex>
#include <fstream>
#include <sstream>
#include <iostream>

#include <stdio.h>
#include "Vec3f.h"
#include "Utility.h"
#include "Parameters.h"

//// reads a voxel grid from a binary file
//template<class TIn, class TOut>
//TOut* ReadGrid (const char* _fname, int& _dim)
//{
//    std::ifstream infile (_fname, std::ios_base::binary | std::ios_base::in);
//
//    vector<double> tempGrid;
//    TIn temp;
//
//    // read the grid values
//    while (infile.read ((char*)(&temp), sizeof (TIn)))
//    {
//        tempGrid.push_back ((double)temp);
//    }  
//
//    int d = tempGrid.size ();
//    double f = std::pow<double> ((double)d, 1.0/3.0);
//    
//    _dim = floor (f+0.5);
//
//    TOut* result = new TOut [d];
//    for (int i=0; i<d; ++i)
//    {
//        result[i] = tempGrid[i];
//    }
//
//    return result;                  
//}



int main (int argc, char** argv)
{
	//std::string databasePath = "C:/Graphics/Scene_Code/databaseFull/";
	std::string databasePath = "C:/Graphics/Scene_Code/GraphKernelTH/TsinghuaScenes/";

	Parameters paras;
	paras.Init(databasePath + "../GraphKernelTH/parameters.txt");

	std::vector<std::string> modelList; 

	std::ifstream file;
	file.open(databasePath + "fields/" + paras.taskName + "_models.txt");

	// get model names
	while(file.good())
	{
		std::string modelBaseName;
		std::getline(file, modelBaseName);
		
		if (modelBaseName != "")
		{
			modelList.push_back(modelBaseName);
		}
	}

	// compute zernike descriptor
	int level = 20;
	int modelNum = modelList.size();
	std::vector<std::vector<double>> invariantsList;


	for (int j = 0; j < modelNum; j++)
	{
		std::string modelFullName = databasePath + "models_voxel/" + modelList[j] + ".binvox";

		// compute the zernike descriptors
		std::cout<<"Saving invariants for model "<<j<<"/"<<modelNum<<std::endl;

		ZernikeDescriptor<double, float>* zd;
		zd = new ZernikeDescriptor<double, float>(modelFullName.c_str(), level);

		std::vector<double> invariants = zd->GetInvariants();

		invariantsList.push_back(invariants);

		delete zd;
	}


	// save descriptors
	int invariantDim = invariantsList[0].size();

	std::ofstream outfile;
	std::string descriptorFileName = databasePath +"fields/" + paras.taskName + "_descriptors.txt";
	outfile.open(descriptorFileName);

	for (int i = 0; i < modelNum; i++)
	{
		outfile<<modelList[i]<<"\t";

		for (int j = 0; j < invariantDim; j++)
		{
			outfile<<invariantsList[i][j]<<"\t";
			assert(invariantsList[i].size() == invariantDim);
		}


		outfile<<std::endl;
	}

	outfile.close();


	return 0;
}