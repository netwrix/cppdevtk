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


#ifndef CPPDEVTK_GUI_CUSTOM_WIZARD_HPP_INCLUDED_
#define CPPDEVTK_GUI_CUSTOM_WIZARD_HPP_INCLUDED_


#include "config.hpp"
#include "widget_base.hpp"

#include <QtCore/QtGlobal>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtGui/QPixmap>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif

#include <cstddef>


namespace Ui {
	class CustomWizard;
}

class QWizardPage;
class QAbstractButton;


namespace cppdevtk {
namespace gui {


class CPPDEVTK_GUI_API CustomWizard: public QDialog, public WidgetBase {
	friend class ::QWizardPage;
	
	Q_OBJECT
	Q_ENUMS(WizardStyle WizardOption)
    Q_FLAGS(WizardOptions)
    Q_PROPERTY(WizardStyle wizardStyle READ wizardStyle WRITE setWizardStyle)
    Q_PROPERTY(WizardOptions options READ options WRITE setOptions)
    Q_PROPERTY(Qt::TextFormat titleFormat READ titleFormat WRITE setTitleFormat)
    Q_PROPERTY(Qt::TextFormat subTitleFormat READ subTitleFormat WRITE setSubTitleFormat)
    Q_PROPERTY(int startId READ startId WRITE setStartId)
    Q_PROPERTY(int currentId READ currentId NOTIFY currentIdChanged)
signals:
	void currentIdChanged(int id);
	void helpRequested();
	//void customButtonClicked(int which);
	void pageAdded(int id);
	//void pageRemoved(int id);
public:
	enum WizardButton {
		BackButton,
		NextButton,
		//CommitButton,
		//FinishButton,
		CancelButton,
		HelpButton,
		//CustomButton1,
		//CustomButton2,
		//CustomButton3,
		//Stretch,
		
		//NoButton = -1,
		NStandardButtons = 4,	//	6,
		NButtons = 4	// 9
	};
	
	enum WizardPixmap {
		//WatermarkPixmap,
		LogoPixmap,
		//BannerPixmap,
		//BackgroundPixmap,
		NPixmaps
	};
	
	enum WizardStyle {
		//ClassicStyle,
		ModernStyle,
		//MacStyle,
		//AeroStyle,
		NStyles
	};
	
	enum WizardOption {
		NoOption                = 0x00000000,
		IndependentPages                = 0x00000001,
		IgnoreSubTitles                 = 0x00000002,
		//ExtendedWatermarkPixmap         = 0x00000004,
		//NoDefaultButton                 = 0x00000008,
		//NoBackButtonOnStartPage         = 0x00000010,
		//NoBackButtonOnLastPage          = 0x00000020,
		//DisabledBackButtonOnLastPage    = 0x00000040,
		//HaveNextButtonOnLastPage        = 0x00000080,
		//HaveFinishButtonOnEarlyPages    = 0x00000100,
		NoCancelButton                  = 0x00000200,
		//CancelButtonOnLeft              = 0x00000400,
		HaveHelpButton                  = 0x00000800
		//HelpButtonOnRight               = 0x00001000,
		//HaveCustomButton1               = 0x00002000,
		//HaveCustomButton2               = 0x00004000,
		//HaveCustomButton3               = 0x00008000
	};
	
	Q_DECLARE_FLAGS(WizardOptions, WizardOption)
	
	
	explicit CustomWizard(QWidget* pParent = NULL, Qt::WindowFlags flags = NULL);
	virtual ~CustomWizard();
	
	int addPage(QWizardPage* pPage);
	void setPage(int pageId, QWizardPage* pPage);
	//void removePage(int pageId);
	QWizardPage* page(int pageId) const;
	//bool hasVisitedPage(int pageId) const;
	//QList<int> visitedPages() const;
	//QList<int> pageIds() const;
	void setStartId(int pageId);
	int startId() const;
	QWizardPage* currentPage() const;
	int currentId() const;
	
	virtual bool validateCurrentPage();
	virtual int nextId() const;
	
	//void setField(const QString& name, const QVariant& value);
	//QVariant field(const QString& name) const;
	
	void setWizardStyle(WizardStyle wizardStyle);
	WizardStyle wizardStyle() const;
	
	void setOption(WizardOption option, bool on = true);
	bool testOption(WizardOption option) const;
	void setOptions(WizardOptions options);
	WizardOptions options() const;
	
	//void setButtonText(WizardButton which, const QString& text);
	//QString buttonText(WizardButton which) const;
	//void setButtonLayout(const QList<WizardButton>& layout);
	//void setButton(WizardButton which, QAbstractButton* button);
	//QAbstractButton* button(WizardButton which) const;
	
	void setTitleFormat(Qt::TextFormat format);
	Qt::TextFormat titleFormat() const;
	void setSubTitleFormat(Qt::TextFormat format);
	Qt::TextFormat subTitleFormat() const;
	void setPixmap(WizardPixmap which, const QPixmap& pixmap);
	QPixmap pixmap(WizardPixmap which) const;
	
	//void setSideWidget(QWidget* pWidget);
	//QWidget* sideWidget() const;
	
	//void setDefaultProperty(const char* className, const char* property, const char* changedSignal);
	
	void setVisible(bool visible);
	//QSize sizeHint() const;
	
	void setHomepage(const QString& homepage);
	QString getHomepage() const;
public slots:
	void back();
	void next();
	void restart();
protected:
	//bool event(QEvent* pEvent);
	//void resizeEvent(QResizeEvent* pEvent);
	//void paintEvent(QPaintEvent* pEvent);
#if defined(Q_WS_WIN)
	//bool winEvent(MSG* message, long* result);
#endif
	virtual void changeEvent(QEvent* pEvent);
	virtual void closeEvent(QCloseEvent* pCloseEvent);
	
	virtual void done(int result);
	virtual void initializePage(int pageId);
	virtual void cleanupPage(int pageId);
private:
	Q_DISABLE_COPY(CustomWizard)
	
	void validateUi() const;
	void makeConnections();
	
	void switchToPage(int pageId);
	void reset();
	
	void setTitle(const QString& msg);
	void setSubtitle(const QString& msg);
	
	
	Ui::CustomWizard* pUiCustomWizard_;
	//WizardStyle wizardStyle_;
    WizardOptions options_;
    int startId_;
    //int currentId_;
	QSet<int> initializedPageIds_;
	QString homepage_;
private slots:
	void updateButtonStates();
	void openHomepage();
};


Q_DECLARE_OPERATORS_FOR_FLAGS(CustomWizard::WizardOptions)



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline CustomWizard::WizardStyle CustomWizard::wizardStyle() const {
	return ModernStyle;
}

inline void CustomWizard::setOption(WizardOption option, bool on) {
	if (!(options_ & option) != !on) {
		setOptions(options_ ^ option);
	}
}

inline bool CustomWizard::testOption(WizardOption option) const {
	return (options_ & option);
}

inline CustomWizard::WizardOptions CustomWizard::options() const {
	return options_;
}

inline void CustomWizard::setHomepage(const QString& homepage) {
	homepage_ = homepage;
}

inline QString CustomWizard::getHomepage() const {
	return homepage_;
}

inline void CustomWizard::restart() {
	reset();
    switchToPage(startId());
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_CUSTOM_WIZARD_HPP_INCLUDED_
