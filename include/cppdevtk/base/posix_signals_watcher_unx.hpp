/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_BASE_POSIX_SIGNALS_WATCHER_UNX_HPP_INCLUDED_
#define CPPDEVTK_BASE_POSIX_SIGNALS_WATCHER_UNX_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif
#include "socket_pair.hpp"
#include "singletons.hpp"

#include <QtCore/QObject>
#include <QtCore/QSocketNotifier>
#include <QtCore/QString>

#include <csignal>


// non-std extension
#ifndef SIGNULL
#define SIGNULL 0
#else
#if (SIGNULL != 0)
#	error "SIGNULL != 0"
#endif
#endif


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Templates explicit instantiation.

class PosixSignalsWatcher;
#if (!defined(CPPDEVTK_BASE_POSIX_SIGNALS_WATCHER_UNX_CPP) || CPPDEVTK_COMPILER_CLANG)
CPPDEVTK_BASE_TMPL_EXPL_INST
#endif
template class CPPDEVTK_BASE_API MeyersSingleton<PosixSignalsWatcher>;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa <a href="http://doc.qt.io/qt-5/unix-signals.html">Calling Qt Functions From Unix Signal Handlers</a>
/// \attention A signal argument can not be SIGNULL, SIGKILL or SIGSTOP
class CPPDEVTK_BASE_API PosixSignalsWatcher: public QObject, public MeyersSingleton<PosixSignalsWatcher> {
	friend class MeyersSingleton<PosixSignalsWatcher>;
	
	Q_OBJECT
Q_SIGNALS:
	void Raised(int sig);
	
	// Convenience signals (corresponding Raised() will be also emitted)
	// Termination signals
	void SigTerm();
	void SigInt();
	// Daemon signals
	void SigHUp();
	void SigChld();
	void SigTStp();
	void SigTtOu();
	void SigTtIn();
	// TODO: if needed add other convenience signals here
public Q_SLOTS:
	/// \pre \a sig is not watched
	/// \note By default no signal is watched.
	bool Watch(int sig);
	
	/// \pre \a sig is not watched
	/// \remark Restores old signal action
	bool Unwatch(int sig);
public:
	bool IsWatched(int sig);
private Q_SLOTS:
	void QtSignalHandler(int socket);
	void Dispatch(int sig);
private:
	PosixSignalsWatcher();
	~PosixSignalsWatcher();
	
	
	static bool MakeWriteSocketNonBlocking(int sig);
	static bool CloseSocketPair(int sig);
	static void PosixSignalHandler(int sig);	// nothrow guarantee
	static QString GetPosixSignalName(int sig);
	
	
	// - NSIG is non standard extension but present on all platforms we support
	// Please see NSIG on: https://www.gnu.org/software/libc/manual/html_node/Standard-Signals.html#Standard-Signals
	static const int kNumSigs_ = NSIG - 1;
	
	// - glibc guarantees that the signal numbers are allocated consecutively (this is not required by std).
	// Please see Standard Signals: https://www.gnu.org/software/libc/manual/html_node/Standard-Signals.html#Standard-Signals
	// This is why we use arrays.
	// - sigaction() prevent the signal from being received from within its own signal handler (unless SA_NODEFER is used).
	// But a different signal can interrupt current signal handler if it is not blocked by the signal mask.
	// This is why we use a socket pair and a socket notifier for each signal.
	static socket_t socketPairs_[kNumSigs_][2];	// socketPairs_[x][0] read, socketPairs_[x][1] write
	static QSocketNotifier* socketNotifiers_[kNumSigs_];
	static struct sigaction oldSigActions_[kNumSigs_];
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Inline functions.


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_POSIX_SIGNALS_WATCHER_UNX_HPP_INCLUDED_
