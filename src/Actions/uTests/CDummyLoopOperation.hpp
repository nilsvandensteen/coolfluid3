// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Actions_CDummyLoopOperation_hpp
#define CF_Actions_CDummyLoopOperation_hpp

#include "Mesh/CFieldElements.hpp"

#include "Actions/CLoopOperation.hpp"

/////////////////////////////////////////////////////////////////////////////////////

namespace CF {
	
  namespace Mesh
  {
		template <typename T> class CTable;
		class CFieldElements;
	}

///////////////////////////////////////////////////////////////////////////////////////

class CDummyLoopOperation : public Actions::CLoopOperation
{
public: // typedefs

  /// pointers
  typedef boost::shared_ptr<CDummyLoopOperation> Ptr;
  typedef boost::shared_ptr<CDummyLoopOperation const> ConstPtr;

public: // functions
  /// Contructor
  /// @param name of the component
  CDummyLoopOperation ( const std::string& name );

  /// Virtual destructor
  virtual ~CDummyLoopOperation() {};

  /// Get the class name
  static std::string type_name () { return "CDummyLoopOperation"; }

  /// Set the loop_helper
  void set_loophelper (CF::Mesh::CElements& geometry_elements );
	
  /// execute the action
  virtual void execute ();
	
	/// @return the nodes to loop over
  virtual Mesh::CList<Uint>& loop_list ();
		
private: // data
	
  struct LoopHelper
  {
    LoopHelper(Mesh::CElements& geometry_elements) :
		node_list(geometry_elements.node_list())
    { }
    Mesh::CList<Uint>& node_list;
  };
	
  boost::shared_ptr<LoopHelper> data;

};

} // CF

/////////////////////////////////////////////////////////////////////////////////////

#endif // CF_Actions_CDummyLoopOperation_hpp