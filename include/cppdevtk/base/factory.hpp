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


#ifndef CPPDEVTK_BASE_FACTORY_HPP_INCLUDED_
#define CPPDEVTK_BASE_FACTORY_HPP_INCLUDED_


#include "config.hpp"
#include "type_traits.hpp"
#include "singletons.hpp"
#include "locking_levels.hpp"
#include "mutex.hpp"
#include "dbc.hpp"
#include "cassert.hpp"

#include <cstddef>
#include <map>
#include CPPDEVTK_TR1_HEADER(memory)
#include CPPDEVTK_TR1_HEADER(functional)


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Factory method design pattern.
/// \sa <a href="https://sourceforge.net/p/loki-lib/code/HEAD/tree/trunk/include/loki/Factory.h#l1012">Loki Factory</a>
template <class TAbstractProduct, typename TConcreteProductIdentifier = int, bool multithreadingSupport = false>
class Factory: public Conditional<multithreadingSupport,
		Singleton<Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport> >,
		MeyersSingleton<Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport> > >::Type,
		public ObjectLevelLocking<typename Conditional<multithreadingSupport, DefaultMutex, NullMutex>::Type> {
	// friend was extented in C++11 but for C++98 we need to declare both Singleton and MeyersSingleton as friend
	//friend typename Conditional<multithreadingSupport, Singleton<Factory>, MeyersSingleton<Factory> >::Type;
	friend class Singleton<Factory>;
	friend class MeyersSingleton<Factory>;
public:
	typedef TAbstractProduct AbstractProductType;
	typedef TConcreteProductIdentifier ConcreteProductIdentifierType;
	typedef CPPDEVTK_TR1_NS::function<TAbstractProduct* ()> ConcreteProductCreatorType;	///< \attention Returned pointer may not be \c NULL
private:
	typedef ::std::map<TConcreteProductIdentifier, ConcreteProductCreatorType> IdentifierCreatorMapType;
public:
	typedef typename IdentifierCreatorMapType::size_type SizeType;
	
	
	bool IsEmpty() const;
	SizeType GetSize() const;	///< \return Number of registered products.
	void Clear();
	
	bool IsConcreteProductRegistered(const TConcreteProductIdentifier& concreteProductIdentifier) const;
	bool RegisterConcreteProduct(const TConcreteProductIdentifier& concreteProductIdentifier,
			const ConcreteProductCreatorType& concreteProductCreator);	///< \pre \a concreteProductCreator must have a target
	bool UnregisterConcreteProduct(const TConcreteProductIdentifier& concreteProductIdentifier);
	
	
	/// \name Factory method
	/// \pre \c IsConcreteProductRegistered(concreteProductIdentifier)
	/// \remark Returned pointer is non-NULL
	///@{
	
	CPPDEVTK_TR1_NS::shared_ptr<TAbstractProduct> CreateConcreteProduct(
			const TConcreteProductIdentifier& concreteProductIdentifier) const;
	
	template <typename TDeleter>
	CPPDEVTK_TR1_NS::shared_ptr<TAbstractProduct> CreateConcreteProduct(
			const TConcreteProductIdentifier& concreteProductIdentifier, TDeleter deleter) const;
	
	///@}
private:
	Factory();
	~Factory();
	
	
	IdentifierCreatorMapType identifierCreatorMap_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions / template definitions.

template <class TAbstractProduct, typename TConcreteProductIdentifier, bool multithreadingSupport>
inline bool Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::IsEmpty() const {
	return identifierCreatorMap_.empty();
}

template <class TAbstractProduct, typename TConcreteProductIdentifier, bool multithreadingSupport>
inline typename Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::SizeType
		Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::GetSize() const {
	return identifierCreatorMap_.size();
}

template <class TAbstractProduct, typename TConcreteProductIdentifier, bool multithreadingSupport>
inline void Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::Clear() {
	identifierCreatorMap_.clear();
}

template <class TAbstractProduct, typename TConcreteProductIdentifier, bool multithreadingSupport>
inline bool Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::
		IsConcreteProductRegistered(const TConcreteProductIdentifier& concreteProductIdentifier) const {
	return identifierCreatorMap_.find(concreteProductIdentifier) != identifierCreatorMap_.end();
}

template <class TAbstractProduct, typename TConcreteProductIdentifier, bool multithreadingSupport>
/* inline */ bool Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::
		RegisterConcreteProduct(const TConcreteProductIdentifier& concreteProductIdentifier,
		const ConcreteProductCreatorType& concreteProductCreator) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(concreteProductCreator, "concreteProductCreator must have a target");
	
	return identifierCreatorMap_.insert(typename IdentifierCreatorMapType::value_type(concreteProductIdentifier,
			concreteProductCreator)).second;
}

template <class TAbstractProduct, typename TConcreteProductIdentifier, bool multithreadingSupport>
inline bool Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::
		UnregisterConcreteProduct(const TConcreteProductIdentifier& concreteProductIdentifier) {
	return identifierCreatorMap_.erase(concreteProductIdentifier) != 0;
}

template <class TAbstractProduct, typename TConcreteProductIdentifier, bool multithreadingSupport>
/* inline */ CPPDEVTK_TR1_NS::shared_ptr<TAbstractProduct>
		Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::
		CreateConcreteProduct(const TConcreteProductIdentifier& concreteProductIdentifier) const {
	CPPDEVTK_DBC_CHECK_PRECONDITION(IsConcreteProductRegistered(concreteProductIdentifier));
	
	typename IdentifierCreatorMapType::const_iterator kIter = identifierCreatorMap_.find(concreteProductIdentifier);
	CPPDEVTK_ASSERT(kIter != identifierCreatorMap_.end());
	
	TAbstractProduct* pConcreteProduct = (kIter->second)();
	CPPDEVTK_DBC_CHECK_NON_NULL_POINTER(pConcreteProduct);
	return CPPDEVTK_TR1_NS::shared_ptr<TAbstractProduct>(pConcreteProduct);
}

template <class TAbstractProduct, typename TConcreteProductIdentifier, bool multithreadingSupport>
template <typename TDeleter>
/* inline */ CPPDEVTK_TR1_NS::shared_ptr<TAbstractProduct>
		Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::
		CreateConcreteProduct(const TConcreteProductIdentifier& concreteProductIdentifier, TDeleter deleter) const {
	CPPDEVTK_DBC_CHECK_PRECONDITION(IsConcreteProductRegistered(concreteProductIdentifier));
	
	typename IdentifierCreatorMapType::const_iterator kIter = identifierCreatorMap_.find(concreteProductIdentifier);
	CPPDEVTK_ASSERT(kIter != identifierCreatorMap_.end());
	
	TAbstractProduct* pConcreteProduct = (kIter->second)();
	CPPDEVTK_DBC_CHECK_NON_NULL_POINTER(pConcreteProduct);
	return CPPDEVTK_TR1_NS::shared_ptr<TAbstractProduct>(pConcreteProduct, deleter);
}

template <class TAbstractProduct, typename TConcreteProductIdentifier, bool multithreadingSupport>
inline Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::Factory() {}

template <class TAbstractProduct, typename TConcreteProductIdentifier, bool multithreadingSupport>
inline Factory<TAbstractProduct, TConcreteProductIdentifier, multithreadingSupport>::~Factory() {}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_FACTORY_HPP_INCLUDED_
