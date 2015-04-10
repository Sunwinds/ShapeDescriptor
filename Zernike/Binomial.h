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

#ifndef BINOMIAL_H
#define BINOMIAL_H

#include <vector>
#include <assert.h>

using std::vector;

/**
 * A template class facilitating fast computation and retrieval of binomials. 
 * The binomials are computed only once at first call of Get function according
 * to Pascal's Triangle.
 */
template<class T>
class Binomial
{
public:
    typedef vector<T>           VectorT;
    typedef vector<VectorT>     VVectorT;

    /** Retrieves the binomial _i "over" _j */
    static T Get (int _i, int _j);
    /** Sets the maximal value of upper binomial param to _max */
    static void SetMax (int _max);
    /** Gets the maximal value of upper binomial param */
    static int GetMax ();

private:
    /** Computed Pascal's Triangle */
    static void ComputePascalsTriangle ();

    static VVectorT pascalsTriangle_;
    static int max_;
};      


template<class T>
inline void Binomial<T>::SetMax (int _max)
{
    max_ = _max;
    ComputePascalsTriangle ();
}


template<class T>
inline int Binomial<T>::GetMax ()
{
    return max_;
}


template<class T>
typename Binomial<T>::VVectorT Binomial<T>::pascalsTriangle_;

template<class T>
int Binomial<T>::max_ = 60;


#include "Binomial.cpp"

#endif
