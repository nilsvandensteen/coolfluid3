// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Common_LogLevel_hpp
#define CF_Common_LogLevel_hpp

////////////////////////////////////////////////////////////////////////////////

namespace CF {

////////////////////////////////////////////////////////////////////////////////

/// Output levels for the Log facility
/// @author Quentin Gasper
enum LogLevel
{
  SILENT = 100,
  NORMAL = 50,
  VERBOSE = 0
}; // !LogLevel

////////////////////////////////////////////////////////////////////////////////

} // namespace CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_Common_LogLevel_hpp
