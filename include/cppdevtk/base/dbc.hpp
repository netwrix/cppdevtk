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


// NOTE: No include guard.
//#ifndef CPPDEVTK_BASE_DBC_HPP_INCLUDED_
//#define CPPDEVTK_BASE_DBC_HPP_INCLUDED_



#include "config.hpp"
#include "dbc_exceptions.hpp"
#include "stdexcept.hpp"
#include "cassert.hpp"

#include <cstddef>


#define CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED 1


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup dbc Design By Contract
/// Design by contract minimal support
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup dbc_macros Design By Contract Macros
/// @{


// doxygen workarounds:

#undef CPPDEVTK_DBC_CHECK_PRECONDITION
#define CPPDEVTK_DBC_CHECK_PRECONDITION(expr) ((void)0)

#undef CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG
#define CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(expr, msg) ((void)0)

#undef CPPDEVTK_DBC_CHECK_POSTCONDITION
#define CPPDEVTK_DBC_CHECK_POSTCONDITION(expr) ((void)0)

#undef CPPDEVTK_DBC_CHECK_POSTCONDITION_W_MSG
#define CPPDEVTK_DBC_CHECK_POSTCONDITION_W_MSG(expr, msg) ((void)0)

#undef CPPDEVTK_DBC_CHECK_INVARIANT
#define CPPDEVTK_DBC_CHECK_INVARIANT(expr) ((void)0)

#undef CPPDEVTK_DBC_CHECK_INVARIANT_W_MSG
#define CPPDEVTK_DBC_CHECK_INVARIANT_W_MSG(expr, msg) ((void)0)

#undef CPPDEVTK_DBC_CHECK_STATE
#define CPPDEVTK_DBC_CHECK_STATE(expr, expectedState, actualState) ((void)0)

#undef CPPDEVTK_DBC_CHECK_ARGUMENT
#define CPPDEVTK_DBC_CHECK_ARGUMENT(expr, msg) ((void)0)

#undef CPPDEVTK_DBC_CHECK_DOMAIN
#define CPPDEVTK_DBC_CHECK_DOMAIN(expr, msg) ((void)0)

#undef CPPDEVTK_DBC_CHECK_LENGTH
#define CPPDEVTK_DBC_CHECK_LENGTH(expr, msg) ((void)0)

#undef CPPDEVTK_DBC_CHECK_IN_RANGE
#define CPPDEVTK_DBC_CHECK_IN_RANGE(expr, msg) ((void)0)

#undef CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT
#define CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(arg) ((void)0)

#undef CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT
#define CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(expr, argName) ((void)0)

#undef CPPDEVTK_DBC_CHECK_NON_NULL_POINTER
#define CPPDEVTK_DBC_CHECK_NON_NULL_POINTER(ptr) ((void)0)


/// @}	// dbc_macros
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @}	// dbc
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/// \cond


#undef CPPDEVTK_DBC_CHECK_PRECONDITION
#undef CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG
#if (!CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_PRECONDITION(expr)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_LOGIC_EXCEPTION("precondition failure: " #expr);	\
				}	\
			}	\
			while (0)
		
#		define CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(expr, msg)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_LOGIC_EXCEPTION("precondition failure: message: " msg "; expression: " #expr);	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_PRECONDITION(expr)	\
			CPPDEVTK_ASSERT((expr) && "precondition failure")
		
#		define CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(expr, msg)	\
			CPPDEVTK_ASSERT((expr) && ("precondition failure: " msg))
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_PRECONDITION(expr)	\
		CPPDEVTK_ASSERT((expr) && "precondition failure")
	
#	define CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(expr, msg)	\
		CPPDEVTK_ASSERT((expr) && ("precondition failure: " msg))
#else
#	define CPPDEVTK_DBC_CHECK_PRECONDITION(expr)	\
		((void)0)

#	define CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(expr, msg)	\
		((void)0)
#endif
#endif


#undef CPPDEVTK_DBC_CHECK_POSTCONDITION
#undef CPPDEVTK_DBC_CHECK_POSTCONDITION_W_MSG
#if (!CPPDEVTK_DBC_DISABLE_CHECK_POSTCONDITION)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_POSTCONDITION(expr)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_LOGIC_EXCEPTION("postcondition failure: " #expr);	\
				}	\
			}	\
			while (0)
		
#		define CPPDEVTK_DBC_CHECK_POSTCONDITION_W_MSG(expr, msg)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_LOGIC_EXCEPTION("postcondition failure: message: " msg "; expression: " #expr);	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_POSTCONDITION(expr)	\
			CPPDEVTK_ASSERT((expr) && "postcondition failure")
		
#		define CPPDEVTK_DBC_CHECK_POSTCONDITION_W_MSG(expr, msg)	\
			CPPDEVTK_ASSERT((expr) && ("postcondition failure: " msg))
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_POSTCONDITION(expr)	\
		CPPDEVTK_ASSERT((expr) && "postcondition failure")
	
#	define CPPDEVTK_DBC_CHECK_POSTCONDITION_W_MSG(expr, msg)	\
		CPPDEVTK_ASSERT((expr) && ("postcondition failure: " msg))
#else
#	define CPPDEVTK_DBC_CHECK_POSTCONDITION(expr)	\
		((void)0)

#	define CPPDEVTK_DBC_CHECK_POSTCONDITION_W_MSG(expr, msg)	\
		((void)0)
#endif
#endif


#undef CPPDEVTK_DBC_CHECK_INVARIANT
#undef CPPDEVTK_DBC_CHECK_INVARIANT_W_MSG
#if (!CPPDEVTK_DBC_DISABLE_CHECK_INVARIANT)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_INVARIANT(expr)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_LOGIC_EXCEPTION("invariant failure: " #expr);	\
				}	\
			}	\
			while (0)
		
#		define CPPDEVTK_DBC_CHECK_INVARIANT_W_MSG(expr, msg)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_LOGIC_EXCEPTION("invariant failure: message: " msg "; expression: " #expr);	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_INVARIANT(expr)	\
			CPPDEVTK_ASSERT((expr) && "invariant failure")
		
#		define CPPDEVTK_DBC_CHECK_INVARIANT_W_MSG(expr, msg)	\
			CPPDEVTK_ASSERT((expr) && ("invariant failure: " msg))
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_INVARIANT(expr)	\
		CPPDEVTK_ASSERT((expr) && "invariant failure")
	
#	define CPPDEVTK_DBC_CHECK_INVARIANT_W_MSG(expr, msg)	\
		CPPDEVTK_ASSERT((expr) && ("invariant failure: " msg))
#else
#	define CPPDEVTK_DBC_CHECK_INVARIANT(expr)	\
		((void)0)

#	define CPPDEVTK_DBC_CHECK_INVARIANT_W_MSG(expr, msg)	\
		((void)0)
#endif
#endif


#undef CPPDEVTK_DBC_CHECK_STATE
#if (!CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_STATE(expr, expectedState, actualState)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_INVALID_STATE_EXCEPTION((expectedState), (actualState), CPPDEVTK_FUNCTION_LONG_NAME);	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_STATE(expr, expectedState, actualState)	\
			CPPDEVTK_ASSERT((expr) && ("Invalid state for requested operation:"	\
			" expectedState: " expectedState "; actualState: " actualState "; requestedOperation: " CPPDEVTK_FUNCTION_LONG_NAME))
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_STATE(expr, expectedState, actualState)	\
		CPPDEVTK_ASSERT((expr) && ("Invalid state for requested operation:"	\
		" expectedState: " expectedState "; actualState: " actualState "; requestedOperation: " CPPDEVTK_FUNCTION_LONG_NAME))
#else
#	define CPPDEVTK_DBC_CHECK_STATE(expr, expectedState, actualState)	\
		((void)0)
#endif
#endif


#undef CPPDEVTK_DBC_CHECK_ARGUMENT
#if (!CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_ARGUMENT(expr, msg)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION(msg);	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_ARGUMENT(expr, msg)	\
			CPPDEVTK_ASSERT((expr) && ("invalid argument: " msg))
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_ARGUMENT(expr, msg)	\
		CPPDEVTK_ASSERT((expr) && ("invalid argument: " msg))
#else
#	define CPPDEVTK_DBC_CHECK_ARGUMENT(expr, msg)	\
		((void)0)
#endif
#endif


#undef CPPDEVTK_DBC_CHECK_DOMAIN
#if (!CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_DOMAIN(expr, msg)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_DOMAIN_EXCEPTION(msg);	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_DOMAIN(expr, msg)	\
			CPPDEVTK_ASSERT((expr) && ("domain error: " msg))
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_DOMAIN(expr, msg)	\
		CPPDEVTK_ASSERT((expr) && ("domain error: " msg))
#else
#	define CPPDEVTK_DBC_CHECK_DOMAIN(expr, msg)	\
		((void)0)
#endif
#endif


#undef CPPDEVTK_DBC_CHECK_LENGTH
#if (!CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_LENGTH(expr, msg)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_LENGTH_EXCEPTION(msg);	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_LENGTH(expr, msg)	\
			CPPDEVTK_ASSERT((expr) && ("length error: " msg))
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_LENGTH(expr, msg)	\
		CPPDEVTK_ASSERT((expr) && ("length error: " msg))
#else
#	define CPPDEVTK_DBC_CHECK_LENGTH(expr, msg)	\
		((void)0)
#endif
#endif


#undef CPPDEVTK_DBC_CHECK_IN_RANGE
#if (!CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_IN_RANGE(expr, msg)	\
			do {	\
				if (!(expr)) {	\
					throw CPPDEVTK_OUT_OF_RANGE_EXCEPTION(msg);	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_IN_RANGE(expr, msg)	\
			CPPDEVTK_ASSERT((expr) && ("out of range: " msg))
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_IN_RANGE(expr, msg)	\
		CPPDEVTK_ASSERT((expr) && ("out of range: " msg))
#else
#	define CPPDEVTK_DBC_CHECK_IN_RANGE(expr, msg)	\
		((void)0)
#endif
#endif


#undef CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT
#if (!CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(arg)	\
			do {	\
				if ((arg) == NULL) {	\
					throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION(#arg);	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(arg)	\
			CPPDEVTK_ASSERT(((arg) != NULL) && "null argument")
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(arg)	\
		CPPDEVTK_ASSERT(((arg) != NULL) && "null argument")
#else
#	define CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(arg)	\
		((void)0)
#endif
#endif


#undef CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT
#if (!CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(expr, argName)	\
			do {	\
				if (expr) {	\
					throw CPPDEVTK_EMPTY_ARGUMENT_EXCEPTION(argName);	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(expr, argName)	\
			CPPDEVTK_ASSERT(!(expr) && ("empty argument: " argName))
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(expr, argName)	\
		CPPDEVTK_ASSERT(!(expr) && ("empty argument: " argName))
#else
#	define CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(expr, argName)	\
		((void)0)
#endif
#endif


#undef CPPDEVTK_DBC_CHECK_NON_NULL_POINTER
#if (CPPDEVTK_ENABLE_DBC)
#	if (!CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE)
#		define CPPDEVTK_DBC_CHECK_NON_NULL_POINTER(ptr)	\
			do {	\
				if ((ptr) == NULL) {	\
					throw CPPDEVTK_NULL_POINTER_EXCEPTION();	\
				}	\
			}	\
			while (0)
#	else
#		define CPPDEVTK_DBC_CHECK_NON_NULL_POINTER(ptr)	\
			CPPDEVTK_ASSERT(((ptr) != NULL) && "NULL pointer")
#	endif
#else
#if (CPPDEVTK_DETAIL_DBC_ASSERT_IF_DISABLED)
#	define CPPDEVTK_DBC_CHECK_NON_NULL_POINTER(ptr)	\
		CPPDEVTK_ASSERT(((ptr) != NULL) && "NULL pointer")
#else
#	define CPPDEVTK_DBC_CHECK_NON_NULL_POINTER(ptr)	\
		((void)0)
#endif
#endif


/// \endcond


//#endif	// CPPDEVTK_BASE_DBC_HPP_INCLUDED_
