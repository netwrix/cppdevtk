/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2017 CoSoSys Ltd <info@cososys.com>\n
/// Licensed under the Apache License, Version 2.0 (the "License");\n
/// you may not use this file except in compliance with the License.\n
/// You may obtain a copy of the License at\n
/// http://www.apache.org/licenses/LICENSE-2.0\n
/// Unless required by applicable law or agreed to in writing, software\n
/// distributed under the License is distributed on an "AS IS" BASIS,\n
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
/// See the License for the specific language governing permissions and\n
/// limitations under the License.\n
/// Please see the file COPYING.
///
/// \authors Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CPPDEVTK_CONFIG_COMPILER_HPP_INCLUDED_
#	error "Do not include directly; please include <cppdevtk/config/compiler.hpp> instead!!!"
#endif

#if	(!CPPDEVTK_COMPILER_GCC)
#	error "This file is GCC specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_COMPILER_GCC_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_COMPILER_GCC_HPP_INCLUDED_


/// \cond


// Please see https://sourceforge.net/p/predef/wiki/Compilers/#gcc-cc

#define CPPDEVTK_GNUC_VERSION_NUM CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(	\
		__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)

#define CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(verMajor, verMinor, verPatch)	\
	((verMajor * 10000) + (verMinor * 100) + (verPatch * 1))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Minimum requirement: GCC 4.0.1
#if (CPPDEVTK_GNUC_VERSION_NUM < CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 0, 1))
#	error "GCC >= 4.0.1 required!!!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPPDEVTK_COMPILER_MESSAGE and CPPDEVTK_COMPILER_WARNING

// Please see GCC Pragmas: https://gcc.gnu.org/onlinedocs/cpp/Pragmas.html
#define CPPDEVTK_DETAIL_GCC_DO_PRAGMA(msg)	\
	_Pragma(#msg)

#define CPPDEVTK_COMPILER_MESSAGE(msg)	\
	CPPDEVTK_DETAIL_GCC_DO_PRAGMA(message("message: " #msg))

// pragma GCC warning was added in GCC 4.8.0
// Please see Pragmas: https://gcc.gnu.org/onlinedocs/gcc-4.8.0/cpp/Pragmas.html#Pragmas
#if (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 8, 0))
#	define CPPDEVTK_COMPILER_WARNING(msg)	\
		CPPDEVTK_DETAIL_GCC_DO_PRAGMA(GCC warning #msg)
#else
#	define CPPDEVTK_COMPILER_WARNING(msg)	\
		CPPDEVTK_DETAIL_GCC_DO_PRAGMA(message("warning: " #msg))
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPPDEVTK_FUNCTION_LONG_NAME and CPPDEVTK_FUNCTION_SHORT_NAME
#define CPPDEVTK_FUNCTION_LONG_NAME __PRETTY_FUNCTION__ 
#define CPPDEVTK_FUNCTION_SHORT_NAME __FUNCTION__ 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compiler Features.

#define CPPDEVTK_HAVE_STDINT_H 1
#define CPPDEVTK_HAVE_INTTYPES_H 1
#define CPPDEVTK_HAVE_STRINGS_H 1
#define CPPDEVTK_HAVE_CXXABI_H 1
#define CPPDEVTK_HAVE_ABI___CXA_DEMANGLE 1

#define CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG 0

// please see forward declaration of explicit instantiations: https://gcc.gnu.org/onlinedocs/gcc-2.95.3/gcc_5.html#SEC110
#if (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(2, 95, 0))
#	define CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL 1
#else
#	define CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL 0
#endif

// Please see -fno-threadsafe-statics:
// https://gcc.gnu.org/gcc-4.0/changes.html
// https://gcc.gnu.org/onlinedocs/gcc-4.0.4/gcc/C_002b_002b-Dialect-Options.html#C_002b_002b-Dialect-Options
// There is no way to detect that -fno-threadsafe-statics was used; assume threadsafe-statics enabled
#define CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS 1


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TR1
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 3, 0)))
#	define CPPDEVTK_HAVE_TR1_DIR 0
#	define CPPDEVTK_HAVE_TR1_NS 0
#else
#	define CPPDEVTK_HAVE_TR1_DIR 1
#	define CPPDEVTK_HAVE_TR1_NS 1
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C++11

// - The default mode for C++ is -std=gnu++98 in gcc <= 6.x and -std=gnu++14 in 6.x
// Please see GCC 6 Release Series Changes, New Features, and Fixes, Caveats:
// https://gcc.gnu.org/gcc-6/changes.html
// - Before GCC 5.1 support for C++11 was experimental
// - GCC 4.8.1 was the first feature-complete implementation of the 2011 C++ standard, previously known as C++0x.
// Please see C++ Standards Support in GCC, C++11 Support in GCC: https://gcc.gnu.org/projects/cxx-status.html#cxx11
// - libstdc++, Status, C++ 2011 is for mainline (not a particular release)
// please see: https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2011
// - according to cppreference.com, C++11 library support in libstdc++ is complete as of GCC 5.0.
// please see C++ compiler support, References, GCC: http://en.cppreference.com/w/cpp/compiler_support
// - Starting with 4.7 G++ sets the predefined macro __cplusplus to the correct value, 199711L for C++98/03, and 201103L for C++11.
// Please see GCC 4.7 Release Series Changes, New Features, and Fixes: https://gcc.gnu.org/gcc-4.7/changes.html
// - Before __cplusplus was fixed, __GXX_EXPERIMENTAL_CXX0X__ could have been used to check if C++11 is enabled

#ifdef __cplusplus
#if ((CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 3, 0))	\
		&& ((__cplusplus < 201103L) && !defined(__GXX_EXPERIMENTAL_CXX0X__)))
#	error "CppDevTk require C++11 enabled for GCC >= 4.3.0"
#endif
#endif

// static_assert was introduced in GCC 4.3
// Please see C++ Standards Support in GCC, C++11 Support in GCC, Static assertions
// https://gcc.gnu.org/projects/cxx-status.html#cxx11
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 3, 0)))
#	define CPPDEVTK_HAVE_CPP11_STATIC_ASSERT 1
#else
#	define CPPDEVTK_HAVE_CPP11_STATIC_ASSERT 0
#endif

#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 4, 0)))
#	define CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR 1
#	define CPPDEVTK_HAVE_CPP11_MUTEX 1
#	define CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE 1
#	define CPPDEVTK_HAVE_CPP11_THREAD 1
#else
#	define CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR 0
#	define CPPDEVTK_HAVE_CPP11_MUTEX 0
#	define CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE 0
#	define CPPDEVTK_HAVE_CPP11_THREAD 0
#endif
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 5, 0)))
#	define CPPDEVTK_HAVE_CPP11_FUTURE 1
#	define CPPDEVTK_HAVE_CPP11_ATOMIC 1
#else
#	define CPPDEVTK_HAVE_CPP11_FUTURE 0
#	define CPPDEVTK_HAVE_CPP11_ATOMIC 0
#endif
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 4, 0)))
#	define CPPDEVTK_HAVE_CPP11_EXCEPTION_PTR 1
#else
#	define CPPDEVTK_HAVE_CPP11_EXCEPTION_PTR 0
#endif
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 8, 0)))
#	define CPPDEVTK_HAVE_CPP11_AT_QUICK_EXIT 1
#else
#	define CPPDEVTK_HAVE_CPP11_AT_QUICK_EXIT 0
#endif
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 8, 0)))
#	define CPPDEVTK_HAVE_CPP11_TLS 1
#else
#	define CPPDEVTK_HAVE_CPP11_TLS 0
#endif
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 3, 0)))
#	define CPPDEVTK_HAVE_CPP11_RVALUE_REFERENCES 1
#else
#	define CPPDEVTK_HAVE_CPP11_RVALUE_REFERENCES 0
#endif
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 4, 0)))
#	define CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS 1
#else
#	define CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS 0
#endif
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 6, 0)))
#	define CPPDEVTK_HAVE_CPP11_CONSTEXPR 1
#else
#	define CPPDEVTK_HAVE_CPP11_CONSTEXPR 0
#endif
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 3, 0)))
#	define CPPDEVTK_HAVE_CPP11_DECLTYPE 1
#else
#	define CPPDEVTK_HAVE_CPP11_DECLTYPE 0
#endif
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 7, 0)))
#	define CPPDEVTK_HAVE_CPP11_TEMPLATE_ALIAS 1
#else
#	define CPPDEVTK_HAVE_CPP11_TEMPLATE_ALIAS 0
#endif
#if (((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))	\
		&& (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 5, 0)))
#	define CPPDEVTK_HAVE_CPP11_LAMBDAS 1
#else
#	define CPPDEVTK_HAVE_CPP11_LAMBDAS 0
#endif


// Please see -fno-rtti: https://gcc.gnu.org/onlinedocs/gcc-2.95.3/gcc_2.html#SEC7
// __GXX_RTTI was introduced in GCC 4.3.0 (please see GCC 4.3.0 ChangeLog)
#if (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 3, 0))
#	ifdef __GXX_RTTI
#		define CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI 1
#	else
#		define CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI 0
#	endif
#else
	// We can not detect; assume RTTI is enabled
#	define CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI 1
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// glibc defines; please see Feature Test Macros: http://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html

#ifndef _GNU_SOURCE
#	define _GNU_SOURCE
#endif

#ifndef _REENTRANT
#	define _REENTRANT
#endif

#ifndef _THREAD_SAFE
#	define _THREAD_SAFE
#endif

#ifdef NDEBUG
#	if (FORTIFY_SOURCE != 2)
#		ifdef CPPDEVTK_DETAIL_BUILD
//			CPPDEVTK_COMPILER_WARNING("setting FORTIFY_SOURCE to 2")
#			undef FORTIFY_SOURCE
#			define FORTIFY_SOURCE 2
#		endif
#	endif
#endif


// To disable warnings we use pragma GCC diagnostic ignored
// Diagnostic Pragmas were added in GCC 4.2
// Please see Diagnostic Pragmas: https://gcc.gnu.org/onlinedocs/gcc-4.2.0/gcc/Diagnostic-Pragmas.html#Diagnostic-Pragmas
#if (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 2, 0))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Third party warnings.
#if (CPPDEVTK_DISABLE_THIRD_PARTY_WARNINGS)
#	if (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 2, 4))
//#		pragma GCC diagnostic ignored "-Wunused-parameter"
#	endif
// TODO: put other third-party warnings safe to disable here.
#endif	// (defined(CPPDEVTK_DETAIL_BUILD) || CPPDEVTK_DISABLE_THIRD_PARTY_WARNINGS)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cppdevtk warnings.
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)
#	if (!CPPDEVTK_HAVE_CPP11_STATIC_ASSERT)
#		if (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 7, 0))
#			pragma GCC diagnostic ignored "-Wunused-local-typedefs"	// typedef 'CppDevTkStaticAssert_1' locally defined but not used
#		endif
#	endif
#	if ((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))
#		pragma GCC diagnostic ignored "-Wdeprecated-declarations"	// template<class> class std::auto_ptr is deprecated if C++11 enabled
#	endif
// TODO: put other cppdevtk warnings safe to disable here.
#endif	// (defined(CPPDEVTK_DETAIL_BUILD) || CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)

#endif	// Disable warnings (Diagnostic Pragmas, GCC >= 4.2)


/// \endcond


#endif	// CPPDEVTK_CONFIG_COMPILER_GCC_HPP_INCLUDED_
