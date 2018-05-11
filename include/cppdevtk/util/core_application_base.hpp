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


#ifndef CPPDEVTK_UTIL_CORE_APPLICATION_BASE_HPP_INCLUDED_
#define CPPDEVTK_UTIL_CORE_APPLICATION_BASE_HPP_INCLUDED_


#include "config.hpp"
#include "language_info.hpp"
#include <cppdevtk/base/non_copyable.hpp>

#include <QtCore/QCoreApplication>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>

#include <cstddef>

#if (CPPDEVTK_PLATFORM_WINDOWS)
#include <Windows.h>
#endif


class QObject;
class QEvent;
class QString;

namespace std {
	class exception;
}


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Q_ENUMS(), qualified enum, Q_DECLARE_METATYPE(), qRegisterMetaType() can not be used here
// because we do not inherit QObject.

/// \brief This class provides common functionality for our core applications (CoreApplication and SingleCoreApplication).
/// \details This is a workaround for Qt multiple (virtual) inheritance limitations.
/// \sa <a href="http://doc.qt.io/qt-4.8/moc.html#multiple-inheritance-requires-qobject-to-be-first">Qt multiple inheritance</a>
class CPPDEVTK_UTIL_API CoreApplicationBase: private ::cppdevtk::base::NonCopyable {
	Q_DECLARE_TR_FUNCTIONS(CoreApplicationBase)
public:
	/// \brief The action that will be taken if an exception is thrown from an event handler.
	/// (Throwing exceptions from an event handler is not supported in Qt.)
	enum NotifyThrowAction {
		ntaFirst,
		ntaRethrow = ntaFirst,	///< Rethrow exception (default, as Qt does).
		ntaQuit,	///< Inform user and quit() application.
		ntaAbsorb,	///< Absorb exception and return \c false in notify().
		ntaLast = ntaAbsorb
	};
	
	
	CoreApplicationBase();
	virtual ~CoreApplicationBase() = 0;
	
	void SetNotifyThrowAction(NotifyThrowAction notifyThrowAction);		///< Default is \c ntaRethrow
	NotifyThrowAction GetNotifyThrowAction() const;
	
	/// \name Localization support
	/// \note To enable localization call SetQmInfo()
	///@{
	
	void SetPreferSystemLocale(bool preferSystemLocale);	///< Default is \c false
	bool IsSystemLocalePreferred() const;
	
	/// If IsSystemLocalePreferred() then system, English (United States) otherwise
	LanguageInfo GetDefaultLanguageInfo() const;
	
	void SetCurrentLanguageInfo(const LanguageInfo& languageInfo);
	LanguageInfo GetCurrentLanguageInfo() const;
	
	static void SetQmInfo(const QString& qmPath, const QString& qmNamePrefix);
	static void GetQmInfo(QString& qmPath, QString& qmNamePrefix);
	
	static int GetNumberOfSupportedLanguages();
	
	/// \remark For convenience returned list is sorted by native name.
	static QList<LanguageInfo> GetSupportedLanguageInfos();
	
	///@}
	
	static void SetInfo(const QString& companyName, const QString& companyHomepage,
			const QString& productName, const QString& productVersion, const QString& componentName);
	
	/// Same as QCoreApplication::applicationDirPath() but on Unix dereference symlinks
	/// and on Mac OS X does not include bundle
	static QString GetAppDirPath();
	
	/// For single instance apps
	/// \pre !organizationName().isEmpty()
	/// \pre !applicationName().isEmpty()
	static QString GetUniqueId();
	
	/// \note Termination signals are:
	/// - Unix: SIGTERM and SIGINT
	/// - Windows: CTRL_C_EVENT, CTRL_BREAK_EVENT and CTRL_CLOSE_EVENT
	/// \remark If not set, default is false (as Qt does)
	void SetQuitOnTerminationSignals(bool value);
	bool GetQuitOnTerminationSignals() const;
protected:
	virtual bool notify(QObject* pReceiver, QEvent* pEvent);
	virtual bool NotifyThrowHandler(const ::std::exception* pExc = NULL);
	
	virtual bool SetupTranslators();
private:
	static QList<QLocale> GetSupportedLocales();
	
#	if (CPPDEVTK_PLATFORM_WINDOWS)
	static BOOL WINAPI ConsoleCtrlHandler(_In_ DWORD dwCtrlType);
#	endif
	
	
	NotifyThrowAction notifyThrowAction_;
	bool isSystemLocalePreferred_;
	LanguageInfo currentLanguageInfo_;
	QTranslator qtTranslator_;
	QTranslator baseTranslator_;
	QTranslator utilTranslator_;
	bool quitOnTerminationSignals_;
#	if (CPPDEVTK_PLATFORM_UNIX)
	bool unwatchSigTerm_;
	bool unwatchSigInt_;
#	endif
	
	static QString qmPath_;
	static QString qmNamePrefix_;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline void CoreApplicationBase::SetNotifyThrowAction(NotifyThrowAction notifyThrowAction) {
	notifyThrowAction_ = notifyThrowAction;
}

inline CoreApplicationBase::NotifyThrowAction CoreApplicationBase::GetNotifyThrowAction() const {
	return notifyThrowAction_;
}

inline void CoreApplicationBase::SetPreferSystemLocale(bool preferSystemLocale) {
	isSystemLocalePreferred_ = preferSystemLocale;
}

inline bool CoreApplicationBase::IsSystemLocalePreferred() const {
	return isSystemLocalePreferred_;
}

inline LanguageInfo CoreApplicationBase::GetCurrentLanguageInfo() const {
	return currentLanguageInfo_;
}

inline void CoreApplicationBase::GetQmInfo(QString& qmPath, QString& qmNamePrefix) {
	qmPath = qmPath_;
	qmNamePrefix = qmNamePrefix_;
}

inline int CoreApplicationBase::GetNumberOfSupportedLanguages() {
	return GetSupportedLocales().size();
}

inline bool CoreApplicationBase::GetQuitOnTerminationSignals() const {
	return quitOnTerminationSignals_;
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_CORE_APPLICATION_BASE_HPP_INCLUDED_
