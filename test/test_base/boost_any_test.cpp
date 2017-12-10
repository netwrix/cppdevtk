// what:  unit tests for variant type boost::any
// who:   contributed by Kevlin Henney
// when:  July 2001, 2013, 2014
// where: tested with BCC 5.5, MSVC 6.0, and g++ 2.95


// NOTE: taken from boost 1.65.1 and adapted for cppdevtk


#include <cppdevtk/base/any.hpp>
#include <cppdevtk/base/typeinfo.hpp>

#include "boost_any_test.hpp"

#include <cstdlib>
#include <string>
#include <vector>
#include <utility>


using ::cppdevtk::base::Any;
using ::cppdevtk::base::BadAnyCastException;


namespace any_tests // test suite
{
    void test_default_ctor();
    void test_converting_ctor();
    void test_copy_ctor();
    void test_copy_assign();
    void test_converting_assign();
    void test_bad_cast();
    void test_swap();
    void test_null_copying();
    void test_cast_to_reference();
    void test_with_array();
    void test_with_func();
    void test_clear();
    void test_vectors();
    void test_addressof();

    const test_case test_cases[] =
    {
        { "default construction",           test_default_ctor      },
        { "single argument construction",   test_converting_ctor   },
        { "copy construction",              test_copy_ctor         },
        { "copy assignment operator",       test_copy_assign       },
        { "converting assignment operator", test_converting_assign },
        { "failed custom keyword cast",     test_bad_cast          },
        { "swap member function",           test_swap              },
        { "copying operations on a null",   test_null_copying      },
        { "cast to reference types",        test_cast_to_reference },
        { "storing an array inside",        test_with_array        },
        { "implicit cast of returned value",test_with_func         },
        { "clear() methods",                test_clear             },
        { "testing with vectors",           test_vectors           },
        { "class with operator&()",         test_addressof         }
    };

    const test_case_iterator begin = test_cases;
    const test_case_iterator end =
        test_cases + (sizeof test_cases / sizeof *test_cases);

    

    struct copy_counter
    {

    public:

        copy_counter() {}
        copy_counter(const copy_counter&) { ++count; }
        copy_counter& operator=(const copy_counter&) { ++count; return *this; }
        static int get_count() { return count; }

    private:

        static int count;

    };

    int copy_counter::count = 0;
}

namespace any_tests // test definitions
{
    void test_default_ctor()
    {
        const Any value;

        check_true(!value.HasValue(), "empty");
        check_null(::cppdevtk::base::AnyCast<int>(&value), "AnyCast<int>");
        check_equal(value.GetTypeInfo(), typeid(void), "type");
    }

    void test_converting_ctor()
    {
        std::string text = "test message";
        Any value = text;

        check_false(!value.HasValue(), "empty");
        check_equal(value.GetTypeInfo(), typeid(std::string), "type");
        check_null(::cppdevtk::base::AnyCast<int>(&value), "AnyCast<int>");
        check_non_null(::cppdevtk::base::AnyCast<std::string>(&value), "AnyCast<std::string>");
        check_equal(
            ::cppdevtk::base::AnyCast<std::string>(value), text,
            "comparing cast copy against original text");
        check_unequal(
            ::cppdevtk::base::AnyCast<std::string>(&value), &text,
            "comparing address in copy against original text");
    }

    void test_copy_ctor()
    {
        std::string text = "test message";
        Any original = text, copy = original;

        check_false(!copy.HasValue(), "empty");
        check_equal(original.GetTypeInfo(), copy.GetTypeInfo(), "type");
        check_equal(
            ::cppdevtk::base::AnyCast<std::string>(original), ::cppdevtk::base::AnyCast<std::string>(copy),
            "comparing cast copy against original");
        check_equal(
            text, ::cppdevtk::base::AnyCast<std::string>(copy),
            "comparing cast copy against original text");
        check_unequal(
            ::cppdevtk::base::AnyCast<std::string>(&original),
            ::cppdevtk::base::AnyCast<std::string>(&copy),
            "comparing address in copy against original");
    }

    void test_copy_assign()
    {
        std::string text = "test message";
        Any original = text, copy;
        Any * assign_result = &(copy = original);

        check_false(!copy.HasValue(), "empty");
        check_equal(original.GetTypeInfo(), copy.GetTypeInfo(), "type");
        check_equal(
            ::cppdevtk::base::AnyCast<std::string>(original), ::cppdevtk::base::AnyCast<std::string>(copy),
            "comparing cast copy against cast original");
        check_equal(
            text, ::cppdevtk::base::AnyCast<std::string>(copy),
            "comparing cast copy against original text");
        check_unequal(
            ::cppdevtk::base::AnyCast<std::string>(&original),
            ::cppdevtk::base::AnyCast<std::string>(&copy),
            "comparing address in copy against original");
        check_equal(assign_result, &copy, "address of assignment result");
    }

    void test_converting_assign()
    {
        std::string text = "test message";
        Any value;
        Any * assign_result = &(value = text);

        check_false(!value.HasValue(), "type");
        check_equal(value.GetTypeInfo(), typeid(std::string), "type");
        check_null(::cppdevtk::base::AnyCast<int>(&value), "AnyCast<int>");
        check_non_null(::cppdevtk::base::AnyCast<std::string>(&value), "AnyCast<std::string>");
        check_equal(
            ::cppdevtk::base::AnyCast<std::string>(value), text,
            "comparing cast copy against original text");
        check_unequal(
            ::cppdevtk::base::AnyCast<std::string>(&value),
            &text,
            "comparing address in copy against original text");
        check_equal(assign_result, &value, "address of assignment result");
    }

    void test_bad_cast()
    {
        std::string text = "test message";
        Any value = text;

        TEST_CHECK_THROW(
            ::cppdevtk::base::AnyCast<const char *>(value),
            BadAnyCastException,
            "AnyCast to incorrect type");
    }

    void test_swap()
    {
        std::string text = "test message";
        Any original = text, swapped;
        std::string * original_ptr = ::cppdevtk::base::AnyCast<std::string>(&original);
        original.Swap(swapped);
        Any * swap_result = &original;

        check_true(!original.HasValue(), "empty on original");
        check_false(!swapped.HasValue(), "empty on swapped");
        check_equal(swapped.GetTypeInfo(), typeid(std::string), "type");
        check_equal(
            text, ::cppdevtk::base::AnyCast<std::string>(swapped),
            "comparing swapped copy against original text");
        check_non_null(original_ptr, "address in pre-swapped original");
        check_equal(
            original_ptr,
            ::cppdevtk::base::AnyCast<std::string>(&swapped),
            "comparing address in swapped against original");
        check_equal(swap_result, &original, "address of swap result");

        Any copy1 = copy_counter();
        Any copy2 = copy_counter();
        int count = copy_counter::get_count();
        swap(copy1, copy2);
        check_equal(count, copy_counter::get_count(), "checking that free swap doesn't make any copies.");
    }

    void test_null_copying()
    {
        const Any null;
        Any copied = null, assigned;
        assigned = null;

        check_true(!null.HasValue(), "empty on null");
        check_true(!copied.HasValue(), "empty on copied");
        check_true(!assigned.HasValue(), "empty on copied");
    }

    void test_cast_to_reference()
    {
        Any a(137);
        const Any b(a);

        int &                ra    = ::cppdevtk::base::AnyCast<int &>(a);
        int const &          ra_c  = ::cppdevtk::base::AnyCast<int const &>(a);
        int volatile &       ra_v  = ::cppdevtk::base::AnyCast<int volatile &>(a);
        int const volatile & ra_cv = ::cppdevtk::base::AnyCast<int const volatile&>(a);

        check_true(
            &ra == &ra_c && &ra == &ra_v && &ra == &ra_cv,
            "cv references to same obj");

        int const &          rb_c  = ::cppdevtk::base::AnyCast<int const &>(b);
        int const volatile & rb_cv = ::cppdevtk::base::AnyCast<int const volatile &>(b);

        check_true(&rb_c == &rb_cv, "cv references to copied const obj");
        check_true(&ra != &rb_c, "copies hold different objects");

        ++ra;
        int incremented = ::cppdevtk::base::AnyCast<int>(a);
        check_true(incremented == 138, "increment by reference changes value");

        TEST_CHECK_THROW(
            ::cppdevtk::base::AnyCast<char &>(a),
            BadAnyCastException,
            "AnyCast to incorrect reference type");

        TEST_CHECK_THROW(
            ::cppdevtk::base::AnyCast<const char &>(b),
            BadAnyCastException,
            "AnyCast to incorrect const reference type");
    }

    void test_with_array()
    {
		// FIXME
		/*
        Any value1("Char array");
        Any value2;
        value2 = "Char array";

        check_false(!value1.HasValue(), "type");
        check_false(!value2.HasValue(), "type");

        check_equal(value1.GetTypeInfo(), typeid(const char*), "type");
        check_equal(value2.GetTypeInfo(), typeid(const char*), "type");
        
        check_non_null(::cppdevtk::base::AnyCast<const char*>(&value1), "AnyCast<const char*>");
        check_non_null(::cppdevtk::base::AnyCast<const char*>(&value2), "AnyCast<const char*>");
        */
    }

    const std::string& returning_string1() 
    {
        static const std::string ret("foo"); 
        return ret;
    }

    std::string returning_string2() 
    {
        static const std::string ret("foo"); 
        return ret;
    }

    void test_with_func()
    {
        std::string s;
        s = ::cppdevtk::base::AnyCast<std::string>(returning_string1());
        s = ::cppdevtk::base::AnyCast<const std::string&>(returning_string1());

        s = ::cppdevtk::base::AnyCast<std::string>(returning_string2());
        s = ::cppdevtk::base::AnyCast<const std::string&>(returning_string2());
    }

    
    void test_clear()
    {
        std::string text = "test message";
        Any value = text;

        check_false(!value.HasValue(), "empty");
        
        value.Reset();
        check_true(!value.HasValue(), "non-empty after clear");

        value.Reset();
        check_true(!value.HasValue(), "non-empty after second clear");

        value = text;
        check_false(!value.HasValue(), "empty");
        
        value.Reset();
        check_true(!value.HasValue(), "non-empty after clear");
    }

    // Following tests cover the case from #9462
    // https://svn.boost.org/trac/boost/ticket/9462
    Any makeVec() 
    {
        return std::vector<int>(100 /*size*/, 7 /*value*/);
    }

    void test_vectors() 
    {
        const std::vector<int>& vec = ::cppdevtk::base::AnyCast<std::vector<int> >(makeVec()); 
        check_equal(vec.size(), 100u, "size of vector extracted from Any"); 
        check_equal(vec.back(), 7, "back value of vector extracted from Any");
        check_equal(vec.front(), 7, "front value of vector extracted from Any");

        std::vector<int> vec1 = ::cppdevtk::base::AnyCast<std::vector<int> >(makeVec()); 
        check_equal(vec1.size(), 100u, "size of second vector extracted from Any"); 
        check_equal(vec1.back(), 7, "back value of second vector extracted from Any");
        check_equal(vec1.front(), 7, "front value of second vector extracted from Any");

    }

    template<typename T>
    class class_with_address_op {
    public:
        class_with_address_op(const T* p)
            : ptr(p)
        {}

	    const T** operator &() {
            return &ptr;
        }

        const T* get() const {
            return ptr;
        }

    private:
        const T* ptr;
    };

    void test_addressof()
    {
        int val = 10;
        const int* ptr = &val;
        class_with_address_op<int> obj(ptr);
        Any test_val(obj);
        
        // FIXME
        //class_with_address_op<int> returned_obj = ::cppdevtk::base::AnyCast<class_with_address_op<int> >(test_val);
        //check_equal(&val, returned_obj.get(), "AnyCast incorrectly works with type that has operator&(): addresses differ");
        
        // FIXME
        //check_true(!!::cppdevtk::base::AnyCast<class_with_address_op<int> >(&test_val), "AnyCast incorrectly works with type that has operator&()");
    }

}

// Copyright Kevlin Henney, 2000, 2001. All rights reserved.
// Copyright Antony Polukhin, 2013-2017.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
