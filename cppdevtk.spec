#
# spec file for package cppdevtk
#
# Copyright (C) 2015 - 2017 CoSoSys Ltd <info@cososys.com>.
#
# Licensed under the Apache License, Version 2.0.
# Please see the file COPYING.

# Please submit bugfixes or comments via https://github.com/cososys/cppdevtk/issues
#


# suse_version, sle_version, is_opensuse, leap_version (deprecated):
# https://en.opensuse.org/openSUSE:Build_Service_cross_distribution_howto#Detect_a_distribution_flavor_for_special_code
# - Tumbleweed: suse_version 1330
# - Leap 42.3: suse_version 1315, sle_version 120300, leap_version 420300
# - Leap 42.2: suse_version 1315, sle_version 120200
# - SLE12 SP3: sle_version 120300
# - SLE12 SP2: sle_version 120200
# Attention: openSUSE 13.2 has suse_version 1320


# TODO: modify as desired
%define _prefix /opt/cososys
%define _datadir /usr/share
%if (0%{?centos} || 0%{?rhel})
%define _defaultdocdir %{_datadir}/doc
%endif
%define _sysconfdir /etc
%ifarch x86_64 amd64
%define rpathdir %{_prefix}/lib64
%else
%define rpathdir %{_prefix}/lib
%endif
%define debug_build 0
%define debug_package %{nil}

# CentOS minor version
%if (0%{?centos} || 0%{?rhel})
%if (0%{?centos} == 6 || 0%{?rhel} == 6)
%define centos_minor_ver 9
%endif
%if (0%{?centos} == 7 || 0%{?rhel} == 7)
%define centos_minor_ver 4
%endif
%endif


# Non-modifyable stuff starts here


# Qt 4 or 5
%if (0%{?centos} || 0%{?rhel})
# Qt5 is available on CentOS 7 >= 7.2 (EPEL) and 7.3 (official)
%if (0%{?centos} == 7 || 0%{?rhel} == 7)
%if (%{centos_minor_ver} >= 2)
%define with_qt5 1
%else
%define with_qt5 0
%endif
%endif
# Qt5 is available on CentOS 6 >= 6.6 (EPEL)
%if (0%{?centos} == 6 || 0%{?rhel} == 6)
%if (%{centos_minor_ver} >= 6)
%define with_qt5 1
%else
%define with_qt5 0
%endif
%endif
%endif
%if (0%{?suse_version})
%define with_qt5 1
%endif


# norootforbuild


Name: cppdevtk
Version: 1.0.5
%if (0%{?centos} || 0%{?rhel})
Release: 1.el%{rhel}_%{centos_minor_ver}
%else
Release: 1
%endif
Summary: C++ Development Toolkit
License: Apache License 2.0
Group: System/Libraries
Url: https://github.com/cososys/cppdevtk
Vendor: CoSoSys Ltd
Packager: Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
Source: %{name}-%{version}.tar.bz2
BuildRequires: libstdc++-devel glibc-devel
%if (%{with_qt5})
%if (0%{?suse_version})
BuildRequires: libqt5-qtbase-devel >= 5.6.1
%endif
%if (0%{?centos} || 0%{?rhel})
BuildRequires: qt5-qtbase-devel >= 5.6.1
%endif
%else
%if (0%{?suse_version})
BuildRequires: libqt4-devel >= 4.6.2
%endif
%if (0%{?centos} || 0%{?rhel})
BuildRequires: qt-devel >= 4.6.2
%endif
%endif
BuildRoot: %{_tmppath}/%{name}-%{version}-build

%description
CppDevTk is a collection of libraries for C++ development.


%package -n lib%{name}-base
Summary: CppDevTk base library
Group: System/Libraries
BuildRequires: boost-devel >= 1.38.0
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n lib%{name}-base
CppDevTk C++ base library.


%package -n lib%{name}-util
Summary: CppDevTk util library
Group: System/Libraries
BuildRequires: zlib-devel
%if (0%{?centos} || 0%{?rhel})
%if (0%{?centos} == 6 || 0%{?rhel} == 6)
BuildRequires: libudev-devel
%endif
%if (0%{?centos} == 7 || 0%{?rhel} == 7)
BuildRequires: systemd-devel
%endif
%endif
%if (0%{?suse_version})
BuildRequires: libudev-devel
%endif
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n lib%{name}-util
CppDevTk C++ util library.


%package -n lib%{name}-jni
Summary: CppDevTk jni library
Group: System/Libraries
%if (0%{?centos} || 0%{?rhel})
BuildRequires: java-devel = 1:1.7.0
%endif
%if (0%{?suse_version})
BuildRequires: java-devel = 1.8.0
%endif
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n lib%{name}-jni
CppDevTk C++ jni library.


%package -n lib%{name}-gui
Summary: CppDevTk gui library
Group: System/Libraries
BuildRequires: libXScrnSaver-devel libX11-devel
%if (%{with_qt5})
%if (0%{?suse_version})
BuildRequires: libqt5-qtx11extras-devel >= 5.6.1
%endif
%if (0%{?centos} || 0%{?rhel})
BuildRequires: qt5-qtx11extras-devel >= 5.6.1
%endif
%else
%if (0%{?centos} || 0%{?rhel})
BuildRequires: qt-x11
%endif
%endif
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n lib%{name}-gui
CppDevTk C++ gui library.


%package -n lib%{name}
Summary: CppDevTk libraries
Group: System/Libraries
Requires: lib%{name}-base = %{version}
Requires: lib%{name}-util = %{version}
Requires: lib%{name}-jni = %{version}
Requires: lib%{name}-gui = %{version}

%description -n lib%{name}
Convenience package to install all CppDevTk C++ libraries.




%package base-devel
Summary: Development files for lib%{name}-base
Group: Development/Libraries/C and C++
Requires: libstdc++-devel glibc-devel
Requires: boost-devel >= 1.38.0
%if (%{with_qt5})
%if (0%{?suse_version})
Requires: libqt5-qtbase-devel >= 5.6.1
%endif
%if (0%{?centos} || 0%{?rhel})
Requires: qt5-qtbase-devel >= 5.6.1
%endif
%else
%if (0%{?suse_version})
Requires: libqt4-devel >= 4.6.2
%endif
%if (0%{?centos} || 0%{?rhel})
Requires: qt-devel >= 4.6.2
%endif
%endif
Requires: lib%{name}-base = %{version}

%description base-devel
The %{name}-base-devel package contains libraries and header files for
developing applications that use lib%{name}-base.


%package util-devel
Summary: Development files for lib%{name}-util
Group: Development/Libraries/C and C++
%if (0%{?centos} || 0%{?rhel})
%if (0%{?centos} == 6 || 0%{?rhel} == 6)
Requires: libudev-devel
%endif
%if (0%{?centos} == 7 || 0%{?rhel} == 7)
Requires: systemd-devel
%endif
%endif
%if (0%{?suse_version})
Requires: libudev-devel
%endif
Requires: zlib-devel
Requires: lib%{name}-util = %{version}
Requires: %{name}-base-devel = %{version}

%description util-devel
The %{name}-util-devel package contains libraries and header files for
developing applications that use lib%{name}-util.


%package jni-devel
Summary: Development files for lib%{name}-jni
Group: Development/Libraries/C and C++
%if (0%{?centos} || 0%{?rhel})
Requires: java-devel >= 1:1.7.0
%endif
%if (0%{?suse_version})
Requires: java-devel >= 1.8.0
%endif
Requires: lib%{name}-jni = %{version}
Requires: %{name}-util-devel = %{version}

%description jni-devel
The %{name}-jni-devel package contains libraries and header files for
developing applications that use lib%{name}-jni.


%package gui-devel
Summary: Development files for lib%{name}-gui
Group: Development/Libraries/C and C++
Requires: libXScrnSaver-devel libX11-devel
%if (%{with_qt5})
%if (0%{?suse_version})
Requires: libqt5-qtx11extras-devel >= 5.6.1
%endif
%if (0%{?centos} || 0%{?rhel})
Requires: qt5-qtx11extras-devel >= 5.6.1
%endif
%else
%if (0%{?centos} || 0%{?rhel})
Requires: qt-x11
%endif
%endif
Requires: lib%{name}-gui = %{version}
Requires: %{name}-util-devel = %{version}

%description gui-devel
The %{name}-gui-devel package contains libraries and header files for
developing applications that use lib%{name}-gui.


%package devel
Summary: Development files for lib%{name}
Group: Development/Libraries/C and C++
Requires: %{name}-base-devel = %{version}
Requires: %{name}-util-devel = %{version}
Requires: %{name}-jni-devel = %{version}
Requires: %{name}-gui-devel = %{version}

%description devel
Convenience package to install all %{name}-devel packages.




%package test-base
Summary: Test applications for lib%{name}-base
Group: Development/Libraries/C and C++
%if (0%{?suse_version})
BuildRequires: update-desktop-files
%endif
%if (0%{?centos} || 0%{?rhel})
BuildRequires: desktop-file-utils
%endif

%description test-base
The %{name}-test-base package contains applications for testing lib%{name}-base.


%package test-util
Summary: Test applications for lib%{name}-util
Group: Development/Libraries/C and C++
%if (0%{?suse_version})
BuildRequires: update-desktop-files
%endif
%if (0%{?centos} || 0%{?rhel})
BuildRequires: desktop-file-utils
%endif

%description test-util
The %{name}-test-util package contains applications for testing lib%{name}-util.


%package test-gui
Summary: Test applications for lib%{name}-gui
Group: Development/Libraries/C and C++
%if (0%{?suse_version})
BuildRequires: update-desktop-files
%endif
%if (0%{?centos} || 0%{?rhel})
BuildRequires: desktop-file-utils
%endif

%description test-gui
The %{name}-test-gui package contains applications for testing lib%{name}-gui.


%package test
Summary: Test applications for lib%{name}
Group: Development/Libraries/C and C++
Requires: %{name}-test-base = %{version}
Requires: %{name}-test-util = %{version}
Requires: %{name}-test-gui = %{version}

%description test
Convenience package to install all %{name}-test packages.




%debug_package




%prep
%setup -q


%build

%if (0%{?suse_version})
%if (%{suse_version} < 1315)
echo "openSUSE < 13.2 and SLED < 12SP2 are not supported!!!"
exit 1
%endif
%else
%if (0%{?centos} || 0%{?rhel})
%if (0%{?centos} < 6 || 0%{?rhel} < 6)
echo "CentOS (Red Hat) < 6 is not supported!!!"
exit 1
%endif
%else
echo "This distribution is not supported!!!"
exit 1
%endif
%endif


%if (%{with_qt5})
qmake-qt5	\
%else
%if (0%{?centos} || 0%{?rhel})
qmake-qt4	\
%endif
%if (0%{?suse_version})
qmake	\
%endif
%endif
	./cppdevtk.pro -r -spec linux-g++	\
%if %{debug_build}
	CONFIG*=debug CONFIG-=release CONFIG-=debug_and_release	\
%else
	CONFIG*=release CONFIG-=debug CONFIG-=debug_and_release	\
%endif
	CONFIG*=shared CONFIG-=static CONFIG-=static_and_shared	\
	CONFIG+=cppdevtk_rpmbuild	\
	CPPDEVTK_PREFIX=%{buildroot}/%{_prefix}	\
	CPPDEVTK_DATA_ROOT_DIR=%{buildroot}/%{_datadir}	\
	CPPDEVTK_SYS_CONF_DIR=%{buildroot}/%{_sysconfdir}	\
	CPPDEVTK_LOCAL_STATE_DIR=%{buildroot}/%{_localstatedir}	\
	CPPDEVTK_RPATHDIR=%{rpathdir}	\
	&& make qmake_all
	
#	CPPDEVTK_THIRD_PARTY_PREFIX=%{_prefix}

# make_build is not available on all distro we support
make %{_smp_mflags}


%install
%make_install
%if (0%{?suse_version})
%__install -m 755 -d %{buildroot}/%{_defaultdocdir}/%{name}-devel
%__install -m 644 -t %{buildroot}/%{_defaultdocdir}/%{name}-devel %{_builddir}/%{name}-%{version}/doc/*
%endif
%if (0%{?centos} || 0%{?rhel})
%__install -m 755 -d %{buildroot}/%{_datadir}/doc/%{name}-devel
%__install -m 644 -t %{buildroot}/%{_datadir}/doc/%{name}-devel %{_builddir}/%{name}-%{version}/doc/*
%endif


%clean
%__rm -rf %{buildroot}




%pre
case "$1" in
	1)
		# install
	;;
	2)
		# upgrade
	;;
esac
exit 0


%post
/sbin/ldconfig
case "$1" in
	1)
		# install
	;;
	2)
		# upgrade
	;;
esac
exit 0


%preun
case "$1" in
	0)
		# uninstall
	;;
	1)
		# upgrade
	;;
esac
exit 0


%postun
/sbin/ldconfig
case "$1" in
	0)
		# uninstall
	;;
	1)
		# upgrade
	;;
esac
exit 0




%files -n lib%{name}-base
%defattr(-,root,root,-)
%{_libdir}/lib%{name}_base.so.*
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files -n lib%{name}-util
%defattr(-,root,root,-)
%{_libdir}/lib%{name}_util.so.*
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files -n lib%{name}-jni
%defattr(-,root,root,-)
%{_libdir}/lib%{name}_jni.so.*
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files -n lib%{name}-gui
%defattr(-,root,root,-)
%{_libdir}/lib%{name}_gui.so.*
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files -n lib%{name}
%defattr(-,root,root,-)
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO




%files base-devel
%defattr(-,root,root,-)
%{_includedir}/%{name}/config
%{_includedir}/%{name}/base
%{_libdir}/lib%{name}_base.so
%{_libdir}/lib%{name}_base.prl
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files util-devel
%defattr(-,root,root,-)
%{_includedir}/%{name}/util
%{_libdir}/lib%{name}_util.so
%{_libdir}/lib%{name}_util.prl
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files jni-devel
%defattr(-,root,root,-)
%{_includedir}/%{name}/jni
%{_libdir}/lib%{name}_jni.so
%{_libdir}/lib%{name}_jni.prl
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files gui-devel
%defattr(-,root,root,-)
%{_includedir}/%{name}/gui
%{_libdir}/lib%{name}_gui.so
%{_libdir}/lib%{name}_gui.prl
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files devel
%defattr(-,root,root,-)
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO
%docdir %{_defaultdocdir}/%{name}-devel
%{_defaultdocdir}/%{name}-devel/*




%files test-base
%defattr(-,root,root,-)
%{_bindir}/%{name}_test_config
%{_bindir}/%{name}_test_base
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files test-util
%defattr(-,root,root,-)
%{_bindir}/%{name}_test_util
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files test-gui
%defattr(-,root,root,-)
%{_bindir}/%{name}_test_caps_lock_widget
%{_bindir}/%{name}_test_disk_space_widget
%{_bindir}/%{name}_test_invisible_widget
%{_bindir}/%{name}_test_slot_throw
%{_datadir}/applications/*.desktop
%{_datadir}/pixmaps
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%files test
%defattr(-,root,root,-)
%doc AUTHORS BUGS COPYING FAQ NEWS README THANKS TODO


%changelog
* Tue Nov 28 2017 Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
- v1.0.5.1
* Mon Nov 27 2017 Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
- v1.0.4.1
- added CPPDEVTK_NOEXCEPT
- added exception propagation
- added thread (not available on iOS < 9.0 and Mac OS X < 10.7 because __thread is not supported)
- added interruption support to condition variable
- added interruption support to semaphore
- Kubuntu: updated Qt to 5.9.2
- moved EnableIf and IfThenElse as Conditional in type_traits.hpp
- added more std exception classes equivalents
- treat warning "not all control paths return a value" as error
- uniform error reporting/handling in synchronization primitives
- added time utils
- removed sleep.hpp (replaced by this_thread sleep API)
- enabled console test apps on Android and iOS
- fixed internal pthread_mutex_timedlock(): absTime instead of relTime
* Tue Oct 17 2017 Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
- v1.0.3.1
- added generic locking algorithms
- added condition variable
- added semaphore
- android: added GetFilesDirPath()
- mutex review/cleanup
- increased iOS minimum version from 6.0 to 8.0
- internal support XCode 8 & 9
- Mac + Win: relaxed IsValidPath()
* Mon Jul 31 2017 Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
- v1.0.2.1
- made public on GitHub: https://github.com/cososys/cppdevtk
- packaging cleanup
- JNI review
- enabled JNI on Win32
- mutex impl .NET workaround: use QMutex even if std mutex available; maybe pImpl in the future
* Mon Jul 3 2017 Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
- v1.0.1.1
- Debian based packaging
- require C++11 enabled for clang and gcc >= 4.3.0
- Made ThrowOutOfMemoryErrorOrAbort() public
- mutex impl: use std mutex if available
- fixed check Qt allowed version
* Mon May 22 2017 Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
- v1.0.0.1
* Thu Aug 18 2016 Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
- initial package
