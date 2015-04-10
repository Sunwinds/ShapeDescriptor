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

template<class VoxelT, class MomentT>
ScaledGeometricalMoments<VoxelT,MomentT>::ScaledGeometricalMoments ()
{
}

template<class VoxelT, class MomentT>
ScaledGeometricalMoments<VoxelT,MomentT>::ScaledGeometricalMoments (
        const VoxelT* _voxels,
        int _xDim, int _yDim, int _zDim, 
        double _xCOG, double _yCOG, double _zCOG,
        double _scale,
        int _maxOrder)
{
   Init (_voxels, _xDim, _yDim, _zDim, _xCOG, _yCOG, _zCOG, _scale, _maxOrder);
}


template<class VoxelT, class MomentT>
ScaledGeometricalMoments<VoxelT,MomentT>::ScaledGeometricalMoments (
        const VoxelT* _voxels,
        int _dim,
        double _xCOG, double _yCOG, double _zCOG,
        double _scale,
        int _maxOrder)
{
   Init (_voxels, _dim, _dim, _dim, _xCOG, _yCOG, _zCOG, _scale, _maxOrder);
}


template<class VoxelT, class MomentT>
void ScaledGeometricalMoments<VoxelT,MomentT>::Init (
        const VoxelT* _voxels,
        int _xDim, int _yDim, int _zDim, 
        double _xCOG, double _yCOG, double _zCOG,
        double _scale,
        int _maxOrder)
{
    xDim_ = _xDim;
    yDim_ = _yDim;
    zDim_ = _zDim;

    maxOrder_ = _maxOrder;

    size_t totalSize = xDim_ * yDim_ * zDim_;
    voxels_.resize (totalSize);
    for (int i=0; i<totalSize; ++i)
    {
        voxels_[i] = _voxels[i];
    }

    moments_.resize (maxOrder_ + 1);
    for (int i=0; i<=maxOrder_; ++i)
    {
        moments_[i].resize (maxOrder_ - i + 1);
        for (int j=0; j<=maxOrder_ - i; ++j)
        {
            moments_[i][j].resize (maxOrder_ - i - j + 1);
        }
    }

    ComputeSamples (_xCOG, _yCOG, _zCOG, _scale);

    Compute ();
}   


template<class VoxelT, class MomentT>
void ScaledGeometricalMoments<VoxelT,MomentT>::ComputeSamples (double _xCOG, double _yCOG, double _zCOG, double _scale)
{
    samples_.resize (3);    // 3 dimensions

    int dim[3];
    dim[0] = xDim_;
    dim[1] = yDim_;
    dim[2] = zDim_;

    double min[3];
    min[0] = (-_xCOG) * _scale;
    min[1] = (-_yCOG) * _scale;
    min[2] = (-_zCOG) * _scale;

    for (int i=0; i<3; ++i)
    {
        samples_[i].resize (dim[i]+1);
        for (int j=0; j<=dim[i]; ++j)
        {
            samples_[i][j] = min[i] + j * _scale;
        }
    }      
}


template<class VoxelT, class MomentT>
void ScaledGeometricalMoments<VoxelT,MomentT>::Compute ()
{
    int arrayDim = zDim_;
    int layerDim = yDim_ * zDim_;

    int diffArrayDim =  zDim_ + 1;
    int diffLayerDim = (yDim_ + 1) * zDim_;
    int diffGridDim  = (xDim_ + 1) * layerDim;

    T1D diffGrid (diffGridDim);
    T1D diffLayer (diffLayerDim);
    T1D diffArray (diffArrayDim);

    T1D layer (layerDim);
    T1D array (arrayDim);
    T   moment;

    typename T1D::iterator iter = voxels_.begin ();
    typename T1D::iterator diffIter = diffGrid.begin ();

    // generate the diff version of the voxel grid in x direction
    for (int x=0; x<layerDim; ++x)
    {
        ComputeDiffFunction (iter, diffIter, xDim_);

        iter += xDim_;
        diffIter += xDim_ + 1;
    }

    for (int i=0; i<=maxOrder_; ++i)
    {
        diffIter = diffGrid.begin ();
        for (int p=0; p<layerDim; ++p)
        {
            // multiply the diff function with the sample values
            T1DIter sampleIter (samples_[0].begin ()); 
            layer[p] = Multiply (diffIter, sampleIter, xDim_ + 1);

            diffIter += xDim_ + 1;
        }              

        iter = layer.begin ();
        diffIter = diffLayer.begin ();
        for (int y=0; y<arrayDim; ++y)
        {
            ComputeDiffFunction (iter, diffIter, yDim_);

            iter += yDim_;
            diffIter += yDim_ + 1;
        }

        for (int j=0; j<maxOrder_+1-i; ++j)
        {
            diffIter = diffLayer.begin ();
            for (int p=0; p<arrayDim; ++p)
            {
                T1DIter sampleIter (samples_[1].begin ()); 
                array[p] = Multiply (diffIter, sampleIter, yDim_ + 1);

                diffIter += yDim_ + 1;
            }

            iter = array.begin ();
            diffIter = diffArray.begin ();
            ComputeDiffFunction (iter, diffIter, zDim_);

            for (int k=0; k<maxOrder_+1-i-j; ++k)
            {
                T1DIter sampleIter (samples_[2].begin ()); 

                moment = Multiply (diffIter, sampleIter, zDim_ + 1);
                moments_[i][j][k] = moment / ((1+i) * (1+j) * (1+k));
            }
        }
    }
}


template<class VoxelT, class MomentT>
void ScaledGeometricalMoments<VoxelT,MomentT>::ComputeDiffFunction (T1DIter _iter, T1DIter _diffIter, int _dim)
{
    _diffIter[0] = -_iter[0];
    for (int i=1; i<_dim; ++i)
    {
        _diffIter[i] = _iter[i-1] - _iter[i];
    }
    _diffIter[_dim] = _iter[_dim-1];
}


template<class VoxelT, class MomentT>
MomentT ScaledGeometricalMoments<VoxelT,MomentT>::Multiply (T1DIter _diffIter, T1DIter _sampleIter, int _dim)
{
    T sum (0);
    for (int i=0; i<_dim; ++i)
    {
        _diffIter[i] *= _sampleIter[i];
        sum += _diffIter[i];
    }

    return sum;
}

template<class VoxelT, class MomentT>
MomentT ScaledGeometricalMoments<VoxelT,MomentT>::GetMoment (int _i, int _j, int _k)
{
    return moments_[_i][_j][_k];
}
