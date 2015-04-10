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

#ifndef ZERNIKEMOMENTS_H
#define ZERNIKEMOMENTS_H

#pragma warning (disable : 4267)

// ----- local program includes -----
#include "Factorial.h"
#include "Binomial.h"
#include "ScaledGeometricMoments.h"

// ----- std includes -----
#include <complex>
#include <set>
#include <ios>

#define PI 3.141592653589793

/**
 * Struct representing a complex coefficient of a moment
 * of order (p_,q_,r_)
 */
template<class T>
struct ComplexCoeff 
{
    typedef     std::complex<T>     ValueT;

    ComplexCoeff (int, int, int, const ValueT&);
    ComplexCoeff (const ComplexCoeff<T>& _cc);
    ComplexCoeff ();

    int                 p_, q_, r_;
    ValueT     value_;
};

/**
 * Class representing the Zernike moments
 */
template<class VoxelT, class MomentT>
class ZernikeMoments
{
public:
    // ---- public typedefs ----
    typedef MomentT             T;
    typedef vector<T>           T1D;        // vector of scalar type
    typedef vector<T1D>         T2D;        // 2D array of scalar type
    typedef vector<T2D>         T3D;        // 3D array of scalar type
    typedef vector<T3D>         T4D;        // 3D array of scalar type

    typedef std::complex<T>                      ComplexT;       // complex type
    typedef vector<vector<vector<ComplexT> > >   ComplexT3D;     // 3D array of complex type
    
    typedef ComplexCoeff<T>                      ComplexCoeffT;
    typedef vector<vector<vector<vector<ComplexCoeffT> > > >    ComplexCoeffT4D;


public:
    // ---- public member functions ----
    ZernikeMoments (int _order, ScaledGeometricalMoments<VoxelT,MomentT>& _gm);
    ZernikeMoments ();

    void Init (int _order, ScaledGeometricalMoments<VoxelT,MomentT>& _gm);
    void Compute ();

    ComplexT GetMoment (int _n, int _l, int _m);

    // ---- debug functions/arguments ----
    void Reconstruct (ComplexT3D&   _grid,                // grid containing the reconstructed function
                      T             _xCOG,                // center of gravity 
                      T             _yCOG,                  
                      T             _zCOG, 
                      T             _scale,               // scaling factor to map into unit ball
                      int           _minN = 0,            // min value for n freq index 
                      int           _maxN = 100,          // min value for n freq index
                      int           _minL = 0,            // min value for l freq index
                      int           _maxL = 100);         // max value for l freq index

    void NormalizeGridValues (ComplexT3D& _grid);
    void CheckOrthonormality (int _n1, int _l1, int _m1, int _n2, int _l2, int _m2);

private:
    // ---- private member functions ----
    void ComputeCs ();                      
    void ComputeQs ();
    void ComputeGCoefficients ();
    
    // ---- private attributes -----
    ComplexCoeffT4D     gCoeffs_;           // coefficients of the geometric moments
    ComplexT3D          zernikeMoments_;    // nomen est omen
    T3D                 qs_;                // q coefficients (radial polynomial normalization)
    T2D                 cs_;                // c coefficients (harmonic polynomial normalization)

    ScaledGeometricalMoments<VoxelT,MomentT> gm_;
    int                 order_;             // := max{n} according to indexing of Zernike polynomials

    // ---- debug functions/arguments ----
    void PrintGrid (ComplexT3D& _grid);
    T EvalMonomialIntegral (int _p, int _q, int _r, int _dim);
};

#include "ZernikeMoments.inl"
#include "ZernikeMoments.cpp"

#endif