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

#ifndef ZERNIKEDESCRIPTOR_H
#define ZERNIKEDESCRIPTOR_H

// ---- std includes ---
#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>

// ---- local program includes ----
//#include "GeometricalMoments.h"
#include "ScaledGeometricMoments.h"
#include "ZernikeMoments.h"

/**
 * This class serves as a wrapper around the geometrical and
 * Zernike moments. It provides also the implementation of invariant Zernike
 * descriptors, means of reconstruction of orig. function, etc.
 */
template<class T, class TIn>
class ZernikeDescriptor
{
public:
    // ---- exported typedefs ----
    /// complex type
    typedef std::complex<T>                         ComplexT;       
    /// 3D array of complex type
    typedef vector<vector<vector<ComplexT> > >      ComplexT3D;     


    //typedef vector<T>                               T1D;
   // typedef vector<T1D>                             T2D;            // 2D array of T type

    //typedef CumulativeMoments<T, T>                 CumulativeMomentsT;
    typedef ScaledGeometricalMoments<T, T>          ScaledGeometricalMomentsT;
    typedef ZernikeMoments<T, T>                    ZernikeMomentsT;

public:
    // ---- public functions ----
    ZernikeDescriptor (
        const char* _rawName,       /**< binary input file name (contains a cubic grid)*/
        int _order                  /**< the maximal order of the moments (N in paper) */
        );
    ZernikeDescriptor (
        T* _voxels,                 /**< the cubic voxel grid */
        int _dim,                   /**< dimension is $_dim^3$ */
        int _order                  /**< maximal order of the Zernike moments (N in paper) */
        );
    ZernikeDescriptor ();

    /**
        Reconstructs the original object from the 3D Zernike moments.    
     */
    void Reconstruct (
        ComplexT3D& _grid,          /**< result grid as 3D complex stl vector */
        int _minN = 0,              /**< min value for n freq index */
        int _maxN = 100,            /**< max value for n freq index */
        int _minL = 0,              /**< min value for l freq index */
        int _maxL = 100             /**< max value for l freq index */
        );

    /**
     * Saves the computed invariants into a binary file
     */
    void SaveInvariants (
        const char* _fName      /**< name of the output file */
        );

	void SaveInvariants(
		std::string modelName,
		std::ofstream &outfile
		);
    /// Access to invariants
    vector<T>  GetInvariants ();

private:
    // ---- private helper functions ----
    void NormalizeGrid ();
    void ComputeNormalization ();
    void ComputeMoments ();
    void ComputeInvariants ();
    void WriteGrid (
        ComplexT3D& _grid, 
        const char* _fName);

    double ComputeScale_BoundingSphere (
        T* _voxels, 
        int _dim, 
        T _xCOG, 
        T _yCOG, 
        T _zCOG
        );
    double ComputeScale_RadiusVar (
        T* _voxels, 
        int _dim, 
        T _xCOG, 
        T _yCOG, 
        T _zCOG
        );

    T* ReadGrid (
        const char* _fname, 
        int& _dim_);

	T* ReadBinvoxGrid(
		const char* _fname, 
		int& _dim_);



private:
    // ---- member variables ----
    int     order_;                 // maximal order of the moments to be computed (max{n})
    int     dim_;                   // length of the edge of the voxel grid (which is a cube)

    T*      voxels_;                // 1D array containing the voxels
    T       zeroMoment_,            // zero order moment
            xCOG_, yCOG_, zCOG_,    // center of gravity  
            scale_;                 // scaling factor mapping the function into the unit sphere

    //T2D                 invariants_;        // 2D vector of invariants under SO(3)
    vector<T>                  invariants_;        // 2D vector of invariants under SO(3)

    ZernikeMomentsT     zm_;
    //CumulativeMomentsT  cm_;
    ScaledGeometricalMomentsT gm_;  
};

#include "ZernikeDescriptor.cpp"

#endif