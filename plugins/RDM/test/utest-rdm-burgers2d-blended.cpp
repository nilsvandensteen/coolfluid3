// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test module for CF::RDM::ScalarAdvection"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem/path.hpp>

#include "Common/CreateComponent.hpp"
#include "Common/ComponentPredicates.hpp"
#include "Common/Log.hpp"
#include "Common/CLink.hpp"
#include "Common/Foreach.hpp"

#include "Solver/CSolver.hpp"
#include "Solver/CModel.hpp"
#include "Solver/CPhysicalModel.hpp"
#include "Solver/Actions/CLoop.hpp"

#include "Mesh/LoadMesh.hpp"
#include "Mesh/CCells.hpp"
#include "Mesh/CMeshReader.hpp"
#include "Mesh/CMeshWriter.hpp"
#include "Mesh/CDomain.hpp"
#include "Mesh/CRegion.hpp"
#include "Mesh/Actions/CBubbleEnrich.hpp"
#include "Mesh/Actions/CBubbleRemove.hpp"

#include "RDM/RKRD.hpp"
#include "RDM/ScalarAdvection.hpp"

using namespace CF;
using namespace CF::Common;
using namespace CF::Common::XML;
using namespace CF::Mesh;
using namespace CF::Solver;
using namespace CF::Solver::Actions;
using namespace CF::RDM;

//#define BUBBLE

struct burgers2d_global_fixture
{
  burgers2d_global_fixture()
  {
    burgers2d_wizard = allocate_component<ScalarAdvection>("mymodel");

    SignalFrame frame("", "", "");
    SignalFrame& options = frame.map( Protocol::Tags::key_options() );

    options.set_option<std::string>("Model name","mymodel");
    options.set_option<std::string>("Physical model","Burgers2D");

    burgers2d_wizard->signal_create_model(frame);
  }

  ScalarAdvection::Ptr burgers2d_wizard;

};

struct burgers2d_local_fixture
{
  burgers2d_local_fixture() :
    model  ( * Core::instance().root()->get_child("mymodel")->as_ptr<CModel>() ),
    domain ( find_component_recursively<CDomain>(model)  ),
    solver ( find_component_recursively<CSolver>(model) )
  {}

  CModel& model;
  CDomain& domain;
  CSolver& solver;
};

//////////////////////////////////////////////////////////////////////////////

BOOST_GLOBAL_FIXTURE( burgers2d_global_fixture )

BOOST_AUTO_TEST_SUITE( burgers2d_test_suite )

//////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_CASE( check_tree , burgers2d_local_fixture )
{
  BOOST_CHECK(true);

  SignalFrame frame("", "", "");

  Core::instance().root()->list_tree(frame);

//  CFinfo << model.tree() << CFendl;
}

//////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_CASE( read_mesh , burgers2d_local_fixture )
{
  BOOST_CHECK(true);

  // create the xml parameters for the read mesh signal

  SignalFrame frame("", "", "");
  SignalFrame& options = frame.map( Protocol::Tags::key_options() );

  BOOST_CHECK(true);

  URI file ( "file:square1x1-tg-p2.msh" );
//  URI file( "file:rotation-tg-p1.neu" );
//  URI file( "file:rotation-qd-p1.neu" );
//  URI file( "file:advection-tg-p2.msh" );
//  URI file( "file:advection-qd-p2.msh" );
//  URI file( "file:rotation-tg-p3.msh" );

  options.set_option<URI>("File", file );

  domain.signal_load_mesh( frame );

  BOOST_CHECK_NE( domain.get_child_count(), (Uint) 0);

#ifdef BUBBLE // enrich the mesh with bubble functions
  CMeshTransformer::Ptr enricher =
      create_component_abstract_type<CMeshTransformer>("CF.Mesh.Actions.CBubbleEnrich","enricher");

  domain.add_component( enricher );

  CMesh::Ptr mesh = find_component_ptr<CMesh>(domain);

  enricher->transform( mesh );
#endif

}

//////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_CASE( setup_iterative_solver , burgers2d_local_fixture )
{
  BOOST_CHECK(true);

  solver.configure_property("Domain",URI("cpath:../Domain"));
  solver.configure_property("Number of Iterations", 1500u);
}

//////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_CASE( signal_create_boundary_term , burgers2d_local_fixture )
{
  BOOST_CHECK(true);

  SignalFrame frame("", "", "");
  SignalFrame& options = frame.map( Protocol::Tags::key_options() );

  std::vector<URI> bc_regions;
  boost_foreach( const CRegion& region, find_components_recursively_with_name<CRegion>(domain,"inlet"))
    bc_regions.push_back( region.full_path() );
  boost_foreach( const CRegion& region, find_components_recursively_with_name<CRegion>(domain,"left"))
    bc_regions.push_back( region.full_path() );
  boost_foreach( const CRegion& region, find_components_recursively_with_name<CRegion>(domain,"right"))
    bc_regions.push_back( region.full_path() );

  BOOST_CHECK_EQUAL( bc_regions.size() , 3u);


  std::string name = "INLET";

  options.set_option<std::string>("Name",name);
  options.set_option<std::string>("Type","CF.RDM.BcDirichlet");
  options.set_array("Regions", bc_regions, " ; ");

  solver.as_ptr<RKRD>()->signal_create_boundary_term(frame);

  Component::Ptr inletbc = find_component_ptr_recursively_with_name( solver, name );
  cf_assert( is_not_null(inletbc) );

  inletbc->get_child("action")->configure_property("Function", std::string("1.5-2.0*x") );

//  CFinfo << find_component_recursively<CModel>(*Core::instance().root()).tree() << CFendl;

  BOOST_CHECK(true);
}

//////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_CASE( create_domain_term , burgers2d_local_fixture )
{
  BOOST_CHECK(true);

  CMesh::Ptr mesh = find_component_ptr<CMesh>(domain);

  SignalFrame frame("", "", "");
  SignalFrame& options = frame.map( Protocol::Tags::key_options() );

  std::vector<URI> bc_regions;
  boost_foreach( const CRegion& region, find_components_recursively_with_name<CRegion>(*mesh,"topology"))
    bc_regions.push_back( region.full_path() );

  BOOST_CHECK_EQUAL( bc_regions.size() , 1u);

  options.set_option<std::string>("Name","INTERNAL");
  options.set_option<std::string>("Type","CF.RDM.CBlended<Burgers2D>");
  options.set_array("Regions", bc_regions, " ; ");

  solver.as_ptr<RKRD>()->signal_create_domain_term(frame);

//  CFinfo << find_component_recursively<CModel>(*Core::instance().root()).tree() << CFendl;

  BOOST_CHECK(true);
}

//////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_CASE( solve , burgers2d_local_fixture )
{
  BOOST_CHECK(true);

//  CFinfo << model.tree() << CFendl;

  solver.solve();
}

////////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_CASE( output , burgers2d_local_fixture )
{
  BOOST_CHECK(true);

  CMesh::Ptr mesh = find_component_ptr<CMesh>(domain);

#ifdef BUBBLE // remove the bubble functions from the mesh
  CMeshTransformer::Ptr remover =
      create_component_abstract_type<CMeshTransformer>("CF.Mesh.Actions.CBubbleRemove","remover");

  domain.add_component( remover );
  remover->transform( mesh );
#endif

  BOOST_CHECK(true);

  CMeshWriter::Ptr mesh_writer = create_component_abstract_type<CMeshWriter> ( "CF.Mesh.Gmsh.CWriter", "GmshWriter" );
  model.add_component(mesh_writer);

  std::vector<URI> fields;
  boost_foreach(const CField2& field, find_components_recursively<CField2>(*mesh))
    fields.push_back(field.full_path());

  mesh_writer->configure_property("Fields",fields);
  mesh_writer->configure_property("File",model.name()+".msh");
  mesh_writer->configure_property("Mesh",mesh->full_path());

  mesh_writer->write();

}

//////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////
