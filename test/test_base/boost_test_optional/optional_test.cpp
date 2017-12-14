// Copyright (C) 2003, 2008 Fernando Luis Cacciola Carballal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/lib/optional for documentation.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
// Revisions:
// 12 May 2008 (added more swap tests)
//


// NOTE: taken from boost 1.55.0 and adapted for cppdevtk


#include "optional_test.hpp"

#include <cppdevtk/base/optional.hpp>
#include <cppdevtk/base/exception.hpp>

#include <algorithm>	// swap(), C++98
#include <utility>	// swap(), C++11


#include CPPDEVTK_TR1_HEADER(type_traits)


void test_implicit_construction ( Optional<double> opt, double v, double z )
{
  check_value(opt,v,z);
}

void test_implicit_construction ( Optional<X> opt, X const& v, X const& z )
{
  check_value(opt,v,z);
}

void test_default_implicit_construction ( double, Optional<double> opt )
{
  BOOST_TEST_OPTIONAL_CHECK(!opt);
}

void test_default_implicit_construction ( X const&, Optional<X> opt )
{
  BOOST_TEST_OPTIONAL_CHECK(!opt);
}

//
// Basic test.
// Check ordinary functionality:
//   Initialization, assignment, comparison and value-accessing.
//
template<class T>
void test_basics( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME  );

  T z(0);

  T a(1);

  // Default construction.
  // 'def' state is Uninitialized.
  // T::T() is not called (and it is not even defined)
  Optional<T> def ;
  check_uninitialized(def);

  // Implicit construction
  // The first parameter is implicitely converted to Optional<T>(a);
  test_implicit_construction(a,a,z);

  // Direct initialization.
  // 'oa' state is Initialized with 'a'
  // T::T( T const& x ) is used.
  set_pending_copy( ARG(T) ) ;
  Optional<T> oa ( a ) ;
  check_is_not_pending_copy( ARG(T) );
  check_initialized(oa);
  check_value(oa,a,z);

  T b(2);

  Optional<T> ob ;

  // Value-Assignment upon Uninitialized optional.
  // T::T( T const& x ) is used.
  set_pending_copy( ARG(T) ) ;
  ob = a ;
  check_is_not_pending_copy( ARG(T) ) ;
  check_initialized(ob);
  check_value(ob,a,z);

  // Value-Assignment upon Initialized optional.
  // T::operator=( T const& x ) is used
  set_pending_assign( ARG(T) ) ;
  set_pending_copy  ( ARG(T) ) ;
  set_pending_dtor  ( ARG(T) ) ;
  ob = b ;
  check_is_not_pending_assign( ARG(T) ) ;
  check_is_pending_copy      ( ARG(T) ) ;
  check_is_pending_dtor      ( ARG(T) ) ;
  check_initialized(ob);
  check_value(ob,b,z);

  // Assignment initialization.
  // T::T ( T const& x ) is used to copy new value.
  set_pending_copy( ARG(T) ) ;
  Optional<T> const oa2 ( oa ) ;
  check_is_not_pending_copy( ARG(T) ) ;
  check_initialized_const(oa2);
  check_value_const(oa2,a,z);

  // Assignment
  // T::operator= ( T const& x ) is used to copy new value.
  set_pending_assign( ARG(T) ) ;
  oa = ob ;
  check_is_not_pending_assign( ARG(T) ) ;
  check_initialized(oa);
  check_value(oa,b,z);

  // Uninitializing Assignment upon Initialized Optional
  // T::~T() is used to destroy previous value in oa.
  set_pending_dtor( ARG(T) ) ;
  set_pending_copy( ARG(T) ) ;
  oa = def ;
  check_is_not_pending_dtor( ARG(T) ) ;
  check_is_pending_copy    ( ARG(T) ) ;
  check_uninitialized(oa);

  // Uninitializing Assignment upon Uninitialized Optional
  // (Dtor is not called this time)
  set_pending_dtor( ARG(T) ) ;
  set_pending_copy( ARG(T) ) ;
  oa = def ;
  check_is_pending_dtor( ARG(T) ) ;
  check_is_pending_copy( ARG(T) ) ;
  check_uninitialized(oa);

  // Deinitialization of Initialized Optional
  // T::~T() is used to destroy previous value in ob.
  set_pending_dtor( ARG(T) ) ;
  ob.Reset();
  check_is_not_pending_dtor( ARG(T) ) ;
  check_uninitialized(ob);

  // Deinitialization of Uninitialized Optional
  // (Dtor is not called this time)
  set_pending_dtor( ARG(T) ) ;
  ob.Reset();
  check_is_pending_dtor( ARG(T) ) ;
  check_uninitialized(ob);

}

template<class T>
void test_conditional_ctor_and_get_valur_or ( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME  );

  T a(321);

  T z(123);

  Optional<T> const cdef0(false,a);

  Optional<T> def0(false,a);
  Optional<T> def1 = ::cppdevtk::base::MakeOptional(false,a); //  T is not within boost so ADL won't find make_optional unqualified
  check_uninitialized(def0);
  check_uninitialized(def1);

  Optional<T> const co0(true,a);

  Optional<T> o0(true,a);
  Optional<T> o1 = ::cppdevtk::base::MakeOptional(true,a); //  T is not within boost so ADL won't find make_optional unqualified

  check_initialized(o0);
  check_initialized(o1);
  check_value(o0,a,z);
  check_value(o1,a,z);

  T b = def0.GetValueOr(z);
  BOOST_TEST_OPTIONAL_CHECK( b == z ) ;

  b = o0.GetValueOr(z);
  BOOST_TEST_OPTIONAL_CHECK( b == a ) ;


  T const& crz = z ;
  T&        rz = z ;

  T const& crzz = def0.GetValueOr(crz);
  BOOST_TEST_OPTIONAL_CHECK( crzz == crz ) ;

  T& rzz = def0.GetValueOr(rz);
  BOOST_TEST_OPTIONAL_CHECK( rzz == rz ) ;

  T const& crb = o0.GetValueOr(crz);
  BOOST_TEST_OPTIONAL_CHECK( crb == a ) ;

  T& rb = o0.GetValueOr(rz);
  BOOST_TEST_OPTIONAL_CHECK( rb == b ) ;
  
  // our Optional does not support references
  /*
  T& ra = a ;
  
  Optional<T&> defref(false,ra);
  BOOST_TEST_OPTIONAL_CHECK(!defref);

  Optional<T&> ref(true,ra);
  BOOST_TEST_OPTIONAL_CHECK(!!ref);

  a = T(432);

  BOOST_TEST_OPTIONAL_CHECK( *ref == a ) ;
  
  T& r1 = defref.GetValueOr(z);
  BOOST_TEST_OPTIONAL_CHECK( r1 == z ) ;

  T& r2 = ref.GetValueOr(z);
  BOOST_TEST_OPTIONAL_CHECK( r2 == a ) ;
  */
}

//
// Test Direct Value Manipulation
//
template<class T>
void test_direct_value_manip( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T x(3);

  Optional<T> const c_opt0(x) ;
  Optional<T>         opt0(x);

  BOOST_TEST_OPTIONAL_CHECK( c_opt0.GetValue().V() == x.V() ) ;
  BOOST_TEST_OPTIONAL_CHECK(   opt0.GetValue().V() == x.V() ) ;

  BOOST_TEST_OPTIONAL_CHECK( c_opt0->V() == x.V() ) ;
  BOOST_TEST_OPTIONAL_CHECK(   opt0->V() == x.V() ) ;

  BOOST_TEST_OPTIONAL_CHECK( (*c_opt0).V() == x.V() ) ;
  BOOST_TEST_OPTIONAL_CHECK( (*  opt0).V() == x.V() ) ;

  T y(4);
  opt0 = y ;
  //BOOST_TEST_OPTIONAL_CHECK( get(opt0).V() == y.V() ) ;
  BOOST_TEST_OPTIONAL_CHECK(opt0.GetValue().V() == y.V() ) ;
}

//
// Test Uninitialized access assert
//
template<class T>
void test_uninitialized_access( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  Optional<T> def ;

  bool passed = false ;
  try
  {
    // This should throw because 'def' is uninitialized
    T const& n = def.GetValue() ;
    unused_variable(n);
    passed = true ;
  }
  catch (...) {}
  BOOST_TEST_OPTIONAL_CHECK(!passed);

  passed = false ;
  try
  {
    // This should throw because 'def' is uninitialized
    T const& n = *def ;
    unused_variable(n);
    passed = true ;
  }
  catch (...) {}
  BOOST_TEST_OPTIONAL_CHECK(!passed);

  passed = false ;
  try
  {
    T v(5) ;
    unused_variable(v);
    // This should throw because 'def' is uninitialized
    *def = v ;
    passed = true ;
  }
  catch (...) {}
  BOOST_TEST_OPTIONAL_CHECK(!passed);

  passed = false ;
  try
  {
    // This should throw because 'def' is uninitialized
    T v = *(def.operator->()) ;
    unused_variable(v);
    passed = true ;
  }
  catch (...) {}
  BOOST_TEST_OPTIONAL_CHECK(!passed);
}

#if (CPPDEVTK_TEST_OPTIONAL_VERIFY_LIFETIME)

//
// Test Direct Initialization of optional for a T with throwing copy-ctor.
//
template<class T>
void test_throwing_direct_init( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T a(6);

  int count = get_instance_count( ARG(T) ) ;

  set_throw_on_copy( ARG(T) ) ;

  bool passed = false ;
  try
  {
    // This should:
    //   Attempt to copy construct 'a' and throw.
    // 'opt' won't be constructed.
    set_pending_copy( ARG(T) ) ;

    Optional<T> opt(a) ;
    passed = true ;
  }
  catch ( ... ){}

  BOOST_TEST_OPTIONAL_CHECK(!passed);
  check_is_not_pending_copy( ARG(T) );
  check_instance_count(count, ARG(T) );

  reset_throw_on_copy( ARG(T) ) ;

}

//
// Test Value Assignment to an Uninitialized optional for a T with a throwing copy-ctor
//
template<class T>
void test_throwing_val_assign_on_uninitialized( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T a(7);

  int count = get_instance_count( ARG(T) ) ;

  set_throw_on_copy( ARG(T) ) ;

  Optional<T> opt ;

  bool passed = false ;
  try
  {
    // This should:
    //   Attempt to copy construct 'a' and throw.
    //   opt should be left uninitialized.
    set_pending_copy( ARG(T) ) ;
    opt.Reset( a );
    passed = true ;
  }
  catch ( ... ) {}

  BOOST_TEST_OPTIONAL_CHECK(!passed);

  check_is_not_pending_copy( ARG(T) );
  check_instance_count(count, ARG(T) );
  check_uninitialized(opt);

  reset_throw_on_copy( ARG(T) ) ;
}

//
// Test Value Reset on an Initialized optional for a T with a throwing copy-ctor
//
template<class T>
void test_throwing_val_assign_on_initialized( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T z(0);
  T a(8);
  T b(9);
  T x(-1);

  int count = get_instance_count( ARG(T) ) ;

  Optional<T> opt ( b ) ;
  ++ count ;

  check_instance_count(count, ARG(T) );

  check_value(opt,b,z);

  set_throw_on_assign( ARG(T) ) ;

  bool passed = false ;
  try
  {
    // This should:
    //   Attempt to assign 'a' and throw.
    //   opt is kept initialized but its value not neccesarily fully assigned
    //   (in this test, incompletely assigned is flaged with the value -1 being set)
    set_pending_assign( ARG(T) ) ;
    opt.Reset(a) ;
    passed = true ;
  }
  catch ( ... ) {}

  BOOST_TEST_OPTIONAL_CHECK(!passed);

  check_is_not_pending_assign( ARG(T) );
  check_instance_count(count, ARG(T) );
  check_initialized(opt);
  check_value(opt,x,z);

  reset_throw_on_assign ( ARG(T) ) ;
}

//
// Test Copy Initialization from an Initialized optional for a T with a throwing copy-ctor
//
template<class T>
void test_throwing_copy_initialization( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T z(0);
  T a(10);

  Optional<T> opt (a);

  int count = get_instance_count( ARG(T) ) ;

  set_throw_on_copy( ARG(T) ) ;

  bool passed = false ;
  try
  {
    // This should:
    //   Attempt to copy construct 'opt' and throw.
    //   opt1 won't be constructed.
    set_pending_copy( ARG(T) ) ;
    Optional<T> opt1 = opt ;
    passed = true ;
  }
  catch ( ... ) {}

  BOOST_TEST_OPTIONAL_CHECK(!passed);

  check_is_not_pending_copy( ARG(T) );
  check_instance_count(count, ARG(T) );

  // Nothing should have happened to the source optional.
  check_initialized(opt);
  check_value(opt,a,z);

  reset_throw_on_copy( ARG(T) ) ;
}

//
// Test Assignment to an Uninitialized optional from an Initialized optional
// for a T with a throwing copy-ctor
//
template<class T>
void test_throwing_assign_to_uninitialized( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T z(0);
  T a(11);

  Optional<T> opt0 ;
  Optional<T> opt1(a) ;

  int count = get_instance_count( ARG(T) ) ;

  set_throw_on_copy( ARG(T) ) ;

  bool passed = false ;
  try
  {
    // This should:
    //   Attempt to copy construct 'opt1.value()' into opt0 and throw.
    //   opt0 should be left uninitialized.
    set_pending_copy( ARG(T) ) ;
    opt0 = opt1 ;
    passed = true ;
  }
  catch ( ... ) {}

  BOOST_TEST_OPTIONAL_CHECK(!passed);

  check_is_not_pending_copy( ARG(T) );
  check_instance_count(count, ARG(T) );
  check_uninitialized(opt0);

  reset_throw_on_copy( ARG(T) ) ;
}

//
// Test Assignment to an Initialized optional from an Initialized optional
// for a T with a throwing copy-ctor
//
template<class T>
void test_throwing_assign_to_initialized( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T z(0);
  T a(12);
  T b(13);
  T x(-1);

  Optional<T> opt0(a) ;
  Optional<T> opt1(b) ;

  int count = get_instance_count( ARG(T) ) ;

  set_throw_on_assign( ARG(T) ) ;

  bool passed = false ;
  try
  {
    // This should:
    //   Attempt to copy construct 'opt1.value()' into opt0 and throw.
    //   opt0 is kept initialized but its value not neccesarily fully assigned
    //   (in this test, incompletely assigned is flaged with the value -1 being set)
    set_pending_assign( ARG(T) ) ;
    opt0 = opt1 ;
    passed = true ;
  }
  catch ( ... ) {}

  BOOST_TEST_OPTIONAL_CHECK(!passed);

  // opt0 was left uninitialized
  check_is_not_pending_assign( ARG(T) );
  check_instance_count(count, ARG(T) );
  check_initialized(opt0);
  check_value(opt0,x,z);

  reset_throw_on_assign( ARG(T) ) ;
}

//
// Test swap in a no-throwing case
//
template<class T>
void test_no_throwing_swap( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T z(0);
  T a(14);
  T b(15);

  Optional<T> def0 ;
  Optional<T> def1 ;
  Optional<T> opt0(a) ;
  Optional<T> opt1(b) ;

  int count = get_instance_count( ARG(T) ) ;

  swap(def0,def1);
  check_uninitialized(def0);
  check_uninitialized(def1);

  swap(def0,opt0);
  check_uninitialized(opt0);
  check_initialized(def0);
  check_value(def0,a,z);

  // restore def0 and opt0
  swap(def0,opt0);

  swap(opt0,opt1);
  check_instance_count(count, ARG(T) );
  check_initialized(opt0);
  check_initialized(opt1);
  check_value(opt0,b,z);
  check_value(opt1,a,z);
}

//
// Test swap in a throwing case
//
template<class T>
void test_throwing_swap( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T a(16);
  T b(17);
  T x(-1);

  Optional<T> opt0(a) ;
  Optional<T> opt1(b) ;

  set_throw_on_assign( ARG(T) ) ;

  //
  // Case 1: Both Initialized.
  //
  bool passed = false ;
  try
  {
    // This should attempt to swap optionals and fail at swap(X&,X&).
    swap(opt0,opt1);

    passed = true ;
  }
  catch ( ... ) {}

  BOOST_TEST_OPTIONAL_CHECK(!passed);

  // optional's swap doesn't affect the initialized states of the arguments. Therefore,
  // the following must hold:
  check_initialized(opt0);
  check_initialized(opt1);
  check_value(opt0,x,a);
  check_value(opt1,b,x);


  //
  // Case 2: Only one Initialized.
  //
  reset_throw_on_assign( ARG(T) ) ;

  opt0.Reset();
  opt1.Reset(a);

  set_throw_on_copy( ARG(T) ) ;

  passed = false ;
  try
  {
    // This should attempt to swap optionals and fail at opt0.Reset(*opt1)
    // Both opt0 and op1 are left unchanged (unswaped)
    swap(opt0,opt1);

    passed = true ;
  }
  catch ( ... ) {}

  BOOST_TEST_OPTIONAL_CHECK(!passed);

  check_uninitialized(opt0);
  check_initialized(opt1);
  check_value(opt1,a,x);

  reset_throw_on_copy( ARG(T) ) ;
}

#endif

//
// This verifies relational operators.
//
template<class T>
void test_relops( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T v0(0);
  T v1(1);
  T v2(1);

  Optional<T> def0 ;
  Optional<T> def1 ;
  Optional<T> opt0(v0);
  Optional<T> opt1(v1);
  Optional<T> opt2(v2);

  // Check identity
  BOOST_TEST_OPTIONAL_CHECK ( def0 == def0 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt0 == opt0 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(def0 != def0) ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(opt0 != opt0) ) ;

  // Check when both are uininitalized.
  BOOST_TEST_OPTIONAL_CHECK (   def0 == def1  ) ; // both uninitialized compare equal
  BOOST_TEST_OPTIONAL_CHECK ( !(def0 <  def1) ) ; // uninitialized is never less    than uninitialized
  BOOST_TEST_OPTIONAL_CHECK ( !(def0 >  def1) ) ; // uninitialized is never greater than uninitialized
  BOOST_TEST_OPTIONAL_CHECK ( !(def0 != def1) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   def0 <= def1  ) ;
  BOOST_TEST_OPTIONAL_CHECK (   def0 >= def1  ) ;

  // Check when only lhs is uninitialized.
  BOOST_TEST_OPTIONAL_CHECK (   def0 != opt0  ) ; // uninitialized is never equal to initialized
  BOOST_TEST_OPTIONAL_CHECK ( !(def0 == opt0) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   def0 <  opt0  ) ; // uninitialized is always less than initialized
  BOOST_TEST_OPTIONAL_CHECK ( !(def0 >  opt0) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   def0 <= opt0  ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(def0 >= opt0) ) ;

  // Check when only rhs is uninitialized.
  BOOST_TEST_OPTIONAL_CHECK (   opt0 != def0  ) ; // initialized is never equal to uninitialized
  BOOST_TEST_OPTIONAL_CHECK ( !(opt0 == def0) ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(opt0 <  def0) ) ; // initialized is never less than uninitialized
  BOOST_TEST_OPTIONAL_CHECK (   opt0 >  def0  ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(opt0 <= def0) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   opt0 >= opt0  ) ;

  // If both are initialized, values are compared
  BOOST_TEST_OPTIONAL_CHECK ( opt0 != opt1 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt1 == opt2 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt0 <  opt1 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt1 >  opt0 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt1 <= opt2 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt1 >= opt0 ) ;

  // Compare against a value directly
  BOOST_TEST_OPTIONAL_CHECK ( opt0 == v0 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt0 != v1 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt1 == v2 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt0 <  v1 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt1 >  v0 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt1 <= v2 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt1 >= v0 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( v0 != opt1 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( v1 == opt2 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( v0 <  opt1 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( v1 >  opt0 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( v1 <= opt2 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( v1 >= opt0 ) ;
  BOOST_TEST_OPTIONAL_CHECK (   def0 != v0  ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(def0 == v0) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   def0 <  v0  ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(def0 >  v0) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   def0 <= v0  ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(def0 >= v0) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   v0 != def0  ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(v0 == def0) ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(v0 <  def0) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   v0 >  def0  ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(v0 <= def0) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   v0 >= opt0  ) ;
}

template<class T>
void test_none( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  using ::cppdevtk::base::kNullOpt ;

  Optional<T> def0 ;
  Optional<T> def1(kNullOpt) ;
  Optional<T> non_def( T(1234) ) ;

  BOOST_TEST_OPTIONAL_CHECK ( def0    == kNullOpt ) ;
  BOOST_TEST_OPTIONAL_CHECK ( non_def != kNullOpt ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !def1           ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(non_def <  kNullOpt) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   non_def >  kNullOpt  ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !(non_def <= kNullOpt) ) ;
  BOOST_TEST_OPTIONAL_CHECK (   non_def >= kNullOpt  ) ;

  non_def = kNullOpt ;
  BOOST_TEST_OPTIONAL_CHECK ( !non_def ) ;

  test_default_implicit_construction(T(1),kNullOpt);
}

template<class T>
void test_arrow( T const* )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  T a(1234);

  Optional<T>        oa(a) ;
  Optional<T> const coa(a) ;

  BOOST_TEST_OPTIONAL_CHECK ( coa->V() == 1234 ) ;

  oa->V() = 4321 ;

  BOOST_TEST_OPTIONAL_CHECK (     a.V() = 1234 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( (*oa).V() = 4321 ) ;
}

void test_with_builtin_types()
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  test_basics( ARG(double) );
  test_conditional_ctor_and_get_valur_or( ARG(double) );
  test_uninitialized_access( ARG(double) );
#if (CPPDEVTK_TEST_OPTIONAL_VERIFY_LIFETIME)
  test_no_throwing_swap( ARG(double) );
#endif
  test_relops( ARG(double) ) ;
  test_none( ARG(double) ) ;
}

void test_with_class_type()
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  test_basics( ARG(X) );
  test_conditional_ctor_and_get_valur_or( ARG(X) );
  test_direct_value_manip( ARG(X) );
  test_uninitialized_access( ARG(X) );
#if (CPPDEVTK_TEST_OPTIONAL_VERIFY_LIFETIME)
  test_throwing_direct_init( ARG(X) );
  test_throwing_val_assign_on_uninitialized( ARG(X) );
  test_throwing_val_assign_on_initialized( ARG(X) );
  test_throwing_copy_initialization( ARG(X) );
  test_throwing_assign_to_uninitialized( ARG(X) );
  test_throwing_assign_to_initialized( ARG(X) );
  test_no_throwing_swap( ARG(X) );
  test_throwing_swap( ARG(X) );
#endif
  test_relops( ARG(X) ) ;
  test_none( ARG(X) ) ;
  test_arrow( ARG(X) ) ;
  BOOST_TEST_OPTIONAL_CHECK ( X::count == 0 ) ;
}

int eat ( bool ) { return 1 ; }
int eat ( char ) { return 1 ; }
int eat ( int  ) { return 1 ; }
int eat ( void const* ) { return 1 ; }

template<class T> int eat ( T ) { return 0 ; }

//
// This verifies that operator safe_bool() behaves properly.
//
template<class T>
void test_no_implicit_conversions_impl( T const& )
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  Optional<T> def ;
  BOOST_TEST_OPTIONAL_CHECK ( eat(def) == 0 ) ;
}

void test_no_implicit_conversions()
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME   );

  bool b = false ;
  char c = 0 ;
  int i = 0 ;
  void const* p = 0 ;

  test_no_implicit_conversions_impl(b);
  test_no_implicit_conversions_impl(c);
  test_no_implicit_conversions_impl(i);
  test_no_implicit_conversions_impl(p);
}

struct A {} ;
void test_conversions1()
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME );

  char c = 20 ;
  Optional<char> opt0(c);
  Optional<int> opt1(opt0);
  BOOST_TEST_OPTIONAL_CHECK(*opt1 == static_cast<int>(c));

  float f = 21.22f ;
  double d = f ;
  Optional<float> opt2(f) ;
  Optional<double> opt3 ;
  opt3 = opt2 ;
  BOOST_TEST_OPTIONAL_CHECK(*opt3 == d);
}

void test_conversions2()
{
  CPPDEVTK_TEST_OPTIONAL_TRACE( std::endl << CPPDEVTK_FUNCTION_LONG_NAME );

  char c = 20 ;
  Optional<int> opt(c);
  BOOST_TEST_OPTIONAL_CHECK( opt.GetValue() == static_cast<int>(c));

  float f = 21.22f ;
  Optional<double> opt1;
  opt1 = f ;
  BOOST_TEST_OPTIONAL_CHECK(*opt1 == static_cast<double>(f));
}


//
// Tests whether the swap function works properly for Optional<T>.
// Assumes that T has one data member, of type char.
// Returns true iff the test is passed.
//
template <class T>
bool test_swap_function( T const* )
{
  const int counter_before_test = boost::optional_test_errors_counter;
  try
  {
    Optional<T> obj1;
    Optional<T> obj2('a');

    // Self-swap should not have any effect.
    swap(obj1, obj1);
    swap(obj2, obj2);
    BOOST_TEST_OPTIONAL_CHECK(!obj1);
    BOOST_TEST_OPTIONAL_CHECK(!!obj2 && obj2->data == 'a');

    // Call non-member swap.
    swap(obj1, obj2);

    // Test if obj1 and obj2 are really swapped.
    BOOST_TEST_OPTIONAL_CHECK(!!obj1 && obj1->data == 'a');
    BOOST_TEST_OPTIONAL_CHECK(!obj2);

    // Call non-member swap one more time.
    swap(obj1, obj2);

    // Test if obj1 and obj2 are swapped back.
    BOOST_TEST_OPTIONAL_CHECK(!obj1);
    BOOST_TEST_OPTIONAL_CHECK(!!obj2 && obj2->data == 'a');
  }
  catch(const std::exception &)
  {
    // The swap function should not throw, for our test cases.
    return false ;
  }
  return boost::optional_test_errors_counter == counter_before_test ;
}

//
// Tests whether the Optional<T>::swap member function works properly.
// Assumes that T has one data member, of type char.
// Returns true iff the test is passed.
//
template <class T>
bool test_swap_member_function( T const* )
{
  const int counter_before_test = boost::optional_test_errors_counter;
  try
  {
    Optional<T> obj1;
    Optional<T> obj2('a');

    // Self-swap should not have any effect.
    obj1.swap(obj1);
    obj2.swap(obj2);
    BOOST_TEST_OPTIONAL_CHECK(!obj1);
    BOOST_TEST_OPTIONAL_CHECK(!!obj2 && obj2->data == 'a');

    // Call member swap.
    obj1.swap(obj2);

    // Test if obj1 and obj2 are really swapped.
    BOOST_TEST_OPTIONAL_CHECK(!!obj1 && obj1->data == 'a');
    BOOST_TEST_OPTIONAL_CHECK(!obj2);

    // Call member swap one more time.
    obj1.swap(obj2);

    // Test if obj1 and obj2 are swapped back.
    BOOST_TEST_OPTIONAL_CHECK(!obj1);
    BOOST_TEST_OPTIONAL_CHECK(!!obj2 && obj2->data == 'a');
  }
  catch(const std::exception &)
  {
    // The Optional<T>::swap member function should not throw, for our test cases.
    return false ;
  }
 return boost::optional_test_errors_counter == counter_before_test ;
}


//
// Tests compile time tweaking of swap, by means of
// optional_swap_should_use_default_constructor.
//
/*
void test_swap_tweaking()
{
  BOOST_TEST_OPTIONAL_CHECK( test_swap_function( ARG(optional_swap_test::class_without_default_ctor) ) );
  BOOST_TEST_OPTIONAL_CHECK( test_swap_function( ARG(optional_swap_test::class_whose_default_ctor_should_be_used) ) );
  BOOST_TEST_OPTIONAL_CHECK( test_swap_function( ARG(optional_swap_test::class_whose_default_ctor_should_not_be_used) ) );
  BOOST_TEST_OPTIONAL_CHECK( test_swap_function( ARG(optional_swap_test::class_whose_explicit_ctor_should_be_used) ) );
  BOOST_TEST_OPTIONAL_CHECK( test_swap_function( ARG(optional_swap_test::template_whose_default_ctor_should_be_used<char>) ) );
  BOOST_TEST_OPTIONAL_CHECK( test_swap_member_function( ARG(optional_swap_test::class_without_default_ctor) ) );
  BOOST_TEST_OPTIONAL_CHECK( test_swap_member_function( ARG(optional_swap_test::class_whose_default_ctor_should_be_used) ) );
  BOOST_TEST_OPTIONAL_CHECK( test_swap_member_function( ARG(optional_swap_test::class_whose_default_ctor_should_not_be_used) ) );
  BOOST_TEST_OPTIONAL_CHECK( test_swap_member_function( ARG(optional_swap_test::class_whose_explicit_ctor_should_be_used) ) );
  BOOST_TEST_OPTIONAL_CHECK( test_swap_member_function( ARG(optional_swap_test::template_whose_default_ctor_should_be_used<char>) ) );
}
*/

// Test for support for classes with overridden operator&
class CustomAddressOfClass
{
    int n;

public:
    CustomAddressOfClass() : n(0) {}
    CustomAddressOfClass(CustomAddressOfClass const& that) : n(that.n) {}
    explicit CustomAddressOfClass(int m) : n(m) {}
    int* operator& () { return &n; }
    bool operator== (CustomAddressOfClass const& that) const { return n == that.n; }
};

void test_custom_addressof_operator()
{
    cppdevtk::base::Optional< CustomAddressOfClass > o1(CustomAddressOfClass(10));
    BOOST_TEST_OPTIONAL_CHECK(!!o1);
    BOOST_TEST_OPTIONAL_CHECK(o1.GetValue() == CustomAddressOfClass(10));

    o1 = CustomAddressOfClass(20);
    BOOST_TEST_OPTIONAL_CHECK(!!o1);
    BOOST_TEST_OPTIONAL_CHECK(o1.GetValue() == CustomAddressOfClass(20));

    o1 = ::cppdevtk::base::kNullOpt;
    BOOST_TEST_OPTIONAL_CHECK(!o1);
}

bool TestOptional() {
	try {
		test_with_class_type();
		test_with_builtin_types();
		test_no_implicit_conversions();
		test_conversions1();
		test_conversions2();
		//test_swap_tweaking();
		test_custom_addressof_operator();
	}
	catch (const ::std::exception& exc) {
		::std::cerr << "OptionalTest(): caught exception: " << exc.what() << ::std::endl;
		return false;
	}
	catch (...) {
		::std::cerr << "OptionalTest(): caught unknown exception" << ::std::endl;
		return false;
	}
	
	return boost::optional_test_errors_counter == 0;
}
