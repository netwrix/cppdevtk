/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2018 CoSoSys Ltd <info@cososys.com>\n
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

#if (!CPPDEVTK_COMPILER_CLANG)
#	error "This file is Clang specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_COMPILER_CLANG_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_COMPILER_CLANG_HPP_INCLUDED_


/// \cond


// Please see https://sourceforge.net/p/predef/wiki/Compilers/#clang

#define CPPDEVTK_CLANG_VERSION_NUM CPPDEVTK_CLANG_VERSION_NUM_FROM_COMPONENTS(	\
		__clang_major__, __clang_minor__, __clang_patchlevel__)

#define CPPDEVTK_CLANG_VERSION_NUM_FROM_COMPONENTS(verMajor, verMinor, verPatch)	\
	((verMajor * 10000) + (verMinor * 100) + (verPatch * 1))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Minimum requirement: Clang 3.2
#ifndef __apple_build_version__	// Apple Clang is handled in macosx_clang.hpp
#	if (CPPDEVTK_CLANG_VERSION_NUM < CPPDEVTK_CLANG_VERSION_NUM_FROM_COMPONENTS(3, 2, 0))
#		error "Clang >= 3.2 required!!!"
#	endif
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPPDEVTK_COMPILER_MESSAGE and CPPDEVTK_COMPILER_WARNING

// Please see Controlling Diagnostics via Pragmas:
// http://llvm.org/releases/3.3/tools/clang/docs/UsersManual.html#controlling-diagnostics-via-pragmas
#define CPPDEVTK_DETAIL_CLANG_DO_PRAGMA(msg)	\
	_Pragma(#msg)

#define CPPDEVTK_COMPILER_MESSAGE(msg)	\
	CPPDEVTK_DETAIL_CLANG_DO_PRAGMA(message("message: " #msg))

#define CPPDEVTK_COMPILER_WARNING(msg)	\
	CPPDEVTK_DETAIL_CLANG_DO_PRAGMA(GCC warning #msg)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPPDEVTK_FUNCTION_LONG_NAME and CPPDEVTK_FUNCTION_SHORT_NAME
#define CPPDEVTK_FUNCTION_LONG_NAME __PRETTY_FUNCTION__ 
#define CPPDEVTK_FUNCTION_SHORT_NAME __FUNCTION__ 


// https://gcc.gnu.org/onlinedocs/gcc-3.3/gcc/Thread-Local.html
#define CPPDEVTK_THREAD __thread


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compiler Features.

#define CPPDEVTK_HAVE_STDINT_H 1
#define CPPDEVTK_HAVE_INTTYPES_H 1
#define CPPDEVTK_HAVE_STRINGS_H 1
#define CPPDEVTK_HAVE_CXXABI_H 1
#define CPPDEVTK_HAVE_ABI___CXA_DEMANGLE 1

#define CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG 0
#define CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL 1

// There is no way to detect that -fno-threadsafe-statics was used; assume threadsafe-statics enabled
#define CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS 1


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TR1
#define CPPDEVTK_HAVE_TR1_DIR 0
#define CPPDEVTK_HAVE_TR1_NS 0


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C++11

// C++ Support in Clang: http://clang.llvm.org/cxx_status.html
// Xcode Version Information: https://trac.macports.org/wiki/XcodeVersionInfo
// Quote: "Clang 3.3 and later implement all of the ISO C++ 2011 standard"
// "By default, Clang builds C++ code according to the C++98 standard, with many C++11 features accepted as extensions."
// Default XCode generated project enable C++11.
// But there is no info about libc++ and C++11.
// According to cppreference.com, C++11 library support in libc++ is complete as of 2012-07-29.
// Please see C++ compiler support, References, Clang++: http://en.cppreference.com/w/cpp/compiler_support
// Also, since Clang 3.4, the C++ SD-6 feature test macros are also supported
// Please see Clang Language Extensions, Checks for Standard Language Features
// http://clang.llvm.org/docs/LanguageExtensions.html#checks-for-standard-language-features

#ifdef __cplusplus
#if ((__cplusplus < 201103L) && !defined(__GXX_EXPERIMENTAL_CXX0X__))
#	error "CppDevTk require C++11 enabled for clang"
#endif
#endif

// Please see Clang Language Extensions, C++11 static_assert(): http://clang.llvm.org/docs/LanguageExtensions.html
#if (__has_extension(cxx_static_assert))
#	define CPPDEVTK_HAVE_CPP11_STATIC_ASSERT 1
#else
#	define CPPDEVTK_HAVE_CPP11_STATIC_ASSERT 0
#endif

#define CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR (defined(__has_include) && __has_include(<system_error>))

#if ((__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__))
#	define CPPDEVTK_HAVE_CPP11_MUTEX 1
#	define CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE 1
#	define CPPDEVTK_HAVE_CPP11_THREAD 1
#	define CPPDEVTK_HAVE_CPP11_FUTURE 1
#	define CPPDEVTK_HAVE_CPP11_ATOMIC 1
#	define CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION 1
#	define CPPDEVTK_HAVE_CPP11_AT_QUICK_EXIT 1
#else
#	define CPPDEVTK_HAVE_CPP11_MUTEX 0
#	define CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE 0
#	define CPPDEVTK_HAVE_CPP11_THREAD 0
#	define CPPDEVTK_HAVE_CPP11_FUTURE 0
#	define CPPDEVTK_HAVE_CPP11_ATOMIC 0
#	define CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION 0
#	define CPPDEVTK_HAVE_CPP11_AT_QUICK_EXIT 0
#endif
#if (__has_feature(cxx_thread_local))
#	define CPPDEVTK_HAVE_CPP11_TLS 1
#else
#	define CPPDEVTK_HAVE_CPP11_TLS 0
#endif
#if (__has_extension(cxx_rvalue_references))
#	define CPPDEVTK_HAVE_CPP11_RVALUE_REFERENCES 1
#else
#	define CPPDEVTK_HAVE_CPP11_RVALUE_REFERENCES 0
#endif
#if (__has_extension(cxx_deleted_functions))
#	define CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS 1
#else
#	define CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS 0
#endif
#if (__has_feature(cxx_constexpr))
#	define CPPDEVTK_HAVE_CPP11_CONSTEXPR 1
#else
#	define CPPDEVTK_HAVE_CPP11_CONSTEXPR 0
#endif
#if (__has_extension(cxx_decltype))
#	define CPPDEVTK_HAVE_CPP11_DECLTYPE 1
#else
#	define CPPDEVTK_HAVE_CPP11_DECLTYPE 0
#endif
#if (__has_extension(cxx_alias_templates))
#	define CPPDEVTK_HAVE_CPP11_TEMPLATE_ALIAS 1
#else
#	define CPPDEVTK_HAVE_CPP11_TEMPLATE_ALIAS 0
#endif
#if (__has_extension(cxx_lambdas))
#	define CPPDEVTK_HAVE_CPP11_LAMBDAS 1
#else
#	define CPPDEVTK_HAVE_CPP11_LAMBDAS 0
#endif
#define CPPDEVTK_HAVE_CPP11_NOEXCEPT 1	// introduced in Clang 3.0 and we require > 3.2

// Please see Clang Language Extensions, C++ RTTI: http://clang.llvm.org/docs/LanguageExtensions.html
#if (__has_feature(cxx_rtti))
#	define CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI 1
#else
#	define CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI 0
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Third party warnings.
#if (CPPDEVTK_DISABLE_THIRD_PARTY_WARNINGS)
#	pragma GCC diagnostic ignored "-Wdeprecated-declarations"	// from Qt Headers
// TODO: put other third-party warnings safe to disable here.
#endif	// (defined(CPPDEVTK_DETAIL_BUILD) || CPPDEVTK_DISABLE_THIRD_PARTY_WARNINGS)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cppdevtk warnings.
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)
//#	pragma GCC diagnostic ignored "-Wunused-local-typedefs"	// typedef 'CppDevTkStaticAssert_1' locally defined but not used
// TODO: put other cppdevtk warnings safe to disable here.
#endif	// (defined(CPPDEVTK_DETAIL_BUILD) || CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)


/// \endcond


#endif	// CPPDEVTK_CONFIG_COMPILER_CLANG_HPP_INCLUDED_
