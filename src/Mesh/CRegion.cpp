// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "Common/CGroup.hpp"
#include "Common/CLink.hpp"
#include "Common/ObjectProvider.hpp"

#include "Mesh/CRegion.hpp"
#include "Mesh/CField.hpp"
#include "Mesh/CElements.hpp"
#include "Mesh/CTable.hpp"
#include "Mesh/CArray.hpp"

namespace CF {
namespace Mesh {

using namespace Common;

Common::ObjectProvider < CRegion, Component, MeshLib, NB_ARGS_1 >
CRegion_Provider ( CRegion::type_name() );


////////////////////////////////////////////////////////////////////////////////

CRegion::CRegion ( const CName& name  ) :
  Component ( name )
{
  BUILD_COMPONENT;
}

////////////////////////////////////////////////////////////////////////////////

CRegion::~CRegion()
{
}

////////////////////////////////////////////////////////////////////////////////

CRegion& CRegion::create_region( const CName& name )
{
  CRegion::Ptr region = get_child_type<CRegion>(name);
  if (!region)
  {
    region = create_component_type<CRegion>(name);
  }
  return *region;

}

////////////////////////////////////////////////////////////////////////////////

CElements& CRegion::create_elements(const std::string& element_type_name, CArray& coordinates)
{
  std::string name = "elements_" + element_type_name;
  
  CElements::Ptr elements = get_child_type<CElements>(name);
  if (!elements)
  {
    elements = create_component_type<CElements>(name);
    elements->add_tag("GeometryElements");
    elements->initialize(element_type_name,coordinates);
  }
  return *elements;
}

//////////////////////////////////////////////////////////////////////////////

CArray& CRegion::create_coordinates(const Uint& dim)
{  
  CArray::Ptr coordinates = get_child_type<CArray>("coordinates");
  if (!coordinates)
  {
    coordinates = create_component_type<CArray>("coordinates");
    coordinates->add_tag("coordinates");
    coordinates->initialize(dim);
  }
  else if (coordinates->array().shape()[1] != dim)
  {
    coordinates = create_component_type<CArray>("coordinates",NUMBER);
    coordinates->add_tag("coordinates");
    coordinates->initialize(dim);
    
  }
  return *coordinates;
}

//////////////////////////////////////////////////////////////////////////////

void CRegion::add_field_link(CField& field)
{
  CGroup::Ptr field_group = get_child_type<CGroup>("fields");
  if (!field_group.get())
    field_group = create_component_type<CGroup>("fields");
  field_group->create_component_type<CLink>(field.field_name())->link_to(field.get());
}

//////////////////////////////////////////////////////////////////////////////

CField& CRegion::get_field(const CName& field_name)
{
  Component::Ptr all_fields = get_child("fields");
  cf_assert(all_fields.get());
  Component::Ptr field = all_fields->get_child(field_name);
  cf_assert(field.get());
  return *boost::dynamic_pointer_cast<CField>(field->get());
}

//////////////////////////////////////////////////////////////////////////////

Uint CRegion::recursive_elements_count() const
{
  Uint elem_count = 0;
  BOOST_FOREACH(const CElements& elements, recursive_range_typed<CElements>(*this))
    elem_count += elements.elements_count();
  return elem_count;
}
  
//////////////////////////////////////////////////////////////////////////////

Uint CRegion::recursive_nodes_count() const
{
  std::set<const CArray*> coordinates_set;
  BOOST_FOREACH(const CElements& elements, recursive_range_typed<CElements>(*this))
    coordinates_set.insert(&elements.coordinates());

  // Total number of nodes in the mesh
  Uint nb_nodes = 0;
  BOOST_FOREACH(const CArray* coordinates, coordinates_set)
    nb_nodes += coordinates->size();
  
  return nb_nodes;
}

////////////////////////////////////////////////////////////////////////////////

const CRegion& CRegion::subregion(const CName& name) const
{
  return get_named_component_typed<CRegion const>(*this,name);
}

////////////////////////////////////////////////////////////////////////////////

CRegion& CRegion::subregion(const CName& name)
{
  return get_named_component_typed<CRegion>(*this,name);
}

////////////////////////////////////////////////////////////////////////////////

const CElements& CRegion::elements(const CName& name) const
{
  return get_named_component_typed<CElements const>(*this,name);
}

////////////////////////////////////////////////////////////////////////////////

CElements& CRegion::elements(const CName& name)
{
  return get_named_component_typed<CElements>(*this,name);
}

//////////////////////////////////////////////////////////////////////////////


} // Mesh
} // CF
