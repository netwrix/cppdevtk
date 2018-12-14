/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_BASE_OPTIONAL_HPP_INCLUDED_
#define CPPDEVTK_BASE_OPTIONAL_HPP_INCLUDED_


#include "config.hpp"
#include "bad_optional_access_exception.hpp"
#include "dbc.hpp"
#include "unused.hpp"
#include "logger.hpp"
#include "exception.hpp"
#include "cassert.hpp"
#include "static_assert.hpp"

#include <cstddef>
#include <new>
#include <algorithm>	// swap(), C++98
#include <utility>	// swap(), C++11
#include CPPDEVTK_TR1_HEADER(type_traits)


namespace cppdevtk {
namespace base {


namespace detail {


struct CPPDEVTK_BASE_API NoneHelper{};


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class used to indicate \c Optional type with uninitialized state.
/// \remarks
/// - must be a non-aggregate LiteralType and cannot have a default constructor or an initializer-list constructor.
/// - It must have a constructor that takes some implementation-defined literal type.
/// \sa <a href="http://en.cppreference.com/w/cpp/utility/optional/nullopt_t">C++17 nullopt_t</a>
typedef int detail::NoneHelper::*NullOptT;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A constant of type \c NullOptT that is used to indicate \c Optional type with uninitialized state.
/// \sa <a href="http://en.cppreference.com/w/cpp/utility/optional/nullopt">C++17 nullopt</a>
static const NullOptT kNullOpt = (static_cast<NullOptT>(NULL)) ;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class that manages an optional contained value, i.e. a value that may or may not be present.
/// \attention
/// - C++ 17 std says that if an ::std::optional<T> contains a value, the value is guaranteed to be allocated
/// as part of the optional object footprint, i.e. no dynamic memory allocation ever takes place.
/// Our implementation currently use dynamic memory allocation (need for public API and no time for proper implementation...)!
/// - Reference types are not supported (C++ 17 ::std::optional does not support references, ::boost::optional does)!
/// \sa
/// - <a href="http://en.cppreference.com/w/cpp/utility/optional">C++17 optional</a>
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4562.html#optional.object">C++ Extensions for Library Fundamentals, Version 2, 5.3 optional for object types</a>
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3793.html">A proposal to add a utility class to represent optional objects (Revision 5)</a>
/// - <a href="http://www.boost.org/doc/libs/1_65_1/libs/optional/doc/html/boost_optional/reference/header__boost_optional_optional_hpp_/header_optional_optional_values.html">Boost Optional Values</a>
template <typename TValue>
class Optional {
public:
	CPPDEVTK_STATIC_ASSERT(!CPPDEVTK_TR1_NS::is_reference<TValue>::value);
	CPPDEVTK_STATIC_ASSERT((!CPPDEVTK_TR1_NS::is_same<TValue, NullOptT>::value));
	
	
	typedef TValue ValueType;
	typedef void (*UnspecifiedBoolType)();
	
	
	Optional() CPPDEVTK_NOEXCEPT;	///< Constructs an object that does not contain a value.
	Optional(NullOptT) CPPDEVTK_NOEXCEPT;	///< Constructs an object that does not contain a value; not explicit!
	
	Optional(const TValue& value);	///< Constructs an object initialized with \a value; not explicit!
	
	/// If \a cond is \c true constructs an object initialized with \a value otherwise constructs an uninitialized object.
	Optional(bool cond, const TValue& value);
	
	template <typename TConvertibleValue>
	explicit Optional(const Optional<TConvertibleValue>& other);	///< Converting copy constructor.
	
	Optional(const Optional& other);	///< Copy constructor
	
	
	~Optional() CPPDEVTK_NOEXCEPT;
	
	
	Optional& operator=(NullOptT nullOpt) CPPDEVTK_NOEXCEPT;
	Optional& operator=(const TValue& value);
	template <typename TConvertibleValue>
	Optional& operator=(const Optional<TConvertibleValue>& other);
	Optional& operator=(const Optional& other);
	
	
	void Swap(Optional& other) CPPDEVTK_NOEXCEPT;
	void Reset() CPPDEVTK_NOEXCEPT;
	void Reset(const TValue& value);
	
	bool HasValue() const CPPDEVTK_NOEXCEPT;
	operator UnspecifiedBoolType() const CPPDEVTK_NOEXCEPT;
	bool operator!() const CPPDEVTK_NOEXCEPT;
	
	TValue& GetValue();	///< \throw BadOptionalAccessException if does not contain a value.
	const TValue& GetValue() const;	///< \throw BadOptionalAccessException if does not contain a value.
	
	TValue* GetValuePtr();	///< \return \c NULL if does not contain a value.
	const TValue* GetValuePtr() const;	///< \return \c NULL if does not contain a value.
	
	TValue& GetValueOr(TValue& defaultValue);
	const TValue& GetValueOr(const TValue& defaultValue) const;
	
	TValue* operator->();	///< \attention The behavior is undefined if does not contain a value!
	const TValue* operator->() const;	///< \attention The behavior is undefined if does not contain a value!
	
	TValue& operator*();	///< \attention The behavior is undefined if does not contain a value!
	const TValue& operator*() const;	///< \attention The behavior is undefined if does not contain a value!
private:
	static void UnspecifiedBoolTrue() CPPDEVTK_NOEXCEPT;
	
	
	TValue* pValue_;
};


template <typename TValue>
void swap(Optional<TValue>& x, Optional<TValue>& y) CPPDEVTK_NOEXCEPT;


template <typename TValue>
Optional<TValue> MakeOptional(const TValue& value);

template <typename TValue>
Optional<TValue> MakeOptional(bool cond, const TValue& value);


/// \name \c Optional comparison
///@{

/// \name Compare two \c Optional objects
///@{

template <typename TValue>
bool operator==(const Optional<TValue>& lhs, const Optional<TValue>& rhs);

template <typename TValue>
bool operator!=(const Optional<TValue>& lhs, const Optional<TValue>& rhs);

template <typename TValue>
bool operator<(const Optional<TValue>& lhs, const Optional<TValue>& rhs);

template <typename TValue>
bool operator<=(const Optional<TValue>& lhs, const Optional<TValue>& rhs);

template <typename TValue>
bool operator>(const Optional<TValue>& lhs, const Optional<TValue>& rhs);

template <typename TValue>
bool operator>=(const Optional<TValue>& lhs, const Optional<TValue>& rhs);

///@}

/// \name Compare an \c Optional object with a \c NullOptT
///@{

template <typename TValue>
bool operator==(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator==(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator!=(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator!=(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator<(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator<(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator<=(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator<=(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator>(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator>(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator>=(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT;

template <typename TValue>
bool operator>=(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT;

///@}

/// \name Compare an \c Optional object with a \c T
///@{

template <typename TValue>
bool operator==(const Optional<TValue>& lhs, const TValue& rhs);

template <typename TValue>
bool operator==(const TValue& lhs, const Optional<TValue>& rhs);

template <typename TValue>
bool operator!=(const Optional<TValue>& lhs, const TValue& rhs);

template <typename TValue>
bool operator!=(const TValue& v, const Optional<TValue>& rhs);

template <typename TValue>
bool operator<(const Optional<TValue>& lhs, const TValue& rhs);

template <typename TValue>
bool operator<(const TValue& lhs, const Optional<TValue>& rhs);

template <typename TValue>
bool operator<=(const Optional<TValue>& lhs, const TValue& rhs);

template <typename TValue>
bool operator<=(const TValue& lhs, const Optional<TValue>& rhs);

template <typename TValue>
bool operator>(const Optional<TValue>& lhs, const TValue& rhs);

template <typename TValue>
bool operator>(const TValue& lhs, const Optional<TValue>& rhs);

template <typename TValue>
bool operator>=(const Optional<TValue>& lhs, const TValue& rhs);

template <typename TValue>
bool operator>=(const TValue& lhs, const Optional<TValue>& rhs);

///@}

///@}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions / template definitions.

template <typename TValue>
inline Optional<TValue>::Optional() CPPDEVTK_NOEXCEPT: pValue_(NULL) {}

template <typename TValue>
inline Optional<TValue>::Optional(NullOptT) CPPDEVTK_NOEXCEPT: pValue_(NULL) {}

template <typename TValue>
inline Optional<TValue>::Optional(const TValue& value): pValue_(new TValue(value)) {}

template <typename TValue>
inline Optional<TValue>::Optional(bool cond, const TValue& value): pValue_(NULL) {
	if (cond) {
		pValue_ = new TValue(value);
	}
}

template <typename TValue>
template <typename TConvertibleValue>
inline Optional<TValue>::Optional(const Optional<TConvertibleValue>& other): pValue_(NULL) {
	if (other.HasValue()) {
		pValue_ = new TValue(*other);
	}
}

template <typename TValue>
inline Optional<TValue>::Optional(const Optional& other): pValue_(NULL) {
	if (other.HasValue()) {
		pValue_ = new TValue(*other);
	}
}

template <typename TValue>
inline Optional<TValue>::~Optional() CPPDEVTK_NOEXCEPT {
	Reset();
}

template <typename TValue>
inline Optional<TValue>& Optional<TValue>::operator=(NullOptT) CPPDEVTK_NOEXCEPT {
	Reset();
	return *this;
}

template <typename TValue>
inline Optional<TValue>& Optional<TValue>::operator=(const TValue& value) {
	Optional tmp(value);
	Swap(tmp);
	return *this;
}

template <typename TValue>
template <typename TConvertibleValue>
inline Optional<TValue>& Optional<TValue>::operator=(const Optional<TConvertibleValue>& other) {
	if (other.HasValue()) {
		Optional tmp(other);
		Swap(tmp);
	}
	else {
		Reset();
	}
	return *this;
}

template <typename TValue>
inline Optional<TValue>& Optional<TValue>::operator=(const Optional& other) {
	if (this != &other) {
		if (other.HasValue()) {
			Optional tmp(other);
			Swap(tmp);
		}
		else {
			Reset();
		}
	}
	return *this;
}

template <typename TValue>
inline void Optional<TValue>::Swap(Optional& other) CPPDEVTK_NOEXCEPT {
	using ::std::swap;
	swap(pValue_, other.pValue_);
}

template <typename TValue>
void Optional<TValue>::Reset() CPPDEVTK_NOEXCEPT {
	using ::std::terminate;
	
	
	if (pValue_ == NULL) {
		return;
	}
	
	try {
		delete pValue_;
	}
	catch (const ::std::exception& exc) {
		CPPDEVTK_LOG_FATAL("Optional::Reset(): destructor of TValue (" << typeid(TValue).name()
			<< ") threw exception: " << Exception::GetDetailedInfo(exc));
		CPPDEVTK_ASSERT(0 && "Optional::Reset(): destructor of TValue threw exception");
		SuppressUnusedWarning(exc);
		terminate();
	}
	catch (...) {
		CPPDEVTK_LOG_FATAL("Optional::Reset(): destructor of TValue (" << typeid(TValue).name()
			<< ") threw unknown exception");
		CPPDEVTK_ASSERT(0 && "Optional::Reset(): destructor of TValue threw unknown exception");
		terminate();
	}
	
	pValue_ = NULL;
}

template <typename TValue>
inline void Optional<TValue>::Reset(const TValue& value) {
	TValue* pTmpValue = new TValue(value);
	Reset();
	pValue_ = pTmpValue;
}

template <typename TValue>
inline bool Optional<TValue>::HasValue() const CPPDEVTK_NOEXCEPT {
	return pValue_ != NULL;
}

template <typename TValue>
inline Optional<TValue>::operator UnspecifiedBoolType() const CPPDEVTK_NOEXCEPT {
	return HasValue() ? &UnspecifiedBoolTrue : NULL;
}

template <typename TValue>
inline bool Optional<TValue>::operator!() const CPPDEVTK_NOEXCEPT {
	return !HasValue();
}

template <typename TValue>
inline TValue& Optional<TValue>::GetValue() {
	if (!HasValue()) {
		throw CPPDEVTK_BAD_OPTIONAL_ACCESS_EXCEPTION();
	}
	return operator*();
}

template <typename TValue>
inline const TValue& Optional<TValue>::GetValue() const {
	if (!HasValue()) {
		throw CPPDEVTK_BAD_OPTIONAL_ACCESS_EXCEPTION();
	}
	return operator*();
}

template <typename TValue>
inline TValue* Optional<TValue>::GetValuePtr() {
	return HasValue() ? operator->() : NULL;
}

template <typename TValue>
inline const TValue* Optional<TValue>::GetValuePtr() const {
	return HasValue() ? operator->() : NULL;
}

template <typename TValue>
inline TValue& Optional<TValue>::GetValueOr(TValue& defaultValue) {
	return HasValue() ? operator*() : defaultValue;
}

template <typename TValue>
inline const TValue& Optional<TValue>::GetValueOr(const TValue& defaultValue) const {
	return HasValue() ? operator*() : defaultValue;
}

template <typename TValue>
CPPDEVTK_NO_INLINE TValue* Optional<TValue>::operator->() {
	CPPDEVTK_DBC_CHECK_PRECONDITION(HasValue());
	return pValue_;
}

template <typename TValue>
CPPDEVTK_NO_INLINE const TValue* Optional<TValue>::operator->() const {
	CPPDEVTK_DBC_CHECK_PRECONDITION(HasValue());
	return pValue_;
}

template <typename TValue>
CPPDEVTK_NO_INLINE TValue& Optional<TValue>::operator*() {
	CPPDEVTK_DBC_CHECK_PRECONDITION(HasValue());
	return *operator->();
}

template <typename TValue>
CPPDEVTK_NO_INLINE const TValue& Optional<TValue>::operator*() const {
	CPPDEVTK_DBC_CHECK_PRECONDITION(HasValue());
	return *operator->();
}

template <typename TValue>
inline void Optional<TValue>::UnspecifiedBoolTrue() CPPDEVTK_NOEXCEPT {}


template <typename TValue>
inline void swap(Optional<TValue>& x, Optional<TValue>& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


template <typename TValue>
inline Optional<TValue> MakeOptional(const TValue& value) {
	return Optional<TValue>(value);
}

template <typename TValue>
inline Optional<TValue> MakeOptional(bool cond, const TValue& value) {
	return Optional<TValue>(cond, value);
}


template <typename TValue>
inline bool operator==(const Optional<TValue>& lhs, const Optional<TValue>& rhs) {
	if (!lhs.HasValue()) {
		return rhs.HasValue() ? false : true;
	}
	if (!rhs.HasValue()) {
		return false;
	}
	return *lhs == *rhs;
}

template <typename TValue>
inline bool operator<(const Optional<TValue>& lhs, const Optional<TValue>& rhs) {
	if (!lhs.HasValue()) {
		return rhs.HasValue() ? true : false;
	}
	if (!rhs.HasValue()) {
		return false;
	}
	return *lhs < *rhs;
}

template <typename TValue>
inline bool operator!=(const Optional<TValue>& lhs, const Optional<TValue>& rhs) {
	return !(lhs == rhs);
}

template <typename TValue>
inline bool operator<=(const Optional<TValue>& lhs, const Optional<TValue>& rhs) {
	return !(rhs < lhs);
}

template <typename TValue>
inline bool operator>(const Optional<TValue>& lhs, const Optional<TValue>& rhs) {
	return rhs < lhs;
}

template <typename TValue>
inline bool operator>=(const Optional<TValue>& lhs, const Optional<TValue>& rhs) {
	return !(lhs < rhs);
}


template <typename TValue>
inline bool operator==(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(rhs);
	return lhs.HasValue() ? false : true;
}

template <typename TValue>
inline bool operator==(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(lhs);
	return rhs.HasValue() ? false : true;
}

template <typename TValue>
inline bool operator<(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(lhs);
	SuppressUnusedWarning(rhs);
	return false;
}

template <typename TValue>
inline bool operator<(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(lhs);
	return rhs.HasValue();
}

template <typename TValue>
inline bool operator!=(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT {
	return !(lhs == rhs);
}

template <typename TValue>
inline bool operator!=(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT {
	return !(lhs == rhs);
}

template <typename TValue>
inline bool operator<=(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT {
	return !(rhs < lhs);
}

template <typename TValue>
inline bool operator<=(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT {
	return !(rhs < lhs);
}

template <typename TValue>
inline bool operator>(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT {
	return rhs < lhs;
}

template <typename TValue>
inline bool operator>(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT {
	return rhs < lhs;
}

template <typename TValue>
inline bool operator>=(const Optional<TValue>& lhs, NullOptT rhs) CPPDEVTK_NOEXCEPT {
	return !(lhs < rhs);
}

template <typename TValue>
inline bool operator>=(NullOptT lhs, const Optional<TValue>& rhs) CPPDEVTK_NOEXCEPT {
	return !(lhs < rhs);
}


template <typename TValue>
inline bool operator==(const Optional<TValue>& lhs, const TValue& rhs) {
	return lhs.HasValue() ? (lhs == Optional<TValue>(rhs)) : false;
}

template <typename TValue>
inline bool operator==(const TValue& lhs, const Optional<TValue>& rhs) {
	return rhs.HasValue() ? (Optional<TValue>(lhs) == rhs) : false;
}

template <typename TValue>
inline bool operator<(const Optional<TValue>& lhs, const TValue& rhs) {
	return lhs.HasValue() ? (lhs < Optional<TValue>(rhs)) : true;
}

template <typename TValue>
inline bool operator<(const TValue& lhs, const Optional<TValue>& rhs) {
	return rhs.HasValue() ? (Optional<TValue>(lhs) < rhs) : false;
}

template <typename TValue>
inline bool operator!=(const Optional<TValue>& lhs, const TValue& rhs) {
	return !(lhs == rhs);
}

template <typename TValue>
inline bool operator!=(const TValue& lhs, const Optional<TValue>& rhs) {
	return !(lhs == rhs);
}

template <typename TValue>
inline bool operator<=(const Optional<TValue>& lhs, const TValue& rhs) {
	return !(rhs < lhs);
}

template <typename TValue>
inline bool operator<=(const TValue& lhs, const Optional<TValue>& rhs) {
	return !(rhs < lhs);
}

template <typename TValue>
inline bool operator>(const Optional<TValue>& lhs, const TValue& rhs) {
	return rhs < lhs;
}

template <typename TValue>
inline bool operator>(const TValue& lhs, const Optional<TValue>& rhs) {
	return rhs < lhs;
}

template <typename TValue>
inline bool operator>=(const Optional<TValue>& lhs, const TValue& rhs) {
	return !(lhs < rhs);
}

template <typename TValue>
inline bool operator>=(const TValue& lhs, const Optional<TValue>& rhs) {
	return !(lhs < rhs);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_OPTIONAL_HPP_INCLUDED_
