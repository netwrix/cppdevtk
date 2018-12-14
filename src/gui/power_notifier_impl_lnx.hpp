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


#ifndef CPPDEVTK_GUI_POWER_NOTIFIER_IMPL_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_POWER_NOTIFIER_IMPL_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include <cppdevtk/gui/power_notifier.hpp>

#include <QtCore/QObject>


namespace cppdevtk {
namespace gui {


// NOTE:
// - until v0.9.23 UPower had Sleeping() and Resuming()
// - since v0.99.1 (next version after 0.9.23) Sleeping() and Resuming() were removed from UPower and logind provides support
class PowerNotifier::Impl: public QObject {
	Q_OBJECT
Q_SIGNALS:
	void Sleeping();
	void Resuming();
protected:
	Impl();
private:
	Q_DISABLE_COPY(Impl);
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline PowerNotifier::Impl::Impl(): QObject() {}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_POWER_NOTIFIER_IMPL_LNX_HPP_INCLUDED_
