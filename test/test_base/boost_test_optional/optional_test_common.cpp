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
#include <cppdevtk/base/cassert.hpp>
#include "optional_test_common.hpp"

#include <iostream>
#include <stdexcept>


namespace boost {


int test_errors_counter = 0;


void ReportError(const char* msg, const char* file, int line, const char* func_name, bool is_msg) {
    ++test_errors_counter;
    std::cerr << file << "(" << line << "): ";

    if( is_msg )
        std::cerr << msg;
    else
        std::cerr << "test " << msg << " failed";

    //if( func_name != "(unknown)" )
        std::cerr << " in function: '" << func_name << "'";

    std::cerr << std::endl;
}


}


int  X::count           = 0 ;
bool X::pending_copy    = false ;
bool X::pending_dtor    = false ;
bool X::pending_assign  = false ;
bool X::throw_on_copy   = false ;
bool X::throw_on_assign = false ;
