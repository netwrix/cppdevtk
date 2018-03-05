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

#ifdef RC_INVOKED
#	error "Internal error: This file should not be included when RC_INVOKED!!!"
#endif

#if (!CPPDEVTK_COMPILER_MSVC)
#	error "This file is MSVC specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_COMPILER_MSVC_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_COMPILER_MSVC_HPP_INCLUDED_


/// \cond


// Please see https://sourceforge.net/p/predef/wiki/Compilers/#microsoft-visual-c


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPPDEVTK_COMPILER_MESSAGE and CPPDEVTK_COMPILER_WARNING

#define CPPDEVTK_COMPILER_MESSAGE(msg)	\
	__pragma(message(__FILE__ "(" CPPDEVTK_STRINGIZE(__LINE__) ") message: " #msg))

#define CPPDEVTK_COMPILER_WARNING(msg)	\
	__pragma(message(__FILE__ "(" CPPDEVTK_STRINGIZE(__LINE__) ") warning: " #msg))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Minimum requirement: MSVC 2008 (9.0) SP1.
// Rationale:
// We need TR1 and it was first introduced in MSVC 2008 (9.0) with Feature Pack Release,
// but Feature Pack Release was not available in Express Edition.

// MSVC 2008 (9.0) SP1: _MSC_VER: 1500; MSC_FULL_VER: 150030729; _MSC_BUILD: 1
#if ((_MSC_VER < 1500) || ((_MSC_VER == 1500) && ((_MSC_FULL_VER < 150030729)	\
		|| ((_MSC_FULL_VER == 150030729) && (_MSC_BUILD < 1)))))
#	error "MSVC >= 2008 (9.0) SP1 required!!!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPPDEVTK_FUNCTION_LONG_NAME and CPPDEVTK_FUNCTION_SHORT_NAME
#define CPPDEVTK_FUNCTION_LONG_NAME __FUNCSIG__ 
#define CPPDEVTK_FUNCTION_SHORT_NAME __FUNCTION__ 


// https://msdn.microsoft.com/en-us/library/6yh4a9k1.aspx
#define CPPDEVTK_THREAD __declspec(thread)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRT secure C++ overloads; please see:
// Secure Template Overloads: https://msdn.microsoft.com/en-us/library/ms175759%28v=vs.90%29.aspx
// We compile our code with MSVC defaults: _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES and
// _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT are defined as 0 (disabled) and
// _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES is defined as 1 (enabled).

#ifdef CPPDEVTK_DETAIL_BUILD
#	ifdef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#		if (_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
			CPPDEVTK_COMPILER_WARNING("setting _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES to 0")
#			undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#			define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 0
#		endif
#	else
#		define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 0
#	endif

#	ifdef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT
#		if (_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT)
			CPPDEVTK_COMPILER_WARNING("setting _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT to 0")
#			undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT
#			define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 0
#		endif
#	else
#		define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 0
#	endif

#	if (_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT && !_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
		CPPDEVTK_COMPILER_WARNING("_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT will have no effect")
#	endif


#	ifdef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
#		if (!_CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES)
			CPPDEVTK_COMPILER_WARNING("setting _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES to 1")
#			undef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
#			define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
#		endif
#	else
#		define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
#	endif
#endif	// CPPDEVTK_DETAIL_BUILD


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sprintf: https://msdn.microsoft.com/en-us/library/ybk95axf%28v=vs.90%29.aspx
#ifdef _CRT_NON_CONFORMING_SWPRINTFS
	CPPDEVTK_COMPILER_WARNING("undefining _CRT_NON_CONFORMING_SWPRINTFS")
#	undef _CRT_NON_CONFORMING_SWPRINTFS
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compiler Features.

#if (_MSC_VER >= 1600)	// MSVC 2010 (10.0)
#	define CPPDEVTK_HAVE_STDINT_H 1
#else
#	define CPPDEVTK_HAVE_STDINT_H 0
#endif

#define CPPDEVTK_HAVE_INTTYPES_H 0
#define CPPDEVTK_HAVE_STRINGS_H 0
#define CPPDEVTK_HAVE_CXXABI_H 0
#define CPPDEVTK_HAVE_ABI___CXA_DEMANGLE 0

#define CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG 1 // TODO: test if fixed in MSVC > 2008 (9.0) SP1

// Please see:
// https://msdn.microsoft.com/en-us/library/by56e477%28v=vs.71%29.aspx
// https://support.microsoft.com/en-us/kb/168958
#if (_MSC_VER >= 1100)	// MSVC 5.0
#	define CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL 1
#else
#	define CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL 0
#endif

// Please see Support For C++11/14/17 Features, Magic statics: https://msdn.microsoft.com/en-us/library/hh567368.aspx
#if (_MSC_VER >= 1900)	// MSVC 2015 (14.0)
#	define CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS 1
#else
#	define CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS 0
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TR1

// MSVC 2008 (9.0) with Feature Pack Release has TR1 dir, but MSVC >= 2008 (9.0) SP1 not.
#define CPPDEVTK_HAVE_TR1_DIR 0
#if (_MSC_VER >= 1700)	// MSVC 2012 (11.0)
#	define CPPDEVTK_HAVE_TR1_NS 0
#else
#	define CPPDEVTK_HAVE_TR1_NS 1
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C++11

// Please see Support For C++11/14/17 Features: https://msdn.microsoft.com/en-us/library/hh567368.aspx

// Please see Support For C++11/14/17 Features, static_assert: https://msdn.microsoft.com/en-us/library/hh567368.aspx
#if (_MSC_VER >= 1600)	// MSVC 2010 (10.0)
#	define CPPDEVTK_HAVE_CPP11_STATIC_ASSERT 1
#	define CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR 1
#else
#	define CPPDEVTK_HAVE_CPP11_STATIC_ASSERT 0
#	define CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR 0
#endif

#if (_MSC_VER >= 1700)	// MSVC 2012 (11.0)
#	define CPPDEVTK_HAVE_CPP11_MUTEX 1
#	define CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE 1
#	define CPPDEVTK_HAVE_CPP11_THREAD 1
#	define CPPDEVTK_HAVE_CPP11_FUTURE 1
#	define CPPDEVTK_HAVE_CPP11_ATOMIC 1
#	define CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION 1
#else
#	define CPPDEVTK_HAVE_CPP11_MUTEX 0
#	define CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE 0
#	define CPPDEVTK_HAVE_CPP11_THREAD 0
#	define CPPDEVTK_HAVE_CPP11_FUTURE 0
#	define CPPDEVTK_HAVE_CPP11_ATOMIC 0
#	define CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION 0
#endif
#if (_MSC_VER >= 1900)	// MSVC 2015 (14.0)
#	define CPPDEVTK_HAVE_CPP11_AT_QUICK_EXIT 1
#else
#	define CPPDEVTK_HAVE_CPP11_AT_QUICK_EXIT 0
#endif
#if (_MSC_VER >= 1900)	// MSVC 2015 (14.0)
#	define CPPDEVTK_HAVE_CPP11_TLS 1
#else
#	define CPPDEVTK_HAVE_CPP11_TLS 0
#endif
#if (_MSC_VER >= 1900)	// MSVC 2015 (14.0)
#	define CPPDEVTK_HAVE_CPP11_RVALUE_REFERENCES 1
#else
#	define CPPDEVTK_HAVE_CPP11_RVALUE_REFERENCES 0
#endif
#if (_MSC_VER >= 1900)	// MSVC 2015 (14.0)
#	define CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS 1
#else
#	define CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS 0
#endif
#if (_MSC_VER >= 1900)	// MSVC 2015 (14.0)
#	define CPPDEVTK_HAVE_CPP11_CONSTEXPR 1
#else
#	define CPPDEVTK_HAVE_CPP11_CONSTEXPR 0
#endif
#if (_MSC_VER >= 1900)	// MSVC 2015 (14.0)
#	define CPPDEVTK_HAVE_CPP11_DECLTYPE 1
#else
#	define CPPDEVTK_HAVE_CPP11_DECLTYPE 0
#endif
#if (_MSC_VER >= 1800)	// MSVC 2013 (12.0)
#	define CPPDEVTK_HAVE_CPP11_TEMPLATE_ALIAS 1
#else
#	define CPPDEVTK_HAVE_CPP11_TEMPLATE_ALIAS 0
#endif
#if (_MSC_VER >= 1900)	// MSVC 2015 (14.0)
#	define CPPDEVTK_HAVE_CPP11_LAMBDAS 1
#	define CPPDEVTK_HAVE_CPP11_NOEXCEPT 1
#else
#	define CPPDEVTK_HAVE_CPP11_LAMBDAS 0
#	define CPPDEVTK_HAVE_CPP11_NOEXCEPT 0
#endif


#ifdef _CPPRTTI
#	define CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI 1
#else
#	define CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI 0
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPPDEVTK_SHARED and _DLL

// We do not support at this moment (and there is no plan, but users may change)
// CppDevTk static library with shared runtime; force CppDevTk shared if _DLL!
// This will also load dlls faster (dllimport) and avoid template explicit instantiation declaration/definition issues
// if CPPDEVTK_SHARED is not defined by library users and CppDevTk is shared.
#if (defined(_DLL) && !defined(CPPDEVTK_SHARED))
#	define CPPDEVTK_SHARED
#endif
#if (defined(CPPDEVTK_SHARED) && (!defined(_DLL) || defined(_LIB)))
#	error "CPPDEVTK shared library with static runtime is too dangerous!!!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Safety checks.

#ifdef _MBCS
#	error "_MBCS is not supported!!!"
#endif

#ifndef _UNICODE
//#	error "_UNICODE must be defined (for Qt)!!!"
#	ifdef UNICODE
#		define _UNICODE
#	endif
#endif

#ifdef _MFC_VER
#	error "MFC is not supported!!!"
#endif

#ifdef __cplusplus
#	ifndef _NATIVE_WCHAR_T_DEFINED
#		error "Please set treat wchar_t as a built-in type!!!"
#	endif
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Third party warnings.
#if (CPPDEVTK_DISABLE_THIRD_PARTY_WARNINGS)
#	pragma warning(disable: 4127)	// C4127: conditional expression is constant (from Qt headers)
#	pragma warning(disable: 4512)	// C4512: assignment operator could not be generated (from Qt headers)
#	pragma warning(disable: 4435)	// C4435: 'class1' : Object layout under /vd2 will change due to virtual base 'class2' (from Qt headers)
#	pragma warning(disable: 4464)	// C4464: relative include path contains '..' (from Qt headers)
#	pragma warning(disable: 4946)	// C4946: reinterpret_cast used between related classes (from Qt headers)
#	pragma warning(disable: 4820)	// C4820: 'bytes' bytes padding added after construct 'member_name'
#	pragma warning(disable: 4668)	// C4668: 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
#	pragma warning(disable: 4619)	// C4619: #pragma warning : there is no warning number 'number'
#	pragma warning(disable: 4365)	// C4365: 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
#	pragma warning(disable: 4625)	// C4625: 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
#	pragma warning(disable: 4626)	// C4626: 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
#	pragma warning(disable: 4571)	// C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#	pragma warning(disable: 4628)	// C4628: digraphs not supported with -Ze. Character sequence 'digraph' not interpreted as alternate token for 'char'
#	pragma warning(disable: 4826)	// C4826: Conversion from 'type1 ' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
#	pragma warning(disable: 4711)	// C4711: function 'function' selected for inline expansion
#	pragma warning(disable: 4710)	// C4710: 'function' : function not inlined
#	pragma warning(disable: 4548)	// C4548: expression before comma has no effect; expected expression with side-effect
#	pragma warning(disable: 4371)	// C4371: 'class': layout of class may have changed from a previous version of the compiler due to better packing of member 'member' (from boost headers)

#	if (_MSC_VER >= 1900)	// MSVC 2015 (14.0)
#	pragma warning(disable: 5026)	// C5026: move constructor was implicitly defined as deleted (from Qt headers)
#	pragma warning(disable: 5027)	// C5027: move assignment operator was implicitly defined as deleted (from Qt headers)
#	pragma warning(disable: 5031)	// C5031: #pragma warning(pop): likely mismatch, popping warning state pushed in different file (from boost headers)
#	endif
// TODO: put other third-party warnings safe to disable here.
#endif	// (CPPDEVTK_DISABLE_THIRD_PARTY_WARNINGS)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cppdevtk warnings.
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRT warnings; please see:
// - Deprecated CRT Functions: https://msdn.microsoft.com/en-us/library/ms235384%28v=vs.90%29.aspx
// - Security Enhancements in the CRT: https://msdn.microsoft.com/en-us/library/8ef0s5kh%28v=vs.90%29.aspx
// - _SCL_SECURE_NO_WARNINGS: https://msdn.microsoft.com/en-us/library/aa985974%28v=vs.90%29.aspx

#ifndef _CRT_NONSTDC_NO_DEPRECATE
//#	define _CRT_NONSTDC_NO_DEPRECATE
#endif
#ifndef _CRT_NONSTDC_NO_WARNINGS
//#	define _CRT_NONSTDC_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
//#	define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
//#	define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _SCL_SECURE_NO_WARNINGS
//#	define _SCL_SECURE_NO_WARNINGS
#endif


#pragma warning(disable: 4350)	// C4350: behavior change: 'member1' called instead of 'member2'
//#pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#pragma warning(disable: 4061)	// C4061: enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
#pragma warning(disable: 4686)	// C4686: 'user-defined type' : possible change in behavior, change in UDT return calling convention
// TODO: put other cppdevtk warnings safe to disable here.

#endif	// (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)


/// \endcond


#endif	// CPPDEVTK_CONFIG_COMPILER_MSVC_HPP_INCLUDED_
