// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_RealMatrix_hpp
#define CF_RealMatrix_hpp

////////////////////////////////////////////////////////////////////////////////

#include "Math/MatrixT.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace CF {

////////////////////////////////////////////////////////////////////////////////

#ifdef CF_HAVE_CXX_EXPLICIT_TEMPLATES
  // explicit template instantiation
  Math_TEMPLATE template class Math_API Math::MatrixT<Real>;
  Math_TEMPLATE template class Math_API Math::MatrixSliceT<Real>;
#endif

/// RealMatrix is a MatrixT templatized with a Real
typedef Math::MatrixT<Real> RealMatrix;

/// RealSliceMatrix is a MatrixSliceT templatized with a Real
typedef Math::MatrixSliceT<Real> RealSliceMatrix;

////////////////////////////////////////////////////////////////////////////////

} // namespace CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_RealMatrix_hpp
