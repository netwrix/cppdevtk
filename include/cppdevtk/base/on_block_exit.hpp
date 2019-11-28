/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_BASE_ON_BLOCK_EXIT_HPP_INCLUDED_
#define CPPDEVTK_BASE_ON_BLOCK_EXIT_HPP_INCLUDED_


#include "config.hpp"
#include "logger.hpp"
#include "cassert.hpp"
#include "exception.hpp"
#include "stdexcept.hpp"
#include "unused.hpp"

#include <boost/scope_exit.hpp>

#include <exception>
#include <stdexcept>


#define CPPDEVTK_ON_BLOCK_EXIT_BEGIN(void_or_seq)	\
	BOOST_SCOPE_EXIT(void_or_seq)

#define CPPDEVTK_ON_BLOCK_EXIT_TPL_BEGIN(void_or_seq)	\
	BOOST_SCOPE_EXIT_TPL(void_or_seq)

#define CPPDEVTK_ON_BLOCK_EXIT_END BOOST_SCOPE_EXIT_END


#define CPPDEVTK_ON_BLOCK_EXIT_CATCH	\
		catch (const ::cppdevtk::base::LogicException& exc) {	\
			CPPDEVTK_LOG_ERROR("CPPDEVTK_ON_BLOCK_EXIT: caught ::cppdevtk::base::LogicException: " << ::cppdevtk::base::Exception::GetDetailedInfo(exc));	\
			::cppdevtk::base::SuppressUnusedWarning(exc);	\
			CPPDEVTK_ASSERT(0 && "CPPDEVTK_ON_BLOCK_EXIT: caught ::cppdevtk::base::LogicException");	\
		}	\
		catch (const ::std::logic_error& exc) {	\
			CPPDEVTK_LOG_ERROR("CPPDEVTK_ON_BLOCK_EXIT: caught ::std::logic_error: " << ::cppdevtk::base::Exception::GetDetailedInfo(exc));	\
			::cppdevtk::base::SuppressUnusedWarning(exc);	\
			CPPDEVTK_ASSERT(0 && "CPPDEVTK_ON_BLOCK_EXIT: caught ::std::logic_error");	\
		}	\
		catch (const ::std::exception& exc) {	\
			CPPDEVTK_LOG_WARN("CPPDEVTK_ON_BLOCK_EXIT: absorbing caught ::std::exception: " << ::cppdevtk::base::Exception::GetDetailedInfo(exc));	\
			::cppdevtk::base::SuppressUnusedWarning(exc);	\
		}	\
		catch (...) {	\
			CPPDEVTK_LOG_WARN("CPPDEVTK_ON_BLOCK_EXIT: absorbing caught unknown exception");	\
		}


#endif	// CPPDEVTK_BASE_ON_BLOCK_EXIT_HPP_INCLUDED_
