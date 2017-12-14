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


#ifndef CPPDEVTK_OPTIONAL_TEST_COMMON_HPP_INCLUDED_
#define CPPDEVTK_OPTIONAL_TEST_COMMON_HPP_INCLUDED_


#include <cppdevtk/config/config.hpp>
#include <cppdevtk/base/optional.hpp>

#include <iostream>


#define CPPDEVTK_TEST_OPTIONAL_VERIFY_LIFETIME 0	// FIXME
#define CPPDEVTK_TEST_OPTIONAL_ENABLE_TRACE 0


#if (CPPDEVTK_TEST_OPTIONAL_ENABLE_TRACE)
#define CPPDEVTK_TEST_OPTIONAL_TRACE(msg) std::cout << msg << std::endl ;
#else
#define CPPDEVTK_TEST_OPTIONAL_TRACE(msg)
#endif


#define BOOST_TEST_OPTIONAL_CHECK(exp)       \
  ( (exp)                      \
      ? static_cast<void>(0)   \
      : boost::ReportError(#exp,__FILE__,__LINE__, CPPDEVTK_FUNCTION_LONG_NAME) )


namespace boost {


extern int optional_test_errors_counter;


void ReportError(const char* msg, const char* file, int line, const char* func_name, bool is_msg = false);


}

using ::cppdevtk::base::Optional;


template<class T> inline void unused_variable ( T ) {}


#define ARG(T) (static_cast< T const* >(0))


//
// Helper class used to verify the lifetime managment of the values held by optional
//
class X
{
  public :

    X ( int av ) : v(av)
    {
      ++ count ;

      CPPDEVTK_TEST_OPTIONAL_TRACE ( "X::X(" << av << "). this=" << this ) ;
    }

    X ( X const& rhs ) : v(rhs.v)
    {
       pending_copy = false ;

       CPPDEVTK_TEST_OPTIONAL_TRACE ( "X::X( X const& rhs). this=" << this << " rhs.v=" << rhs.v ) ;

       if ( throw_on_copy )
       {
         CPPDEVTK_TEST_OPTIONAL_TRACE ( "throwing exception in X's copy ctor" ) ;
         throw 0 ;
       }

       ++ count ;
    }

    ~X()
    {
      pending_dtor = false ;

      -- count ;

      CPPDEVTK_TEST_OPTIONAL_TRACE ( "X::~X(). v=" << v << "  this=" << this );
    }

    X& operator= ( X const& rhs )
      {
        pending_assign = false ;

        if ( throw_on_assign )
        {
          CPPDEVTK_TEST_OPTIONAL_TRACE ( "throwing exception in X's assignment" ) ;

          v = -1 ;

          throw 0 ;
        }
        else
        {
          v = rhs.v ;

          CPPDEVTK_TEST_OPTIONAL_TRACE ( "X::operator =( X const& rhs). this=" << this << " rhs.v=" << rhs.v ) ;
        }
        return *this ;
      }

    friend bool operator == ( X const& a, X const& b )
      { return a.v == b.v ; }

    friend bool operator != ( X const& a, X const& b )
      { return a.v != b.v ; }

    friend bool operator < ( X const& a, X const& b )
      { return a.v < b.v ; }

    int  V() const { return v ; }
    int& V()       { return v ; }

    static int  count ;
    static bool pending_copy   ;
    static bool pending_dtor   ;
    static bool pending_assign ;
    static bool throw_on_copy ;
    static bool throw_on_assign ;

  private :

    int  v ;

  private :

    X() ;
} ;


#if (CPPDEVTK_TEST_OPTIONAL_VERIFY_LIFETIME)

inline void set_pending_copy           ( X const* x ) { unused_variable(x); X::pending_copy  = true  ; }
inline void set_pending_dtor           ( X const* x ) { unused_variable(x); X::pending_dtor  = true  ; }
inline void set_pending_assign         ( X const* x ) { unused_variable(x); X::pending_assign = true  ; }
inline void set_throw_on_copy          ( X const* x ) { unused_variable(x); X::throw_on_copy = true  ; }
inline void set_throw_on_assign        ( X const* x ) { unused_variable(x); X::throw_on_assign = true  ; }
inline void reset_throw_on_copy        ( X const* x ) { unused_variable(x); X::throw_on_copy = false ; }
inline void reset_throw_on_assign      ( X const* x ) { unused_variable(x); X::throw_on_assign = false ; }
inline void check_is_pending_copy      ( X const* x ) { unused_variable(x); BOOST_TEST_OPTIONAL_CHECK( X::pending_copy ) ; }
inline void check_is_pending_dtor      ( X const* x ) { unused_variable(x); BOOST_TEST_OPTIONAL_CHECK( X::pending_dtor ) ; }
inline void check_is_pending_assign    ( X const* x ) { unused_variable(x); BOOST_TEST_OPTIONAL_CHECK( X::pending_assign ) ; }
inline void check_is_not_pending_copy  ( X const* x ) { unused_variable(x); BOOST_TEST_OPTIONAL_CHECK( !X::pending_copy ) ; }
inline void check_is_not_pending_dtor  ( X const* x ) { unused_variable(x); BOOST_TEST_OPTIONAL_CHECK( !X::pending_dtor ) ; }
inline void check_is_not_pending_assign( X const* x ) { unused_variable(x); BOOST_TEST_OPTIONAL_CHECK( !X::pending_assign ) ; }
inline void check_instance_count       ( int c, X const* x ) { unused_variable(x); BOOST_TEST_OPTIONAL_CHECK( X::count == c ) ; }
inline int  get_instance_count         ( X const* x ) { unused_variable(x); return X::count ; }

#else

inline void set_pending_copy           ( X const* x ) { unused_variable(x);}
inline void set_pending_dtor           ( X const* x ) { unused_variable(x);}
inline void set_pending_assign         ( X const* x ) { unused_variable(x);}
inline void set_throw_on_copy          ( X const* x ) { unused_variable(x);}
inline void set_throw_on_assign        ( X const* x ) { unused_variable(x);}
inline void reset_throw_on_copy        ( X const* x ) { unused_variable(x);}
inline void reset_throw_on_assign      ( X const* x ) { unused_variable(x);}
inline void check_is_pending_copy      ( X const* x ) { unused_variable(x);}
inline void check_is_pending_dtor      ( X const* x ) { unused_variable(x);}
inline void check_is_pending_assign    ( X const* x ) { unused_variable(x);}
inline void check_is_not_pending_copy  ( X const* x ) { unused_variable(x);}
inline void check_is_not_pending_dtor  ( X const* x ) { unused_variable(x);}
inline void check_is_not_pending_assign( X const* x ) { unused_variable(x);}
inline void check_instance_count       ( int c, X const* x ) { unused_variable(c); unused_variable(x);}
inline int  get_instance_count         ( X const* x ) { unused_variable(x); return X::count ; }

#endif

inline void set_pending_copy           (...) {}
inline void set_pending_dtor           (...) {}
inline void set_pending_assign         (...) {}
inline void set_throw_on_copy          (...) {}
inline void set_throw_on_assign        (...) {}
inline void reset_throw_on_copy        (...) {}
inline void reset_throw_on_assign      (...) {}
inline void check_is_pending_copy      (...) {}
inline void check_is_pending_dtor      (...) {}
inline void check_is_pending_assign    (...) {}
inline void check_is_not_pending_copy  (...) {}
inline void check_is_not_pending_dtor  (...) {}
inline void check_is_not_pending_assign(...) {}
inline void check_instance_count       (...) {}
inline int  get_instance_count         (...) { return 0 ; }


template<class T>
inline void check_uninitialized_const ( Optional<T> const& opt )
{
  BOOST_TEST_OPTIONAL_CHECK( opt == 0 ) ;
  BOOST_TEST_OPTIONAL_CHECK( !opt ) ;
  //BOOST_TEST_OPTIONAL_CHECK( !get_pointer(opt) ) ;
  BOOST_TEST_OPTIONAL_CHECK( !opt.GetValuePtr() ) ;
}
template<class T>
inline void check_uninitialized ( Optional<T>& opt )
{
  BOOST_TEST_OPTIONAL_CHECK( opt == 0 ) ;
  BOOST_TEST_OPTIONAL_CHECK( !opt ) ;
  //BOOST_TEST_OPTIONAL_CHECK( !get_pointer(opt) ) ;
  BOOST_TEST_OPTIONAL_CHECK( !opt.GetValuePtr() ) ;

  check_uninitialized_const(opt);
}

template<class T>
inline void check_initialized_const ( Optional<T> const& opt )
{
  BOOST_TEST_OPTIONAL_CHECK( opt ) ;
  BOOST_TEST_OPTIONAL_CHECK( opt != 0 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !!opt ) ;
  //BOOST_TEST_OPTIONAL_CHECK ( get_pointer(opt) ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt.GetValuePtr() ) ;
}

template<class T>
inline void check_initialized ( Optional<T>& opt )
{
  BOOST_TEST_OPTIONAL_CHECK( opt ) ;
  BOOST_TEST_OPTIONAL_CHECK( opt != 0 ) ;
  BOOST_TEST_OPTIONAL_CHECK ( !!opt ) ;
  //BOOST_TEST_OPTIONAL_CHECK ( get_pointer(opt) ) ;
  BOOST_TEST_OPTIONAL_CHECK ( opt.GetValuePtr() ) ;

  check_initialized_const(opt);
}

template<class T>
inline void check_value_const ( Optional<T> const& opt, T const& v, T const& z )
{
  BOOST_TEST_OPTIONAL_CHECK( *opt == v ) ;
  BOOST_TEST_OPTIONAL_CHECK( *opt != z ) ;
  BOOST_TEST_OPTIONAL_CHECK( opt.GetValue() == v ) ;
  BOOST_TEST_OPTIONAL_CHECK( opt.GetValue() != z ) ;
  BOOST_TEST_OPTIONAL_CHECK( (*(opt.operator->()) == v) ) ;
  //BOOST_TEST_OPTIONAL_CHECK( *get_pointer(opt) == v ) ;
}

template<class T>
inline void check_value ( Optional<T>& opt, T const& v, T const& z )
{
  BOOST_TEST_OPTIONAL_CHECK( *opt == v ) ;
  BOOST_TEST_OPTIONAL_CHECK( *opt != z ) ;
  BOOST_TEST_OPTIONAL_CHECK( opt.GetValue() == v ) ;
  BOOST_TEST_OPTIONAL_CHECK( opt.GetValue() != z ) ;
  BOOST_TEST_OPTIONAL_CHECK( (*(opt.operator->()) == v) ) ;
  //BOOST_TEST_OPTIONAL_CHECK( *get_pointer(opt) == v ) ;

  check_value_const(opt,v,z);
}



#endif
