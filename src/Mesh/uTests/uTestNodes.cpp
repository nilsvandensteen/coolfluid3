// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test module for CF::Mesh::LagrangeSF"

#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>

#include "Common/Log.hpp"
#include "Common/CRoot.hpp"
#include "Common/ComponentPredicates.hpp"

#include "Mesh/CMesh.hpp"
#include "Mesh/CRegion.hpp"
#include "Mesh/CElements.hpp"
#include "Mesh/CArray.hpp"
#include "Mesh/CMeshReader.hpp"
#include "Mesh/CMeshWriter.hpp"
#include "Mesh/ElementNodes.hpp"

#include "Mesh/Integrators/Gauss.hpp"

#include "Tools/Testing/Difference.hpp"

using namespace CF;
using namespace CF::Mesh;
using namespace CF::Mesh::Integrators;
using namespace CF::Common;

//////////////////////////////////////////////////////////////////////////////

struct Nodes_Fixture
{
  /// common setup for each test case
  Nodes_Fixture() : mesh2d(new CMesh  ( "mesh2d" ))
  {
     // uncomment if you want to use arguments to the test executable
     //int*    argc = &boost::unit_test::framework::master_test_suite().argc;
     //char*** argv = &boost::unit_test::framework::master_test_suite().argv;

    // Read the a .neu mesh as 2D mixed mesh
    boost::shared_ptr<CMeshReader> meshreader = create_component_abstract_type<CMeshReader>("Neu","meshreader");

    // the file to read from
    boost::filesystem::path fp_in ("quadtriag.neu");

    // Read the mesh
    meshreader->read_from_to(fp_in,mesh2d);
  }

  /// common tear-down for each test case
  ~Nodes_Fixture()
  {
  }
  /// common values accessed by all tests goes here
  boost::shared_ptr<CMesh> mesh2d;

  CElements& get_first_region()
  {
    BOOST_FOREACH(CElements& region, recursive_range_typed<CElements>(*mesh2d))
    {
        return (region);
    }
    throw ShouldNotBeHere(FromHere(), "");
  }

};

//////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_SUITE( Nodes, Nodes_Fixture )

//////////////////////////////////////////////////////////////////////////////

/// Test node modification
BOOST_AUTO_TEST_CASE( writeNodes )
{
  CElements& firstRegion = get_first_region();
  ElementNodeView nodes(firstRegion.coordinates(), firstRegion.connectivity_table().array()[0]);
  nodes[0][0] = 1.;
  const ConstElementNodeView const_nodes(firstRegion.coordinates(), firstRegion.connectivity_table().array()[0]);
  BOOST_CHECK_EQUAL(nodes[0][0], const_nodes[0][0]);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////

