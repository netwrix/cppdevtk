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


#ifndef CPPDEVTK_BASE_ANY_HPP_INCLUDED_
#define CPPDEVTK_BASE_ANY_HPP_INCLUDED_


#include "config.hpp"
#include "typeinfo.hpp"
#include "bad_any_cast_exception.hpp"
#include "cloneable.hpp"
#include "non_copyable.hpp"

#include <cstddef>
#include <new>
#include <algorithm>	// swap(), C++98
#include <utility>	// swap(), C++11
#include CPPDEVTK_TR1_HEADER(type_traits)


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A type-safe container for single values of any \c CopyConstructible type with no-throw destructor.
/// \sa
/// - <a href="http://en.cppreference.com/w/cpp/utility/any">C++17 any</a>
/// - <a href="http://www.boost.org/doc/libs/1_65_1/doc/html/any.html">Boost.Any</a>
class CPPDEVTK_BASE_API Any {
	template <typename TValue>
	friend TValue* AnyCast(Any*);
public:
	Any() CPPDEVTK_NOEXCEPT;
	
	template <typename TValue>
	Any(const TValue& value);
	
	Any(const Any& other);
	
	~Any() CPPDEVTK_NOEXCEPT;
	
	
	Any& operator=(const Any& other);
	
	template <typename TValue>
	Any& operator=(const TValue& value);
	
	
	bool HasValue() const CPPDEVTK_NOEXCEPT;
	void Reset() CPPDEVTK_NOEXCEPT;
	
	
	/// \return The \c TypeInfo of the contained value if instance is non-empty, otherwise \c TypeInfo(typeid(void))
	TypeInfo GetTypeInfo() const CPPDEVTK_NOEXCEPT;
	
	
	void Swap(Any& other) CPPDEVTK_NOEXCEPT;
private:
	class TypeErasedValue: public Cloneable {
	public:
		virtual TypeInfo GetTypeInfo() const = 0;
	private:
		TypeErasedValue& operator=(const TypeErasedValue&);
	};
	
	
	template <typename TValue>
	class Value: public TypeErasedValue {
		template <typename TValue1>
		friend TValue1* AnyCast(Any*);
	public:
		typedef TValue ValueType;
		
		
		Value(const TValue& value) CPPDEVTK_NOEXCEPT;
		
		virtual TypeInfo GetTypeInfo() const CPPDEVTK_NOEXCEPT;
		
		::std::auto_ptr<Value> Clone() const;
	protected:
#		if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
		virtual Cloneable* DoClone() const;
#		else
		virtual Value* DoClone() const;
#		endif
	private:
		Value& operator=(const Value&);
		
		
		TValue value_;
	};
	
	
	TypeErasedValue* pTypeErasedValue_;
};


CPPDEVTK_BASE_API void swap(Any& x, Any& y) CPPDEVTK_NOEXCEPT;


template <typename TValue>
TValue* AnyCast(Any* pAny);	///< \return \c NULL if conversion fails

template <typename TValue>
const TValue* AnyCast(const Any* pAny);	///< \return \c NULL if conversion fails

template <typename TValue>
TValue AnyCast(Any& any);	///< \throw BadAnyCastException if conversion fails

template <typename TValue>
TValue AnyCast(const Any& any);	///< \throw BadAnyCastException if conversion fails




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline Any::Any() CPPDEVTK_NOEXCEPT: pTypeErasedValue_(NULL) {}

template <typename TValue>
inline Any::Any(const TValue& value): pTypeErasedValue_(new Value<TValue>(value)) {}

inline Any::Any(const Any& other): pTypeErasedValue_(
		(other.pTypeErasedValue_ == NULL) ? NULL : static_cast<TypeErasedValue*>(other.pTypeErasedValue_->Clone().release())) {}

Any::~Any() CPPDEVTK_NOEXCEPT {
	Reset();
}

inline Any& Any::operator=(const Any& other) {
	Any tmp(other);
	Swap(tmp);
	return *this;
}

template <typename TValue>
inline Any& Any::operator=(const TValue& value) {
	Any tmp(value);
	Swap(tmp);
	return *this;
}

inline bool Any::HasValue() const CPPDEVTK_NOEXCEPT {
	return pTypeErasedValue_ != NULL;
}

inline TypeInfo Any::GetTypeInfo() const CPPDEVTK_NOEXCEPT {
	return (pTypeErasedValue_ == NULL) ? typeid(void) : pTypeErasedValue_->GetTypeInfo();
}

inline void Any::Swap(Any& other) CPPDEVTK_NOEXCEPT {
	using ::std::swap;
	swap(pTypeErasedValue_, other.pTypeErasedValue_);
}


template <typename TValue>
inline Any::Value<TValue>::Value(const TValue& value) CPPDEVTK_NOEXCEPT: TypeErasedValue(), value_(value) {}

template <typename TValue>
inline TypeInfo Any::Value<TValue>::GetTypeInfo() const CPPDEVTK_NOEXCEPT {
	return typeid(TValue);
}

template <typename TValue>
inline ::std::auto_ptr<Any::Value<TValue> > Any::Value<TValue>::Clone() const {
	return ::std::auto_ptr<Value>(static_cast<Value*>(Cloneable::Clone().release()));
}

template <typename TValue>
#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* Any::Value<TValue>::DoClone() const {
#else
inline Any::Value<TValue>* Any::Value<TValue>::DoClone() const {
#endif
	return new Value(*this);
}


inline CPPDEVTK_BASE_API void swap(Any& x, Any& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


template <typename TValue>
inline TValue* AnyCast(Any* pAny) {
	return ((pAny != NULL) && (pAny->GetTypeInfo() == typeid(TValue))) ?
			&(static_cast<Any::Value<TValue>*>(pAny->pTypeErasedValue_)->value_) : NULL;
}

template <typename TValue>
inline const TValue* AnyCast(const Any* pAny) {
	return AnyCast<TValue>(const_cast<Any*>(pAny));
}

template <typename TValue>
inline TValue AnyCast(Any& any) {
	typedef typename CPPDEVTK_TR1_NS::remove_reference<TValue>::type NonRefValueType;
	NonRefValueType* pNonRefValueType = AnyCast<NonRefValueType>(&any);
	if (pNonRefValueType == NULL) {
		throw CPPDEVTK_BAD_ANY_CAST_EXCEPTION();
	}
	return *pNonRefValueType;
}

template <typename TValue>
inline TValue AnyCast(const Any& any) {
	typedef typename CPPDEVTK_TR1_NS::remove_reference<TValue>::type NonRefValueType;
	return AnyCast<const NonRefValueType&>(const_cast<Any&>(any));
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_ANY_HPP_INCLUDED_
