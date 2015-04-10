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

#ifndef FACTORIAL_H
#define FACTORIAL_H

#include <vector>
#include <iostream>
#include <assert.h>

using std::vector;

/**
 * A template class for precomputation and subsequent retrieval of factorials
 * of an integer number.
 *
 * The maximal input parameter is set to 19 at first, since double and __int64
 * can represent exactly numbers of 18 or less digits. There is an option to change this
 * in case one uses a data type being capable of representing bigger numbers.
 */
template<class T>
class Factorial
{
public:
    /** Gets the factorial of _i */
    static T Get (int _i);
    /** Gets _i*(_i+1)*...*(_j-1)*_j */
    static T Get (int _i, int _j);
    /** Sets the maximal stored factorial value to _max */
    static void SetMax (int _max);
    /** Gets the maximal stored factorial value */
    static int GetMax ();

private:
    /** Computes factorials of numbers [1..max] */
    static void ComputeFactorials ();

    static int max_;
    static vector<T>           factorials_;
    static vector<vector<T> >  subFactorials_;
};

// The obligatory initialization of static attributes
template<class T> 
int Factorial<T>::max_ = 19;

template<class T> 
vector<T> Factorial<T>::factorials_;

/**
 * Computes factorials up to max_ and stores them internally
 */
template<class T>
inline void Factorial<T>::ComputeFactorials ()
{
    factorials_.resize (max_);

    factorials_[0] = (T)1;

    for (int i=1; i<max_; ++i)
    {
        factorials_[i] = factorials_[i-1] * (T)(i+1);
    }
}

/**
 * Retrieves the factorial of _i. All factorials are computed only the first 
 * time this function is called, after this they are just read from the store.
 */
template<class T>
inline T Factorial<T>::Get (int _i)
{
    assert (_i >= 0 && _i <= max_);

    if (!factorials_.size ())
    {
        ComputeFactorials ();
    }

    // 0! = 1
    if (!_i)
    {
        return 1;
    }
    
    return factorials_[_i-1];
}

template<class T>
inline T Factorial<T>::Get (int _i, int _j)
{
    T result = (T)1;

    for (int i=_j; i>=_i; --i)
    {
        result *= i;
    }

    return result;
}

/*
 * Modifies the maximum factorial input parameter. All factorials are recomputed here
 */
template<class T>
inline void Factorial<T>::SetMax (int _max)
{
    assert (_max >= (T)0);

    // In fact, the previously computed factorials could be reused here,
    // however, since this is rarely performed and takes only a couple of
    // multiplications, we don't care.
    max_ = _max;
    if (max_ <= _max)
    {
        ComputeFactorials ();
    }
}


template<class T>
inline int Factorial<T>::GetMax ()
{
    return max_;
}
#endif
