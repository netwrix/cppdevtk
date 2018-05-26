//  boost::compressed_pair test program   
    
//  (C) Copyright John Maddock 2000. 
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).


// standalone test program for <boost/call_traits.hpp>
// 18 Mar 2002:
//    Changed some names to prevent conflicts with some new type_traits additions.
// 03 Oct 2000:
//    Enabled extra tests for VC6.

// NOTE: taken from boost 1.65.1 and adapted for cppdevtk

#include <cppdevtk/base/config.hpp>
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)

#if (CPPDEVTK_COMPILER_MSVC)
#pragma warning(disable:4181) // : warning C4181: qualifier applied to reference type; ignored
#endif

#if (CPPDEVTK_COMPILER_CLANG)
// 'const' qualifier on reference type 'r_type' (aka 'int &') has no effect [-Wignored-qualifiers]
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif

#endif

#include "boost_call_traits_test.hpp"
#include "boost_test_optional/optional_test_common.hpp"
#include <cppdevtk/base/call_traits.hpp>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <typeinfo>


// a way prevent warnings for unused variables
//template<class T> inline void unused_variable(const T&) {}

//
// struct contained models a type that contains a type (for example std::pair)
// arrays are contained by value, and have to be treated as a special case:
//
template <class T>
struct contained
{
   // define our typedefs first, arrays are stored by value
   // so value_type is not the same as result_type:
   typedef typename ::cppdevtk::base::CallTraits<T>::ParameterType       param_type;
   typedef typename ::cppdevtk::base::CallTraits<T>::Reference        reference;
   typedef typename ::cppdevtk::base::CallTraits<T>::ConstReference  const_reference;
   typedef T                                                value_type;
   typedef typename ::cppdevtk::base::CallTraits<T>::ValueType       result_type;

   // stored value:
   value_type v_;
   
   // constructors:
   contained() {}
   contained(param_type p) : v_(p){}
   // return byval:
   result_type value()const { return v_; }
   // return by_ref:
   reference get() { return v_; }
   const_reference const_get()const { return v_; }
   // pass value:
   void call(param_type){}
private:
   contained& operator=(const contained&);
};

//#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T, std::size_t N>
struct contained<T[N]>
{
   typedef typename ::cppdevtk::base::CallTraits<T[N]>::ParameterType       param_type;
   typedef typename ::cppdevtk::base::CallTraits<T[N]>::Reference        reference;
   typedef typename ::cppdevtk::base::CallTraits<T[N]>::ConstReference  const_reference;
   typedef T                                                   value_type[N];
   typedef typename ::cppdevtk::base::CallTraits<T[N]>::ValueType       result_type;

   value_type v_;

   contained(param_type p)
   {
      std::copy(p, p+N, v_);
   }
   // return byval:
   result_type value()const { return v_; }
   // return by_ref:
   reference get() { return v_; }
   const_reference const_get()const { return v_; }
   void call(param_type){}
private:
   contained& operator=(const contained&);
};
//#endif

template <class T>
contained<typename ::cppdevtk::base::CallTraits<T>::ValueType> test_wrap_type(const T& t)
{
   typedef typename ::cppdevtk::base::CallTraits<T>::ValueType ct;
   return contained<ct>(t);
}


struct test_abc1
{
   test_abc1();
   virtual ~test_abc1();
   test_abc1(const test_abc1&);
   test_abc1& operator=(const test_abc1&);
   virtual void foo() = 0;
   virtual void foo2() = 0;
};


struct incomplete_type;


enum enum_UDT{ one, two, three };


namespace test{

template <class T1, class T2>
std::pair<
   typename ::cppdevtk::base::CallTraits<T1>::ValueType,
   typename ::cppdevtk::base::CallTraits<T2>::ValueType>
      make_pair(const T1& t1, const T2& t2)
{
   return std::pair<
      typename ::cppdevtk::base::CallTraits<T1>::ValueType,
      typename ::cppdevtk::base::CallTraits<T2>::ValueType>(t1, t2);
}

} // namespace test

using namespace std;

//
// struct CallTraits_checker:
// verifies behaviour of contained example:
//
template <class T>
struct CallTraits_checker
{
   typedef typename ::cppdevtk::base::CallTraits<T>::ParameterType param_type;
   void operator()(param_type);
};

template <class T>
void CallTraits_checker<T>::operator()(param_type p)
{
   T t(p);
   contained<T> c(t);
   cout << "checking contained<" << typeid(T).name() << ">..." << endl;
   CPPDEVTK_BOOST_CHECK(t == c.value());
   CPPDEVTK_BOOST_CHECK(t == c.get());
   CPPDEVTK_BOOST_CHECK(t == c.const_get());
//#ifndef __ICL
   //cout << "typeof contained<" << typeid(T).name() << ">::v_ is:           " << typeid(&contained<T>::v_).name() << endl;
   cout << "typeof contained<" << typeid(T).name() << ">::value() is:      " << typeid(&contained<T>::value).name() << endl;
   cout << "typeof contained<" << typeid(T).name() << ">::get() is:        " << typeid(&contained<T>::get).name() << endl;
   cout << "typeof contained<" << typeid(T).name() << ">::const_get() is:  " << typeid(&contained<T>::const_get).name() << endl;
   cout << "typeof contained<" << typeid(T).name() << ">::call() is:       " << typeid(&contained<T>::call).name() << endl;
   cout << endl;
//#endif
}

//#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T, std::size_t N>
struct CallTraits_checker<T[N]>
{
   typedef typename ::cppdevtk::base::CallTraits<T[N]>::ParameterType param_type;
   void operator()(param_type t)
   {
      contained<T[N]> c(t);
      cout << "checking contained<" << typeid(T[N]).name() << ">..." << endl;
      unsigned int i = 0;
      for(i = 0; i < N; ++i)
         CPPDEVTK_BOOST_CHECK(t[i] == c.value()[i]);
      for(i = 0; i < N; ++i)
         CPPDEVTK_BOOST_CHECK(t[i] == c.get()[i]);
      for(i = 0; i < N; ++i)
         CPPDEVTK_BOOST_CHECK(t[i] == c.const_get()[i]);

      cout << "typeof contained<" << typeid(T[N]).name() << ">::v_ is:         " << typeid(&contained<T[N]>::v_).name() << endl;
      cout << "typeof contained<" << typeid(T[N]).name() << ">::value is:      " << typeid(&contained<T[N]>::value).name() << endl;
      cout << "typeof contained<" << typeid(T[N]).name() << ">::get is:        " << typeid(&contained<T[N]>::get).name() << endl;
      cout << "typeof contained<" << typeid(T[N]).name() << ">::const_get is:  " << typeid(&contained<T[N]>::const_get).name() << endl;
      cout << "typeof contained<" << typeid(T[N]).name() << ">::call is:       " << typeid(&contained<T[N]>::call).name() << endl;
      cout << endl;
   }
};
//#endif

//
// check_wrap:
template <class W, class U>
void check_wrap(const W& w, const U& u)
{
   cout << "checking " << typeid(W).name() << "..." << endl;
   CPPDEVTK_BOOST_CHECK(w.value() == u);
}

//
// check_make_pair:
// verifies behaviour of "make_pair":
//
template <class T, class U, class V>
void check_make_pair(T c, U u, V v)
{
   cout << "checking std::pair<" << typeid(c.first).name() << ", " << typeid(c.second).name() << ">..." << endl;
   CPPDEVTK_BOOST_CHECK(c.first == u);
   CPPDEVTK_BOOST_CHECK(c.second == v);
   cout << endl;
}


struct comparible_UDT
{
   int i_;
   comparible_UDT() : i_(2){}
   comparible_UDT(const comparible_UDT& other) : i_(other.i_){}
   comparible_UDT& operator=(const comparible_UDT& other)
   { 
      i_ = other.i_;
      return *this;
   }
   bool operator == (const comparible_UDT& v){ return v.i_ == i_; }
};

bool TestCallTraits()
{
   CallTraits_checker<comparible_UDT> c1;
   comparible_UDT u;
   c1(u);
   CallTraits_checker<int> c2;
   CallTraits_checker<enum_UDT> c2b;
   int i = 2;
   c2(i);
   c2b(one);
   int* pi = &i;
   int a[2] = {1,2};
//#if defined(BOOST_MSVC6_MEMBER_TEMPLATES) && !defined(__ICL)
   CallTraits_checker<int*> c3;
   c3(pi);
   CallTraits_checker<int&> c4;
   c4(i);
   CallTraits_checker<const int&> c5;
   c5(i);
//#if !defined (BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(__MWERKS__) && !defined(__SUNPRO_CC)
   CallTraits_checker<int[2]> c6;
   c6(a);
//#endif
//#endif

   check_wrap(test_wrap_type(2), 2);
//#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(__SUNPRO_CC)
   check_wrap(test_wrap_type(a), a);
   check_make_pair(test::make_pair(a, a), a, a);
//#endif

   // cv-qualifiers applied to reference types should have no effect
   // declare these here for later use with is_reference and remove_reference:
   typedef int& r_type;
   typedef const r_type cr_type;

   CPPDEVTK_BOOST_CHECK_TYPE(comparible_UDT, ::cppdevtk::base::CallTraits<comparible_UDT>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(comparible_UDT&, ::cppdevtk::base::CallTraits<comparible_UDT>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(const comparible_UDT&, ::cppdevtk::base::CallTraits<comparible_UDT>::ConstReference);
   CPPDEVTK_BOOST_CHECK_TYPE(const comparible_UDT&, ::cppdevtk::base::CallTraits<comparible_UDT>::ParameterType);
   CPPDEVTK_BOOST_CHECK_TYPE(int, ::cppdevtk::base::CallTraits<int>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(int&, ::cppdevtk::base::CallTraits<int>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(const int&, ::cppdevtk::base::CallTraits<int>::ConstReference);
   #if 0	// cppdevtk not as boost
   CPPDEVTK_BOOST_CHECK_TYPE(const int, ::cppdevtk::base::CallTraits<int>::ParameterType);
   #else
   CPPDEVTK_BOOST_CHECK_TYPE(int, ::cppdevtk::base::CallTraits<int>::ParameterType);
   #endif
   CPPDEVTK_BOOST_CHECK_TYPE(int*, ::cppdevtk::base::CallTraits<int*>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(int*&, ::cppdevtk::base::CallTraits<int*>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(int*const&, ::cppdevtk::base::CallTraits<int*>::ConstReference);
   #if 0	// cppdevtk not as boost
   CPPDEVTK_BOOST_CHECK_TYPE(int*const, ::cppdevtk::base::CallTraits<int*>::ParameterType);
   #else
   CPPDEVTK_BOOST_CHECK_TYPE(int*, ::cppdevtk::base::CallTraits<int*>::ParameterType);
   #endif
//#if defined(BOOST_MSVC6_MEMBER_TEMPLATES)
   CPPDEVTK_BOOST_CHECK_TYPE(int&, ::cppdevtk::base::CallTraits<int&>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(int&, ::cppdevtk::base::CallTraits<int&>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(const int&, ::cppdevtk::base::CallTraits<int&>::ConstReference);
   CPPDEVTK_BOOST_CHECK_TYPE(int&, ::cppdevtk::base::CallTraits<int&>::ParameterType);
//#if !(defined(__GNUC__) && ((__GNUC__ < 3) || (__GNUC__ == 3) && (__GNUC_MINOR__ < 1)))
   CPPDEVTK_BOOST_CHECK_TYPE(int&, ::cppdevtk::base::CallTraits<cr_type>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(int&, ::cppdevtk::base::CallTraits<cr_type>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(const int&, ::cppdevtk::base::CallTraits<cr_type>::ConstReference);
   CPPDEVTK_BOOST_CHECK_TYPE(int&, ::cppdevtk::base::CallTraits<cr_type>::ParameterType);
//#else
//   std::cout << "Your compiler cannot instantiate CallTraits<int&const>, skipping four tests (4 errors)" << std::endl;
//#endif
   CPPDEVTK_BOOST_CHECK_TYPE(const int&, ::cppdevtk::base::CallTraits<const int&>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(const int&, ::cppdevtk::base::CallTraits<const int&>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(const int&, ::cppdevtk::base::CallTraits<const int&>::ConstReference);
   CPPDEVTK_BOOST_CHECK_TYPE(const int&, ::cppdevtk::base::CallTraits<const int&>::ParameterType);
//#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
   CPPDEVTK_BOOST_CHECK_TYPE(const int*, ::cppdevtk::base::CallTraits<int[3]>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(int(&)[3], ::cppdevtk::base::CallTraits<int[3]>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(const int(&)[3], ::cppdevtk::base::CallTraits<int[3]>::ConstReference);
   #if 0	// cppdevtk not as boost
   CPPDEVTK_BOOST_CHECK_TYPE(const int*const, ::cppdevtk::base::CallTraits<int[3]>::ParameterType);
   #else
   CPPDEVTK_BOOST_CHECK_TYPE(const int*, ::cppdevtk::base::CallTraits<int[3]>::ParameterType);
   #endif
   CPPDEVTK_BOOST_CHECK_TYPE(const int*, ::cppdevtk::base::CallTraits<const int[3]>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(const int(&)[3], ::cppdevtk::base::CallTraits<const int[3]>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(const int(&)[3], ::cppdevtk::base::CallTraits<const int[3]>::ConstReference);
   #if 0	// cppdevtk not as boost
   CPPDEVTK_BOOST_CHECK_TYPE(const int*const, ::cppdevtk::base::CallTraits<const int[3]>::ParameterType);
   #else
   CPPDEVTK_BOOST_CHECK_TYPE(const int*, ::cppdevtk::base::CallTraits<const int[3]>::ParameterType);
   #endif
   // test with abstract base class:
   CPPDEVTK_BOOST_CHECK_TYPE(test_abc1, ::cppdevtk::base::CallTraits<test_abc1>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(test_abc1&, ::cppdevtk::base::CallTraits<test_abc1>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(const test_abc1&, ::cppdevtk::base::CallTraits<test_abc1>::ConstReference);
   CPPDEVTK_BOOST_CHECK_TYPE(const test_abc1&, ::cppdevtk::base::CallTraits<test_abc1>::ParameterType);
//#else
//   std::cout << "You're compiler does not support partial template specialiation, skipping 8 tests (8 errors)" << std::endl;
//#endif
//#else
//   std::cout << "You're compiler does not support partial template specialiation, skipping 20 tests (20 errors)" << std::endl;
//#endif
   // test with an incomplete type:
   CPPDEVTK_BOOST_CHECK_TYPE(incomplete_type, ::cppdevtk::base::CallTraits<incomplete_type>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(incomplete_type&, ::cppdevtk::base::CallTraits<incomplete_type>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(const incomplete_type&, ::cppdevtk::base::CallTraits<incomplete_type>::ConstReference);
   CPPDEVTK_BOOST_CHECK_TYPE(const incomplete_type&, ::cppdevtk::base::CallTraits<incomplete_type>::ParameterType);
   // test enum:
   CPPDEVTK_BOOST_CHECK_TYPE(enum_UDT, ::cppdevtk::base::CallTraits<enum_UDT>::ValueType);
   CPPDEVTK_BOOST_CHECK_TYPE(enum_UDT&, ::cppdevtk::base::CallTraits<enum_UDT>::Reference);
   CPPDEVTK_BOOST_CHECK_TYPE(const enum_UDT&, ::cppdevtk::base::CallTraits<enum_UDT>::ConstReference);
   #if 0	// cppdevtk not as boost
   CPPDEVTK_BOOST_CHECK_TYPE(const enum_UDT, ::cppdevtk::base::CallTraits<enum_UDT>::ParameterType);
   #else
   CPPDEVTK_BOOST_CHECK_TYPE(enum_UDT, ::cppdevtk::base::CallTraits<enum_UDT>::ParameterType);
   #endif
   
   return true;
}

//
// define CallTraits tests to check that the assertions in the docs do actually work
// this is an compile-time only set of tests:
//
template <typename T, bool isarray = false>
struct CallTraits_test
{
   typedef ::cppdevtk::base::CallTraits<T> ct;
   typedef typename ct::ParameterType param_type;
   typedef typename ct::Reference reference;
   typedef typename ct::ConstReference const_reference;
   typedef typename ct::ValueType value_type;
   static void assert_construct(param_type val);
};

template <typename T, bool isarray>
void CallTraits_test<T, isarray>::assert_construct(param_type val)
{
   //
   // this is to check that the CallTraits assertions are valid:
   T t(val);
   value_type v(t);
   reference r(t);
   const_reference cr(t);
   param_type p(t);
   value_type v2(v);
   value_type v3(r);
   value_type v4(p);
   reference r2(v);
   reference r3(r);
   const_reference cr2(v);
   const_reference cr3(r);
   const_reference cr4(cr);
   const_reference cr5(p);
   param_type p2(v);
   param_type p3(r);
   param_type p4(p);
   
   unused_variable(v2);
   unused_variable(v3);
   unused_variable(v4);
   unused_variable(r2);
   unused_variable(r3);
   unused_variable(cr2);
   unused_variable(cr3);
   unused_variable(cr4);
   unused_variable(cr5);
   unused_variable(p2);
   unused_variable(p3);
   unused_variable(p4);
}
//#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <typename T>
struct CallTraits_test<T, true>
{
   typedef ::cppdevtk::base::CallTraits<T> ct;
   typedef typename ct::ParameterType param_type;
   typedef typename ct::Reference reference;
   typedef typename ct::ConstReference const_reference;
   typedef typename ct::ValueType value_type;
   static void assert_construct(param_type val);
};

template <typename T>
void CallTraits_test<T, true>::assert_construct(param_type val)
{
   //
   // this is to check that the CallTraits assertions are valid:
   T t;
   value_type v(t);
   value_type v5(val);
   reference r = t;
   const_reference cr = t;
   reference r2 = r;
   #ifndef __BORLANDC__
   // C++ Builder buglet:
   const_reference cr2 = r;
   #endif
   param_type p(t);
   value_type v2(v);
   const_reference cr3 = cr;
   value_type v3(r);
   value_type v4(p);
   param_type p2(v);
   param_type p3(r);
   param_type p4(p);
   
   unused_variable(v2);
   unused_variable(v3);
   unused_variable(v4);
   unused_variable(v5);
#ifndef __BORLANDC__
   unused_variable(r2);
   unused_variable(cr2);
#endif
   unused_variable(cr3);
   unused_variable(p2);
   unused_variable(p3);
   unused_variable(p4);
}
//#endif //BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//
// now check CallTraits assertions by instantiating CallTraits_test:
template struct CallTraits_test<int>;
template struct CallTraits_test<const int>;
template struct CallTraits_test<int*>;
//#if defined(BOOST_MSVC6_MEMBER_TEMPLATES)
template struct CallTraits_test<int&>;
template struct CallTraits_test<const int&>;
//#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(__SUNPRO_CC)
template struct CallTraits_test<int[2], true>;
//#endif
//#endif

#if 0
CallTraits_test<int> cti;
CallTraits_test<const int> ctci;
CallTraits_test<int*> ctip;
CallTraits_test<int&> ctir;
CallTraits_test<const int&> ctcir;
CallTraits_test<int[2], true> ctai;
#endif
