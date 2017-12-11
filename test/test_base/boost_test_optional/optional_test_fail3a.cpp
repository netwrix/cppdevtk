// Copyright (C) 2003, Fernando Luis Cacciola Carballal.
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


// NOTE: taken from boost 1.55.0 and adapted for cppdevtk


#include <cppdevtk/base/optional.hpp>

#include<string>

//
// THIS TEST SHOULD FAIL TO COMPILE
//
#if 0	// cppdevtk test passed
void test_no_unsupported_conversion()
{
  ::cppdevtk::base::Optional<int> opt1(1) ;
  ::cppdevtk::base::Optional< std::string > opt2( opt1 ) ; // Cannot convert from "int" to "std::string"
}
#endif
