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


#include <cppdevtk/util/libudev_lnx.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX || CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is Linux specific!!!"
#endif
#include <cppdevtk/base/cassert.hpp>

#include <libudev.h>

#include <cstddef>


namespace cppdevtk {
namespace util {


LibUDev::LibUDev(const QString& dynLibName): DynamicLibrary(dynLibName),
		pfn_udev_new_(NULL),
		pfn_udev_unref_(NULL),
		pfn_udev_enumerate_new_(NULL),
		pfn_udev_enumerate_unref_(NULL),
		pfn_udev_enumerate_add_match_subsystem_(NULL),
		pfn_udev_enumerate_add_match_property_(NULL),
		pfn_udev_enumerate_scan_devices_(NULL),
		pfn_udev_enumerate_get_list_entry_(NULL),
		pfn_udev_list_entry_get_name_(NULL),
		pfn_udev_list_entry_get_value_(NULL),
		pfn_udev_list_entry_get_next_(NULL),
		pfn_udev_device_new_from_syspath_(NULL),
		pfn_udev_device_unref_(NULL),
		pfn_udev_device_get_devtype_(NULL),
		pfn_udev_device_get_property_value_(NULL),
		pfn_udev_device_get_devnode_(NULL),
		pfn_udev_device_get_devpath_(NULL),
		pfn_udev_device_get_subsystem_(NULL),
		pfn_udev_device_get_syspath_(NULL),
		pfn_udev_device_get_sysname_(NULL),
		pfn_udev_device_get_sysnum_(NULL),
		pfn_udev_device_get_properties_list_entry_(NULL),
		pfn_udev_device_get_sysattr_list_entry_(NULL),
		pfn_udev_device_get_tags_list_entry_(NULL) {
	LoadSymbols();
}

void LibUDev::LoadSymbols() {
	CPPDEVTK_ASSERT(pfn_udev_new_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_unref_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_enumerate_new_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_enumerate_unref_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_enumerate_add_match_subsystem_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_enumerate_add_match_property_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_enumerate_scan_devices_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_enumerate_get_list_entry_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_list_entry_get_name_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_list_entry_get_value_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_list_entry_get_next_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_new_from_syspath_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_unref_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_devtype_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_property_value_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_devnode_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_devpath_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_subsystem_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_syspath_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_sysname_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_sysnum_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_properties_list_entry_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_sysattr_list_entry_ == NULL);
	CPPDEVTK_ASSERT(pfn_udev_device_get_tags_list_entry_ == NULL);
	
	pfn_udev_new_ = (tpfn_udev_new)LoadSymbol("udev_new");
	pfn_udev_unref_ = (tpfn_udev_unref)LoadSymbol("udev_unref");
	pfn_udev_enumerate_new_ = (tpfn_udev_enumerate_new)LoadSymbol("udev_enumerate_new");
	pfn_udev_enumerate_unref_ = (tpfn_udev_enumerate_unref)LoadSymbol("udev_enumerate_unref");
	pfn_udev_enumerate_add_match_subsystem_ = (tpfn_udev_enumerate_add_match_subsystem)LoadSymbol("udev_enumerate_add_match_subsystem");
	pfn_udev_enumerate_add_match_property_ = (tpfn_udev_enumerate_add_match_property)LoadSymbol("udev_enumerate_add_match_property");
	pfn_udev_enumerate_scan_devices_ = (tpfn_udev_enumerate_scan_devices)LoadSymbol("udev_enumerate_scan_devices");
	pfn_udev_enumerate_get_list_entry_ = (tpfn_udev_enumerate_get_list_entry)LoadSymbol("udev_enumerate_get_list_entry");
	pfn_udev_list_entry_get_name_ = (tpfn_udev_list_entry_get_name)LoadSymbol("udev_list_entry_get_name");
	pfn_udev_list_entry_get_value_ = (tpfn_udev_list_entry_get_value)LoadSymbol("udev_list_entry_get_value");
	pfn_udev_list_entry_get_next_ = (tpfn_udev_list_entry_get_next)LoadSymbol("udev_list_entry_get_next");
	pfn_udev_device_new_from_syspath_ = (tpfn_udev_device_new_from_syspath)LoadSymbol("udev_device_new_from_syspath");
	pfn_udev_device_unref_ = (tpfn_udev_device_unref)LoadSymbol("udev_device_unref");
	pfn_udev_device_get_devtype_ = (tpfn_udev_device_get_devtype)LoadSymbol("udev_device_get_devtype");
	pfn_udev_device_get_property_value_ = (tpfn_udev_device_get_property_value)LoadSymbol("udev_device_get_property_value");
	pfn_udev_device_get_devnode_ = (tpfn_udev_device_get_devnode)LoadSymbol("udev_device_get_devnode");
	pfn_udev_device_get_devpath_ = (tpfn_udev_device_get_devpath)LoadSymbol("udev_device_get_devpath");
	pfn_udev_device_get_subsystem_ = (tpfn_udev_device_get_subsystem)LoadSymbol("udev_device_get_subsystem");
	pfn_udev_device_get_syspath_ = (tpfn_udev_device_get_syspath)LoadSymbol("udev_device_get_syspath");
	pfn_udev_device_get_sysname_ = (tpfn_udev_device_get_sysname)LoadSymbol("udev_device_get_sysname");
	pfn_udev_device_get_sysnum_ = (tpfn_udev_device_get_sysnum)LoadSymbol("udev_device_get_sysnum");
	pfn_udev_device_get_properties_list_entry_ = (tpfn_udev_device_get_properties_list_entry)LoadSymbol("udev_device_get_properties_list_entry");
	pfn_udev_device_get_sysattr_list_entry_ = (tpfn_udev_device_get_sysattr_list_entry)LoadSymbol("udev_device_get_sysattr_list_entry");
	pfn_udev_device_get_tags_list_entry_ = (tpfn_udev_device_get_tags_list_entry)LoadSymbol("udev_device_get_tags_list_entry");
}


}	// namespace util
}	// namespace cppdevtk
