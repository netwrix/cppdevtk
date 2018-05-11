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
#define CPPDEVTK_CONFIG_COMPILER_HPP_INCLUDED_


#include "features.hpp"


#ifndef RC_INVOKED
#if (CPPDEVTK_DISABLE_THIRD_PARTY_WARNINGS && defined(_MSC_VER))
#	pragma warning(disable: 4668)	// C4668: 'symbol' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#endif
#endif


#undef __STDC_WANT_LIB_EXT2__
#define __STDC_WANT_LIB_EXT2__ 1


/// \defgroup config_compiler Compiler
/// \ingroup config
/// @{

/// \defgroup config_compiler_supported_compilers Supported Compilers
/// We require C++98 with TR1. Due to OSs/compilers we need to support we can not use C++11/14!
/// \sa https://sourceforge.net/p/predef/wiki/Compilers
/// @{

/// \defgroup config_compiler_supported_compilers_gcc GCC
/// Minimum requirement: GCC 4.0.1
/// @{

/// \brief GCC compiler.
///
/// Example:
/// \code
/// #if (CPPDEVTK_COMPILER_GCC)
/// // gcc specific code
/// #endif
/// \endcode
#if (defined(__GNUC__) && !defined(__clang__))
#	define CPPDEVTK_COMPILER_GCC 1
#else
#	define CPPDEVTK_COMPILER_GCC 0
#endif

/// @}	// config_compiler_supported_compilers_gcc


/// \defgroup config_compiler_supported_compilers_clang Clang
/// Minimum requirement: highest Xcode on Mac OS X 10.7 is 4.6.3: Apple LLVM version 4.2 (clang-425.0.28) (based on LLVM 3.2svn)
/// @{

/// \brief Clang compiler.
///
/// Example:
/// \code
/// #if (CPPDEVTK_COMPILER_CLANG)
/// // clang specific code
/// #endif
/// \endcode
#ifdef __clang__
#	define CPPDEVTK_COMPILER_CLANG 1
#else
#	define CPPDEVTK_COMPILER_CLANG 0
#endif

/// @}	// config_compiler_supported_compilers_clang


/// \defgroup config_compiler_supported_compilers_msvc msvc
/// Minimum requirement: MSVC 2008 (9.0) SP1.
/// @{

/// \brief MSVC compiler.
///
/// Example:
/// \code
/// #if (CPPDEVTK_COMPILER_MSVC)
/// // MSVC specific code
/// #endif
/// \endcode
#ifdef _MSC_VER
#	define CPPDEVTK_COMPILER_MSVC 1
#else
#	define CPPDEVTK_COMPILER_MSVC 0
#endif

/// @}	// config_compiler_supported_compilers_msvc


/// \brief Current compiler.
///
/// Example:
/// \code
/// #if (CPPDEVTK_COMPILER_GCC)
/// // GCC specific code
/// #elif (CPPDEVTK_COMPILER_CLANG)
/// // Clang specific code
/// #elif (CPPDEVTK_COMPILER_MSVC)
/// // MSVC specific code
/// #else
/// #	error "Unsupported compiler!!!"
/// #endif
/// \endcode

// Safety check.
#ifndef RC_INVOKED
#	if (!(CPPDEVTK_COMPILER_GCC || CPPDEVTK_COMPILER_CLANG || CPPDEVTK_COMPILER_MSVC))
#		error "Unsupported compiler!!!"
#	endif
#endif

/// @}	// config_compiler_supported_compilers

/// Stringize char.
#define CPPDEVTK_STRINGIZE(text)	\
	CPPDEVTK_DETAIL_STRINGIZE_HELPER(text)

#define CPPDEVTK_DETAIL_STRINGIZE_HELPER(text)	\
	#text


/// \brief Message preprocessor directive.
///
/// Example:
/// \code
/// CPPDEVTK_COMPILER_MESSAGE("This is a compiler message.");
/// \endcode
/// \attention Must be implemented by each \ref config_compiler_supported_compilers.
#define CPPDEVTK_COMPILER_MESSAGE
#undef CPPDEVTK_COMPILER_MESSAGE

/// \brief Warning preprocessor directive.
///
/// Example:
/// \code
/// CPPDEVTK_COMPILER_WARNING("This is a compiler warning!");
/// \endcode
/// \attention Must be implemented by each \ref config_compiler_supported_compilers.
#define CPPDEVTK_COMPILER_WARNING
#undef CPPDEVTK_COMPILER_WARNING

/// \brief Function long name.
/// \attention Must be implemented by each \ref config_compiler_supported_compilers.
/// \sa C99 \c __func__
#define CPPDEVTK_FUNCTION_LONG_NAME
#undef CPPDEVTK_FUNCTION_LONG_NAME

/// \brief Function short name.
/// \attention Must be implemented by each \ref config_compiler_supported_compilers.
/// \sa C99 \c __func__
#define CPPDEVTK_FUNCTION_SHORT_NAME
#undef CPPDEVTK_FUNCTION_SHORT_NAME

/// \brief Compiler thread local storage (not C++11 thread_local).
/// \attention Must be implemented by each \ref config_compiler_supported_compilers.
#define CPPDEVTK_THREAD
#undef CPPDEVTK_THREAD

/// \brief C++11 final and override or C++03 compiler extensions.
/// \attention Must be implemented by each \ref config_compiler_supported_compilers.
#define CPPDEVTK_FINAL
#undef CPPDEVTK_FINAL
#define CPPDEVTK_OVERRIDE
#undef CPPDEVTK_OVERRIDE

/// \defgroup config_compiler_features Compiler Features
/// Define to 1 if feature is available, 0 otherwise.
/// - CPPDEVTK_HAVE_STDINT_H
/// - CPPDEVTK_HAVE_INTTYPES_H
/// - CPPDEVTK_HAVE_STRINGS_H
/// - CPPDEVTK_HAVE_CXXABI_H
/// - CPPDEVTK_HAVE_ABI___CXA_DEMANGLE
/// - CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG multiple virtual inheritance and covariant return types bug
/// - CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL template explicit instantiation declaration (extern template)
/// - CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS local static variable initialization thread safe
/// - CPPDEVTK_HAVE_TR1_DIR
/// - CPPDEVTK_HAVE_TR1_NS
/// - CPPDEVTK_HAVE_CPP11_STATIC_ASSERT
/// - CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR
/// - CPPDEVTK_HAVE_CPP11_MUTEX
/// - CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE
/// - CPPDEVTK_HAVE_CPP11_THREAD
/// - CPPDEVTK_HAVE_CPP11_FUTURE
/// - CPPDEVTK_HAVE_CPP11_ATOMIC
/// - CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION
/// - CPPDEVTK_HAVE_CPP11_AT_QUICK_EXIT
/// - CPPDEVTK_HAVE_CPP11_TLS
/// - CPPDEVTK_HAVE_CPP11_RVALUE_REFERENCES
/// - CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS
/// - CPPDEVTK_HAVE_CPP11_CONSTEXPR
/// - CPPDEVTK_HAVE_CPP11_DECLTYPE
/// - CPPDEVTK_HAVE_CPP11_TEMPLATE_ALIAS
/// - CPPDEVTK_HAVE_CPP11_LAMBDAS
/// - CPPDEVTK_HAVE_CPP11_NOEXCEPT
/// \attention Must be implemented by each \ref config_compiler_supported_compilers.

// - CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI


/// \cond


#ifndef RC_INVOKED	// for RC4011: identifier truncated to 'identifier'...

#if (CPPDEVTK_COMPILER_GCC)
#	include "compiler/gcc.hpp"
#elif (CPPDEVTK_COMPILER_CLANG)
#	include "compiler/clang.hpp"
#elif (CPPDEVTK_COMPILER_MSVC)
#	include "compiler/msvc.hpp"
#else
#	error "Unsupported compiler!!!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Safety checks.

#ifndef CPPDEVTK_COMPILER_MESSAGE
#	error "Please implement CPPDEVTK_COMPILER_MESSAGE for current compiler!!!"
#endif
#ifndef CPPDEVTK_COMPILER_WARNING
#	error "Please implement CPPDEVTK_COMPILER_WARNING for current compiler!!!"
#endif

#ifndef CPPDEVTK_FUNCTION_LONG_NAME
#	error "Please define CPPDEVTK_FUNCTION_LONG_NAME for current compiler!!!"
#endif
#ifndef CPPDEVTK_FUNCTION_SHORT_NAME
#	error "Please define CPPDEVTK_FUNCTION_SHORT_NAME for current compiler!!!"
#endif

#ifndef CPPDEVTK_THREAD
#	error "Please define CPPDEVTK_THREAD for current compiler!!!"
#endif

#ifndef CPPDEVTK_FINAL
#	error "Please define CPPDEVTK_FINAL for current compiler!!!"
#endif
#ifndef CPPDEVTK_OVERRIDE
#	error "Please define CPPDEVTK_OVERRIDE for current compiler!!!"
#endif

#ifndef CPPDEVTK_HAVE_STDINT_H
#	error "Please define CPPDEVTK_HAVE_STDINT_H properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_INTTYPES_H
#	error "Please define CPPDEVTK_HAVE_INTTYPES_H properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_STRINGS_H
#	error "Please define CPPDEVTK_HAVE_STRINGS_H properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CXXABI_H
#	error "Please define CPPDEVTK_HAVE_CXXABI_H properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_ABI___CXA_DEMANGLE
#	error "Please define CPPDEVTK_HAVE_ABI___CXA_DEMANGLE properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG
#	error "Please define CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL
#	error "Please define CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS
#	error "Please define CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS properly for current compiler!!!"
#endif

#ifndef CPPDEVTK_HAVE_TR1_DIR
#	error "Please define CPPDEVTK_HAVE_TR1_DIR properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_TR1_NS
#	error "Please define CPPDEVTK_HAVE_TR1_NS properly for current compiler!!!"
#endif

#ifndef CPPDEVTK_HAVE_CPP11_STATIC_ASSERT
#	error "Please define CPPDEVTK_HAVE_CPP11_STATIC_ASSERT properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR
#	error "Please define CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_MUTEX
#	error "Please define CPPDEVTK_HAVE_CPP11_MUTEX properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE
#	error "Please define CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_THREAD
#	error "Please define CPPDEVTK_HAVE_CPP11_THREAD properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_FUTURE
#	error "Please define CPPDEVTK_HAVE_CPP11_FUTURE properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_ATOMIC
#	error "Please define CPPDEVTK_HAVE_CPP11_ATOMIC properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION
#	error "Please define CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_AT_QUICK_EXIT
#	error "Please define CPPDEVTK_HAVE_CPP11_AT_QUICK_EXIT properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_TLS
#	error "Please define CPPDEVTK_HAVE_CPP11_TLS properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_RVALUE_REFERENCES
#	error "Please define CPPDEVTK_HAVE_CPP11_RVALUE_REFERENCES properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS
#	error "Please define CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_CONSTEXPR
#	error "Please define CPPDEVTK_HAVE_CPP11_CONSTEXPR properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_DECLTYPE
#	error "Please define CPPDEVTK_HAVE_CPP11_DECLTYPE properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_TEMPLATE_ALIAS
#	error "Please define CPPDEVTK_HAVE_CPP11_TEMPLATE_ALIAS properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_LAMBDAS
#	error "Please define CPPDEVTK_HAVE_CPP11_LAMBDAS properly for current compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_CPP11_NOEXCEPT
#	error "Please define CPPDEVTK_HAVE_CPP11_NOEXCEPT properly for current compiler!!!"
#endif

#ifndef CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI
#	error "Please define CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI properly for current compiler!!!"
#endif
#ifdef __cplusplus
#	if (!CPPDEVTK_DETAIL_COMPILER_ENABLE_RTTI)
#		error "Please enable RTTI for current compiler!!!"
#	endif
#endif


#endif	// RC_INVOKED


/// \endcond


#if (CPPDEVTK_HAVE_TR1_DIR)
#	define CPPDEVTK_TR1_DIR tr1/
#else
#	define CPPDEVTK_TR1_DIR
#endif

/// \brief Helper macro to include TR1 headers.
///
/// Example:
/// \code
/// #include CPPDEVTK_TR1_HEADER(type_traits)
/// \endcode
#define CPPDEVTK_TR1_HEADER(header)	\
	CPPDEVTK_DETAIL_PUT_IN_ANGLE_BRACKETS(CPPDEVTK_DETAIL_TR1_HEADER_HELPER(CPPDEVTK_TR1_DIR, header))


#define CPPDEVTK_DETAIL_PUT_IN_ANGLE_BRACKETS(text)	\
	<text>

#define CPPDEVTK_DETAIL_TR1_HEADER_HELPER(tr1dir, tr1header)	\
	tr1dir tr1header


/// \brief Helper macro for TR1 namespace.
///
/// Example:
/// \code
/// CPPDEVTK_TR1_NS::shared_ptr<MyClass> ptr;
/// \endcode
#if (CPPDEVTK_HAVE_TR1_NS)
#	define CPPDEVTK_TR1_NS ::std::tr1
#else
#	define CPPDEVTK_TR1_NS ::std
#endif


#if (CPPDEVTK_HAVE_CPP11_CONSTEXPR)
#	define CPPDEVTK_CONSTEXPR constexpr
#else
#	define CPPDEVTK_CONSTEXPR
#endif

#if (CPPDEVTK_HAVE_CPP11_NOEXCEPT)
#	define CPPDEVTK_NOEXCEPT noexcept
#else
#	define CPPDEVTK_NOEXCEPT throw()
#endif

/// @}	// config_compiler


#endif	// CPPDEVTK_CONFIG_COMPILER_HPP_INCLUDED_
