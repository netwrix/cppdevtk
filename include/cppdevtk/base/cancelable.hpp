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
///
/// \brief Improved \c QtConcurrent::run()
/// \details
///	- \c QThreadPool has the main disadvantage that \c start() does not return a \c QFuture
/// (also it is not policy based (scheduling and size policies for ex) and does not offer more control
/// (active and pending tasks for ex)).
/// - \c QtConcurrent::run() has the main disadvantages that the returned future is not cancelable
/// and does not support priorities.
/// - Our \c StoppableTaskExecutor supports cancelable future and priorities.
/// \note If we'll have time we'll implement our own thread pool.
/// \sa \c QThreadPool and \c QtConcurrent::run()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CPPDEVTK_BASE_CANCELABLE_HPP_INCLUDED_
#define CPPDEVTK_BASE_CANCELABLE_HPP_INCLUDED_


#include "config.hpp"
#include "mutex.hpp"


namespace cppdevtk {
namespace base {


class CPPDEVTK_BASE_API Cancelable {
public:
	virtual ~Cancelable();
	
	virtual void Cancel() = 0;
	virtual bool IsCanceled() const = 0;
};


class CPPDEVTK_BASE_API ClassicCancelable: public Cancelable {
public:
	ClassicCancelable();
	ClassicCancelable(const ClassicCancelable& other);
	ClassicCancelable& operator=(const ClassicCancelable& other);
	
	virtual void Cancel();
	virtual bool IsCanceled() const;
private:
	mutable DefaultMutex isCanceledMtx_;
	volatile bool isCanceled_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline Cancelable::~Cancelable() {}


inline ClassicCancelable::ClassicCancelable(): Cancelable(), isCanceledMtx_(), isCanceled_(false) {}

inline ClassicCancelable::ClassicCancelable(const ClassicCancelable& other): Cancelable(other),
		isCanceledMtx_(), isCanceled_(other.IsCanceled()) {}

inline ClassicCancelable& ClassicCancelable::operator=(const ClassicCancelable& other) {
	if (this != &other) {
		DefaultLockGuard lockGuard(isCanceledMtx_);
		Cancelable::operator=(other);
		isCanceled_ = other.IsCanceled();
	}
	return *this;
}

inline void ClassicCancelable::Cancel() {
	DefaultLockGuard lockGuard(isCanceledMtx_);
	isCanceled_ = true;
}

inline bool ClassicCancelable::IsCanceled() const {
	DefaultLockGuard lockGuard(isCanceledMtx_);
	return isCanceled_;
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_CANCELABLE_HPP_INCLUDED_
