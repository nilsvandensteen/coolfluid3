// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.
//
// IMPORTANT:
// run it both on 1 and many cores
// for example: mpirun -np 4 ./test-parallel-environment --report_level=confirm or --report_level=detailed

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test module for CF::Common 's parallel environment - part of testing the commpattern."

////////////////////////////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "Common/Log.hpp"
#include "Common/ComponentPredicates.hpp"
#include "Common/Component.hpp"
#include "Common/MPI/PE.hpp"
#include "Common/MPI/PEObjectWrapper.hpp"
#include "Common/MPI/PECommPattern.hpp"
#include "Common/CGroup.hpp"

/*
  TODO: move to allocate_component
*/

////////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace boost;
using namespace CF;
using namespace CF::Common;

////////////////////////////////////////////////////////////////////////////////

struct PECommPatternFixture
{
  /// common setup for each test case
  PECommPatternFixture()
  {
    m_argc = boost::unit_test::framework::master_test_suite().argc;
    m_argv = boost::unit_test::framework::master_test_suite().argv;
  }

  /// common tear-down for each test case
  ~PECommPatternFixture()
  {
    PE::instance().finalize();
  }

  /// common params
  int m_argc;
  char** m_argv;
};

////////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_SUITE( PECommPatternSuite, PECommPatternFixture )

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( init )
{
  PE::instance().init(m_argc,m_argv);
  BOOST_CHECK_EQUAL( PE::instance().is_init() , true );
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( ObjectWrapperPtr )
{
  int i,j;
  double *d1=new double[32];
  double *d2=new double[24];
  std::vector<int> map(4);

  for(i=0; i<32; i++) d1[i]=32.+(double)i;
  for(i=0; i<24; i++) d2[i]=64.+(double)i;
  for(i=0; i<4; i++) map[i]=2+i;

  PEObjectWrapperPtr<double>::Ptr w1=allocate_component_type< PEObjectWrapperPtr<double> >("Ptr1");
  PEObjectWrapperPtr<double>::Ptr w2=allocate_component_type< PEObjectWrapperPtr<double> >("Ptr2");

  w1->setup(d1,32,2,true);
  w2->setup(d2,24,3,false);

  BOOST_CHECK_EQUAL( w1->needs_update() , true );
  BOOST_CHECK_EQUAL( w2->needs_update() , false );

  BOOST_CHECK_EQUAL( w1->is_data_type_Uint() , false );
  BOOST_CHECK_EQUAL( w2->is_data_type_Uint() , false );

  BOOST_CHECK_EQUAL( w1->size() , 16 );
  BOOST_CHECK_EQUAL( w2->size() , 8 );

  BOOST_CHECK_EQUAL( w1->stride() , 2 );
  BOOST_CHECK_EQUAL( w2->stride() , 3 );

  BOOST_CHECK_EQUAL( w1->size_of() , sizeof(double) );
  BOOST_CHECK_EQUAL( w2->size_of() , sizeof(double) );

  double *dtest1=(double*)w1->pack(map);
  double *dtest2=(double*)w2->pack(map);

  for(i=0; i<8; i++) { BOOST_CHECK_EQUAL( dtest1[i] , 32+4+i ); dtest1[i]*=-1.; }
  for(i=0; i<12; i++) { BOOST_CHECK_EQUAL( dtest2[i] , 64+6+i ); dtest2[i]*=-1.; }

  w1->unpack(map,dtest1);
  w2->unpack(map,dtest2);

  double *dtesttest1=(double*)w1->pack(map);
  double *dtesttest2=(double*)w2->pack(map);

  for(i=0; i<8; i++) { BOOST_CHECK_EQUAL( dtesttest1[i] , -32-4-i ); }
  for(i=0; i<12; i++) { BOOST_CHECK_EQUAL( dtesttest2[i] , -64-6-i ); }

  delete[] dtest1;
  delete[] dtest2;
  delete[] dtesttest1;
  delete[] dtesttest2;
  delete[] d1;
  delete[] d2;
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( ObjectWrapperVector )
{

  int i,j;
  std::vector<double> d1(32);
  std::vector<double> d2(24);
  std::vector<int> map(4);

  for(i=0; i<32; i++) d1[i]=32.+(double)i;
  for(i=0; i<24; i++) d2[i]=64.+(double)i;
  for(i=0; i<4; i++) map[i]=2+i;

  PEObjectWrapperVector<double>::Ptr w1=allocate_component_type< PEObjectWrapperVector<double> >("Vector1");
  PEObjectWrapperVector<double>::Ptr w2=allocate_component_type< PEObjectWrapperVector<double> >("Vector2");

  w1->setup(d1,2,true);
  w2->setup(d2,3,false);

  BOOST_CHECK_EQUAL( w1->needs_update() , true );
  BOOST_CHECK_EQUAL( w2->needs_update() , false );

  BOOST_CHECK_EQUAL( w1->is_data_type_Uint() , false );
  BOOST_CHECK_EQUAL( w2->is_data_type_Uint() , false );

  BOOST_CHECK_EQUAL( w1->size() , 16 );
  BOOST_CHECK_EQUAL( w2->size() , 8 );

  BOOST_CHECK_EQUAL( w1->stride() , 2 );
  BOOST_CHECK_EQUAL( w2->stride() , 3 );

  BOOST_CHECK_EQUAL( w1->size_of() , sizeof(double) );
  BOOST_CHECK_EQUAL( w2->size_of() , sizeof(double) );

  double *dtest1=(double*)w1->pack(map);
  double *dtest2=(double*)w2->pack(map);

  for(i=0; i<8; i++) { BOOST_CHECK_EQUAL( dtest1[i] , 32+4+i ); dtest1[i]*=-1.; }
  for(i=0; i<12; i++) { BOOST_CHECK_EQUAL( dtest2[i] , 64+6+i ); dtest2[i]*=-1.; }

  w1->unpack(map,dtest1);
  w2->unpack(map,dtest2);

  double *dtesttest1=(double*)w1->pack(map);
  double *dtesttest2=(double*)w2->pack(map);

  for(i=0; i<8; i++) { BOOST_CHECK_EQUAL( dtesttest1[i] , -32-4-i ); }
  for(i=0; i<12; i++) { BOOST_CHECK_EQUAL( dtesttest2[i] , -64-6-i ); }

  delete[] dtest1;
  delete[] dtest2;
  delete[] dtesttest1;
  delete[] dtesttest2;
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( ObjectWrapperVectorWeakPtr )
{

  int i,j;
  boost::shared_ptr< std::vector<double> > d1( new std::vector<double>(32) );
  boost::shared_ptr< std::vector<double> > d2( new std::vector<double>(24) );
  std::vector<int> map(5);

  for(i=0; i<32; i++) (*d1)[i]=32.+(double)i;
  for(i=0; i<24; i++) (*d2)[i]=64.+(double)i;
  for(i=0; i<4; i++) map[i]=2+i;

  PEObjectWrapperVectorWeakPtr<double>::Ptr w1=allocate_component_type< PEObjectWrapperVectorWeakPtr<double> >("VectorWeakPtr1");
  PEObjectWrapperVectorWeakPtr<double>::Ptr w2=allocate_component_type< PEObjectWrapperVectorWeakPtr<double> >("VectorWeakPtr2");

  w1->setup(d1,2,true);
  w2->setup(d2,3,false);

  BOOST_CHECK_EQUAL( w1->needs_update() , true );
  BOOST_CHECK_EQUAL( w2->needs_update() , false );

  BOOST_CHECK_EQUAL( w1->is_data_type_Uint() , false );
  BOOST_CHECK_EQUAL( w2->is_data_type_Uint() , false );

  BOOST_CHECK_EQUAL( w1->size() , 16 );
  BOOST_CHECK_EQUAL( w2->size() , 8 );

  BOOST_CHECK_EQUAL( w1->stride() , 2 );
  BOOST_CHECK_EQUAL( w2->stride() , 3 );

  BOOST_CHECK_EQUAL( w1->size_of() , sizeof(double) );
  BOOST_CHECK_EQUAL( w2->size_of() , sizeof(double) );

  double *dtest1=(double*)w1->pack(map);
  double *dtest2=(double*)w2->pack(map);

  for(i=0; i<8; i++) { BOOST_CHECK_EQUAL( dtest1[i] , 32+4+i ); dtest1[i]*=-1.; }
  for(i=0; i<12; i++) { BOOST_CHECK_EQUAL( dtest2[i] , 64+6+i ); dtest2[i]*=-1.; }

  w1->unpack(map,dtest1);
  w2->unpack(map,dtest2);

  double *dtesttest1=(double*)w1->pack(map);
  double *dtesttest2=(double*)w2->pack(map);

  for(i=0; i<8; i++) { BOOST_CHECK_EQUAL( dtesttest1[i] , -32-4-i ); }
  for(i=0; i<12; i++) { BOOST_CHECK_EQUAL( dtesttest2[i] , -64-6-i ); }

  delete[] dtest1;
  delete[] dtest2;
  delete[] dtesttest1;
  delete[] dtesttest2;
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( data_registration_related )
{
  PECommPattern pecp("CommPattern2");
  BOOST_CHECK_EQUAL( pecp.isUpToDate() , false );

  boost::shared_ptr< std::vector<double> > d1( new std::vector<double>(32) );
  boost::shared_ptr< std::vector<double> > d2( new std::vector<double>(24) );

  // register data to PECommPattern
  pecp.insert<double>("VectorWeakPtr1",d1,2,true);
  pecp.insert<double>("VectorWeakPtr2",d2,3,true);

  // these are just dummies to see the selective iteration
  Component::Ptr dir1  ( new CGroup ( "dir1" ) );
  Component::Ptr dir2  ( new CGroup ( "dir2" ) );
  pecp.add_component( dir1 );
  pecp.add_component( dir2 );

  // count all child
  BOOST_CHECK_EQUAL( pecp.get_child_count() , 4 );

  // count recursively childs but only of type PEObjectWrapper
  //BOOST_CHECK_EQUAL( find_components_recursively<PEObjectWrapper>(pecp).size() , 2 );

  // iterate recursively childs but only of type PEObjectWrapper
  BOOST_FOREACH( PEObjectWrapper& pobj, find_components_recursively<PEObjectWrapper>(pecp) )
  {
    BOOST_CHECK_EQUAL( pobj.type_name() , "PEObjectWrapper" );
    BOOST_CHECK_EQUAL( pobj.size_of() , sizeof(double) );
    if (pobj.name()=="VectorWeakPtr1"){
      BOOST_CHECK_EQUAL( pobj.size() , 16 );
      BOOST_CHECK_EQUAL( pobj.stride() , 2 );
//      BOOST_CHECK_EQUAL( pobj.data() , &(*d1)[0] );
    }
    if (pobj.name()=="VectorWeakPtr2"){
      BOOST_CHECK_EQUAL( pobj.size() , 8 );
      BOOST_CHECK_EQUAL( pobj.stride() , 3 );
//      BOOST_CHECK_EQUAL( pobj.data() , &(*d2)[0] );
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( commpattern )
{
  // general conts in this routine
  const int nproc=PE::instance().size();
  const int irank=PE::instance().rank();

  // commpattern
  PECommPattern pecp("CommPattern2");
/*
  // stupid global-reverse global indices
  std::vector<Uint> gid(nproc);
  for (int i=0; i<gid.size(); i++) gid[i]=(nproc*nproc-1)-(irank*nproc+i);
  boost::shared_ptr< PEObjectWrapper > ow ( new PEObjectWrapperVector<Uint>("gid",data,size,stride,needs_update), Deleter< PEObjectWrapperPtr<T> >() );

  // rank is built such that total scatter
  std::vector<int> rank(nproc);
  for (int i=0; i<gid.size(); i++) rank[i]=i;

  // three additional arrays for testing
  std::vector<int> v1;
  for(int i=0;i<nproc;i++) v1.push_back(irank*10000+i+100);
  pecp.insert("v1",v1,1,true);
  std::vector<int> v2;
  for(int i=0;i<2*nproc;i++) v2.push_back(irank*10000+(i/2)*100+i%2);
  pecp.insert("v2",v2,2,true);
  std::vector<int> v3;
  for(int i=0;i<nproc;i++) v3.push_back(irank);
  pecp.insert("v3",v3,1,false);

  // initial setup
//  pecp.setup(,rank);
*/
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( finalize )
{
  PE::instance().finalize();
  BOOST_CHECK_EQUAL( PE::instance().is_init() , false );
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////

