// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Common_OSystem_hpp
#define CF_Common_OSystem_hpp

#include "Common/Exception.hpp"
#include "Common/SafePtr.hpp"
#include "Common/CommonAPI.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace CF {
  namespace Common {

  class OSystemLayer;
  class LibLoader;

////////////////////////////////////////////////////////////////////////////////

class Common_API OSystemError : public Common::Exception {
public:
  /// Constructor
  OSystemError ( const Common::CodeLocation& where, const std::string& what);
}; // end class OSystem

////////////////////////////////////////////////////////////////////////////////

/// Represents the operating system
/// @author Tiago Quintino
class Common_API OSystem : public boost::noncopyable {

public: // methods

  /// @return the single object that represents the operating system
  static OSystem& instance();

  /// @return ProcessInfo object
  Common::SafePtr<Common::OSystemLayer> OSystemLayer();

  /// @return LibLoader object
  Common::SafePtr<Common::LibLoader> LibLoader();

  /// Executes the command passed in the string
  /// @todo should return the output of the command but not yet implemented.
  void executeCommand (const std::string& call);

private: // functions

  /// constructor
  OSystem ();
  /// destructor
  ~OSystem ();

private: // data

  /// memory usage object
  Common::OSystemLayer * m_system_layer;
  /// libloader object
  Common::LibLoader * m_lib_loader;

}; // class FileHandlerOutput

////////////////////////////////////////////////////////////////////////////////

  } // namespace Common
} // namespace CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_Common_OSystem_hpp
