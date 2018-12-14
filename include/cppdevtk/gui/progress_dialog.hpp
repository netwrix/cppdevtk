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


#ifndef CPPDEVTK_GUI_PROGRESS_DIALOG_HPP_INCLUDED_
#define CPPDEVTK_GUI_PROGRESS_DIALOG_HPP_INCLUDED_


#include "config.hpp"
#include "widget_base.hpp"

#include <QtCore/QString>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QProgressDialog>
#else
#include <QtGui/QProgressDialog>
#endif

#include <cstddef>


namespace cppdevtk {
namespace gui {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note Defaults:
/// - modality: window modal
/// - minimumDuration: 0
/// - autoClose: true
/// - autoReset: true
/// - cancelable: according to ctor args (windowFlags for first ctor, and cancelButtonText for second ctor)
/// \attention Public API (except ctor/dtor ans reject()) calls QApplication::processEvents()
class CPPDEVTK_GUI_API ProgressDialog: public QProgressDialog, public WidgetBase {
	Q_OBJECT
public:
	explicit ProgressDialog(QWidget* pParent = NULL, Qt::WindowFlags windowFlags = 0);
	ProgressDialog(const QString& labelText, const QString& cancelButtonText, int minimum, int maximum,
			QWidget* pParent = NULL, Qt::WindowFlags windowFlags = 0);
	virtual ~ProgressDialog();
	
	/// \arg cancelButtonText If empty disable cancellation.
	/// \note Handles cancel button, close (X) button and Esc key. Does not handle Alt+F4 shortcut (TODO)
	void SetCancelable(const QString& cancelButtonText, bool processEvents = true);
	
	/// \pre \a numStages >= 1
	/// \remark Will also set autoClose and autoReset to false
	void SetNumStages(int numStages);
	
	virtual void reject();
public Q_SLOTS:
	/// \pre \a numStage must be in range specified by SetNumStages()
	/// \note label will have text: numStage of numStages: labelText (ex: 1 of 4: Reading file)
	void SetStage(int numStage, const QString& labelText);
	void IncrementStage(const QString& labelText);
	
	void ShowWait(const QString& labelText, int msMinimumDuration = 0);	///< \remark Not cancelable
	void ShowBussy(const QString& labelText, const QString& cancelButtonText, int msMinimumDuration = 0);	///< \remark Cancelable
private:
	Q_DISABLE_COPY(ProgressDialog)
	
	void Initialize();
	
	
	bool isCancelable_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline ProgressDialog::~ProgressDialog() {}

inline void ProgressDialog::reject() {
	if (isCancelable_) {
		QProgressDialog::reject();
	}
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_PROGRESS_DIALOG_HPP_INCLUDED_
