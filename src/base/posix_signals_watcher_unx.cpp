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


#define CPPDEVTK_BASE_POSIX_SIGNALS_WATCHER_UNX_CPP


#include <cppdevtk/base/posix_signals_watcher_unx.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/static_assert.hpp>
#include <cppdevtk/base/architecture_types.hpp>
#include <cppdevtk/base/cerrno.hpp>

#include <new>
#include <cstddef>
#include <cstring>
#include <climits>
#include <limits>

#include <unistd.h>
#include <fcntl.h>


// The default choice in the GNU C Library (and we agree) is to make primitives fail with EINTR (POSIX style, not BSD style).
// This is why we do not use SA_RESTART. Please see Primitives Interrupted by Signals:
// https://www.gnu.org/software/libc/manual/html_node/Interrupted-Primitives.html#Interrupted-Primitives
// It was used in Qt example so we offer it if desired
#define CPPDEVTK_POSIX_SIGNALS_WATCHER_ENABLE_SA_RESTART 0

// In Qt example socket notifier was disabled in qt (read) signal handlers; we do not agree...
// WARN: enabling may lead to problems (signal buffer get full while disabled so no longer notified for ex)
#define CPPDEVTK_POSIX_SIGNALS_WATCHER_DISABLE_SOCKET_NOTIFIER_IN_QT_SIGNAL_HANDLER 0


namespace cppdevtk {
namespace base {


// Implicit initialization of static arrays with 0
socket_t PosixSignalsWatcher::socketPairs_[kNumSigs_][2];
QSocketNotifier* PosixSignalsWatcher::socketNotifiers_[kNumSigs_];
struct sigaction PosixSignalsWatcher::oldSigActions_[kNumSigs_];


bool PosixSignalsWatcher::Watch(int sig) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(!IsWatched(sig), "sig already watched");
	
	const int kSigIdx = sig - 1;
	
	socket_t& readSocket = socketPairs_[kSigIdx][0];
	CPPDEVTK_ASSERT(readSocket == kInvalidSocket);
	socket_t& writeSocket = socketPairs_[kSigIdx][1];
	CPPDEVTK_ASSERT(writeSocket == kInvalidSocket);
	if (SocketPair(socketPairs_[kSigIdx], true) != 0) {
		// On Linux and our Windows implementation this is guaranteed; could not find info for Mac
		// TODO: on Mac call close() if needed (assert for now...)
		CPPDEVTK_ASSERT(readSocket == kInvalidSocket);
		CPPDEVTK_ASSERT(writeSocket == kInvalidSocket);
		
		CPPDEVTK_LOG_ERROR("failed to create socket pair for Posix signal " << sig);
		return false;
	}
	SuppressUnusedWarning(writeSocket);
	
	if (!MakeWriteSocketNonBlocking(sig)) {
		CPPDEVTK_LOG_ERROR("failed to make write socket non-blocking for Posix signal " << sig);
		
		if (!CloseSocketPair(sig)) {
			CPPDEVTK_LOG_WARN("failed to close socket pair for Posix signal " << sig);
		}
		
		return false;
	}
	
	try {
		CPPDEVTK_ASSERT(socketNotifiers_[kSigIdx] == NULL);
		socketNotifiers_[kSigIdx] = new QSocketNotifier(readSocket, QSocketNotifier::Read);
		CPPDEVTK_VERIFY(connect(socketNotifiers_[kSigIdx], SIGNAL(activated(int)), SLOT(QtSignalHandler(int))));
		
		struct sigaction sigAction;
		memset(&sigAction, 0, sizeof(sigAction));
		sigemptyset(&sigAction.sa_mask);
		sigAction.sa_handler = PosixSignalHandler;
#		if (CPPDEVTK_POSIX_SIGNALS_WATCHER_ENABLE_SA_RESTART)
		sigAction.sa_flags |= SA_RESTART;
#		endif
		if (sigaction(sig, &sigAction, &oldSigActions_[kSigIdx]) != 0) {
			/*
			memset(&oldSigActions_[kSigIdx], 0, sizeof(struct sigaction));
			sigemptyset(&oldSigActions_[kSigIdx].sa_mask);
			*/
			delete socketNotifiers_[kSigIdx];
			socketNotifiers_[kSigIdx] = NULL;
			
			throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "failed to register Posix signal handler");
		}
	}
	catch (const ::std::runtime_error& exc) {
		CPPDEVTK_LOG_ERROR("failed to create socket notifier or to register handler for Posix signal " << sig
				<< "; exc: " << Exception::GetDetailedInfo(exc));
		
		if (!CloseSocketPair(sig)) {
			CPPDEVTK_LOG_WARN("failed to close socket pair for Posix signal " << sig);
		}
		
		return false;
	}
	
	return true;
}

bool PosixSignalsWatcher::Unwatch(int sig) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsWatched(sig), "sig is not watched");
	
	const int kSigIdx = sig - 1;
	
	if (sigaction(sig, &oldSigActions_[kSigIdx], NULL) != 0) {
		CPPDEVTK_LOG_ERROR("failed to unregister Posix signal handler for Posix signal " << sig
				<< "; errorCode: " << GetLastSystemErrorCode().ToString());
		return false;
	}
	/*
	memset(&oldSigActions_[kSigIdx], 0, sizeof(struct sigaction));
	sigemptyset(&oldSigActions_[kSigIdx].sa_mask);
	*/
	
	CPPDEVTK_ASSERT(socketNotifiers_[kSigIdx] != NULL);
	delete socketNotifiers_[kSigIdx];
	socketNotifiers_[kSigIdx] = NULL;
	
	if (!CloseSocketPair(sig)) {
		CPPDEVTK_LOG_WARN("failed to close socket pair for Posix signal " << sig);
	}
	
	return true;
}

bool PosixSignalsWatcher::IsWatched(int sig) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((sig != SIGNULL), "sig can not be SIGNULL");
	CPPDEVTK_DBC_CHECK_ARGUMENT((sig != SIGKILL), "sig can not be SIGKILL");
	CPPDEVTK_DBC_CHECK_ARGUMENT((sig != SIGSTOP), "sig can not be SIGSTOP");
	CPPDEVTK_DBC_CHECK_IN_RANGE(((1 <= sig) && (sig <= kNumSigs_)), "sig is out of range");	// [1, kNumSigs_]
	
	const int kSigIdx = sig - 1;
	
	CPPDEVTK_ASSERT((((socketPairs_[kSigIdx][0] == kInvalidSocket) && (socketPairs_[kSigIdx][1] == kInvalidSocket)) && (socketNotifiers_[kSigIdx] == NULL))
			|| (((socketPairs_[kSigIdx][0] != kInvalidSocket) && (socketPairs_[kSigIdx][1] != kInvalidSocket)) && (socketNotifiers_[kSigIdx] != NULL)));
	
	return socketNotifiers_[kSigIdx] != NULL;
}

void PosixSignalsWatcher::QtSignalHandler(int socket) {
	CPPDEVTK_ASSERT(socket != kInvalidSocket);
	
	int sig = SIGNULL;
	for (int idx = 0; idx < kNumSigs_; ++idx) {
		if (socketPairs_[idx][0] == socket) {
			sig = idx + 1;
			break;
		}
	}
	if (sig == SIGNULL) {
		CPPDEVTK_LOG_WARN("Posix signal not found for read socket " << socket << " (maybe unwatched)");
		return;
	}
	
	const int kSigIdx = sig - 1;
	
#	if (CPPDEVTK_POSIX_SIGNALS_WATCHER_DISABLE_SOCKET_NOTIFIER_IN_QT_SIGNAL_HANDLER)
	QSocketNotifier* pSocketNotifier = socketNotifiers_[kSigIdx];
	CPPDEVTK_ASSERT(pSocketNotifier != NULL);
	CPPDEVTK_ASSERT(pSocketNotifier->isEnabled());
	pSocketNotifier->setEnabled(false);
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((pSocketNotifier)) {
		pSocketNotifier->setEnabled(true);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
#	endif
	
	byte msg = SIGNULL;
	CPPDEVTK_STATIC_ASSERT(sizeof(msg) == 1);
	const ssize_t kReaded = TEMP_FAILURE_RETRY(read(socketPairs_[kSigIdx][0], &msg, sizeof(msg)));
	switch (kReaded) {
		case -1:
			CPPDEVTK_LOG_ERROR("failed to read from socket for Posix signal: " << sig
					<< "; errorCode: " << GetLastSystemErrorCode().ToString());
			return;
		case 1:
			CPPDEVTK_ASSERT(msg == sig);
			break;
		default:
			CPPDEVTK_LOG_ERROR("readed from socket " << kReaded << " bytes (expected 1) for Posix signal: " << sig);
			CPPDEVTK_ASSERT(0 && "we should never get here");
			break;
	}
	
	CPPDEVTK_LOG_INFO("raising Qt signal for Posix signal " << GetPosixSignalName(sig));
	Q_EMIT Raised(sig);
}

void PosixSignalsWatcher::Dispatch(int sig) {
	CPPDEVTK_ASSERT(sig != SIGNULL);
	CPPDEVTK_ASSERT(sig != SIGKILL);
	CPPDEVTK_ASSERT(sig != SIGSTOP);
	CPPDEVTK_ASSERT((1 <= sig) && (sig <= kNumSigs_));	// [1, kNumSigs_]
	
	switch (sig) {
		case SIGTERM:
			Q_EMIT SigTerm();
			break;
		case SIGINT:
			Q_EMIT SigInt();
			break;
		case SIGHUP:
			Q_EMIT SigHUp();
			break;
		case SIGCHLD:
			Q_EMIT SigChld();
			break;
		case SIGTSTP:
			Q_EMIT SigTStp();
			break;
		case SIGTTOU:
			Q_EMIT SigTtOu();
			break;
		case SIGTTIN:
			Q_EMIT SigTtIn();
			break;
		default:
			break;
	}
}

PosixSignalsWatcher::PosixSignalsWatcher(): QObject(), MeyersSingleton<PosixSignalsWatcher>() {
	for (int idx = 0; idx < kNumSigs_; ++idx) {
		socketPairs_[idx][0] = kInvalidSocket;
		socketPairs_[idx][1] = kInvalidSocket;
	}
	
	// socketNotifiers_ and oldSigActions_ are implicitly initialized with 0 so there is nothing more to do for them
	
	CPPDEVTK_VERIFY(connect(this, SIGNAL(Raised(int)), SLOT(Dispatch(int))));
}

PosixSignalsWatcher::~PosixSignalsWatcher() {
	for (int sig = 1; sig <= kNumSigs_; ++sig) {
		if ((sig != SIGKILL) && (sig != SIGSTOP)) {
			if (IsWatched(sig)) {
				if (!Unwatch(sig)) {
					CPPDEVTK_LOG_WARN("failed to unwatch Posix signal: " << sig);
				}
			}
		}
	}
}

bool PosixSignalsWatcher::MakeWriteSocketNonBlocking(int sig) {
	CPPDEVTK_ASSERT(sig != SIGNULL);
	CPPDEVTK_ASSERT(sig != SIGKILL);
	CPPDEVTK_ASSERT(sig != SIGSTOP);
	CPPDEVTK_ASSERT((1 <= sig) && (sig <= kNumSigs_));	// [1, kNumSigs_]
	
	const int kSigIdx = sig - 1;
	
	socket_t& writeSocket = socketPairs_[kSigIdx][1];
	CPPDEVTK_ASSERT(writeSocket != kInvalidSocket);
	
	int socketFlags = fcntl(writeSocket, F_GETFL);
	if (socketFlags == -1) {
		CPPDEVTK_LOG_ERROR("failed to get socket flags; errorCode: " << GetLastSystemErrorCode().ToString());
		return false;
	}
	socketFlags |= O_NONBLOCK;
	if (fcntl(writeSocket, F_SETFL, socketFlags) == -1) {
		CPPDEVTK_LOG_ERROR("failed to set socket flags; errorCode: " << GetLastSystemErrorCode().ToString());
		return false;
	}
	
	return true;
}

bool PosixSignalsWatcher::CloseSocketPair(int sig) {
	CPPDEVTK_ASSERT(sig != SIGNULL);
	CPPDEVTK_ASSERT(sig != SIGKILL);
	CPPDEVTK_ASSERT(sig != SIGSTOP);
	CPPDEVTK_ASSERT((1 <= sig) && (sig <= kNumSigs_));	// [1, kNumSigs_]
	
	bool ret = true;
	
	const int kSigIdx = sig - 1;
	
	socket_t& writeSocket = socketPairs_[kSigIdx][1];
	CPPDEVTK_ASSERT(writeSocket != kInvalidSocket);
	if (close(writeSocket) != 0) {
		CPPDEVTK_LOG_WARN("failed to close write socket for Posix signal: " << sig
				<< "; errorCode: " << GetLastSystemErrorCode().ToString());
		ret = false;
	}
	writeSocket = kInvalidSocket;
	
	socket_t& readSocket = socketPairs_[kSigIdx][0];
	CPPDEVTK_ASSERT(readSocket != kInvalidSocket);
	if (close(readSocket) != 0) {
		CPPDEVTK_LOG_WARN("failed to close read socket for Posix signal: " << sig
				<< "; errorCode: " << GetLastSystemErrorCode().ToString());
		ret = false;
	}
	readSocket = kInvalidSocket;
	
	return ret;
}

void PosixSignalsWatcher::PosixSignalHandler(int sig) {
	CPPDEVTK_ASSERT(sig != SIGNULL);
	CPPDEVTK_ASSERT(sig != SIGKILL);
	CPPDEVTK_ASSERT(sig != SIGSTOP);
	CPPDEVTK_ASSERT((1 <= sig) && (sig <= kNumSigs_));	// [1, kNumSigs_]
	
	const int kSigIdx = sig - 1;
	// does not compile on CentOS 6, gcc 4.4.7
#	if (CPPDEVTK_COMPILER_GCC)
#	if (CPPDEVTK_GNUC_VERSION_NUM > CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 4, 7))
	CPPDEVTK_STATIC_ASSERT(::std::numeric_limits<byte>::max() >= kNumSigs_);
#	else
	CPPDEVTK_STATIC_ASSERT(UCHAR_MAX >= kNumSigs_);
#	endif
#	else
	CPPDEVTK_STATIC_ASSERT(::std::numeric_limits<byte>::max() >= kNumSigs_);
#	endif
	const byte kMsg = sig;
	CPPDEVTK_STATIC_ASSERT(sizeof(kMsg) == 1);
	// write() is async-signal-safe
	CPPDEVTK_ASSERT(socketPairs_[kSigIdx][1] != kInvalidSocket);
	const ssize_t kWritten = TEMP_FAILURE_RETRY(write(socketPairs_[kSigIdx][1], &kMsg, sizeof(kMsg)));
	switch (kWritten) {
		case -1:
			// async-signal-safe so can not log here...
			switch (errno) {
				case EAGAIN:
					break;
#				if (EWOULDBLOCK != EAGAIN)
				case EWOULDBLOCK:
					break;	
#				endif
				case EIO:
					break;	// giveup
				case ENOBUFS:
					break;	// giveup
				case ENOSPC:
					break;	// giveup
				case EINVAL:
					CPPDEVTK_ASSERT(0 && "we should never get here");
					break;
				case EBADF:
					CPPDEVTK_ASSERT(0 && "we should never get here");
					break;
				case EFBIG:
					CPPDEVTK_ASSERT(0 && "we should never get here");
					break;
				case ERANGE:
					CPPDEVTK_ASSERT(0 && "we should never get here");
					break;
				case ECONNRESET:
					CPPDEVTK_ASSERT(0 && "we should never get here");
					break;
				case EPIPE:
					CPPDEVTK_ASSERT(0 && "we should never get here");
					break;
				case ENXIO:
					CPPDEVTK_ASSERT(0 && "we should never get here");
					break;
				case EACCES:
					CPPDEVTK_ASSERT(0 && "we should never get here");
					break;
				case ENETDOWN:
					CPPDEVTK_ASSERT(0 && "we should never get here");
					break;
				case ENETUNREACH:
					CPPDEVTK_ASSERT(0 && "we should never get here");
					break;
				default:
					CPPDEVTK_ASSERT(0 && "write() returned undocumented error code");
					break;
			}
			break;
		case sizeof(kMsg):	// sizeof(kMsg) == 1 so or msg is written, or not (can not be partially written)
			break;
		default:
			CPPDEVTK_ASSERT(0 && "we should never get here");
			break;
	}
}

QString PosixSignalsWatcher::GetPosixSignalName(int sig) {
	switch (sig) {
		case SIGNULL:
			return "SIGNULL";
		// C
		case SIGABRT:
			return "SIGABRT";
		case SIGFPE:
			return "SIGFPE";
		case SIGILL:
			return "SIGILL";
		case SIGINT:
			return "SIGINT";
		case SIGSEGV:
			return "SIGSEGV";
		case SIGTERM:
			return "SIGTERM";
		// POSIX
		case SIGHUP:
			return "SIGHUP";
		case SIGCHLD:
			return "SIGCHLD";
		case SIGTSTP:
			return "SIGTSTP";
		case SIGTTOU:
			return "SIGTTOU";
		case SIGTTIN:
			return "SIGTTIN";
		// TODO: add more signals here if needed
		default:
			return QString::number(sig);
	}
}


#if (CPPDEVTK_COMPILER_CLANG)
template class CPPDEVTK_BASE_API MeyersSingleton<PosixSignalsWatcher>;
#endif


}	// namespace base
}	// namespace cppdevtk
