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


#ifndef CPPDEVTK_GUI_DISK_SPACE_WIDGET_HPP_INCLUDED_
#define CPPDEVTK_GUI_DISK_SPACE_WIDGET_HPP_INCLUDED_


#include "config.hpp"
#include "widget_base.hpp"

#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtGui/QColor>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

#include <cstddef>


namespace Ui {
	class DiskSpaceWidget;
}


namespace cppdevtk {
namespace gui {


/// \note
/// - default DiskNameColor is Qt::black
/// - default SpaceInfoColor is Qt::darkBlue
/// - auto-refresh is disabled by default.
/// - default auto-refresh interval is 5 seconds.
class CPPDEVTK_GUI_API DiskSpaceWidget: public QWidget, public WidgetBase {
	Q_OBJECT
public:
	explicit DiskSpaceWidget(QWidget* pParent = NULL);
	virtual ~DiskSpaceWidget();
	
	void SetDiskNameColor(const QColor& color);
	QColor GetDiskNameColor() const;
	
	void SetSpaceInfoColor(const QColor& color);
	QColor GetSpaceInfoColor() const;
	
	void SetBold(bool value);
	bool GetBold() const;
	
	QString GetPath() const;
	
	int GetAutoRefreshInterval() const;
	bool IsAutoRefreshEnabled() const;
public slots:
	void SetPath(const QString& path);
	
	void SetAutoRefreshInterval(int sec);
	void SetAutoRefreshEnabled(bool value);
	
	void Refresh();
protected:
	virtual void changeEvent(QEvent* pEvent);
	virtual void showEvent(QShowEvent* pShowEvent);
private:
	Q_DISABLE_COPY(DiskSpaceWidget)
	
	void ValidateUi() const;
	void MakeConnections();
	
	void DoRefresh(qreal totalSize, qreal freeSpace);
	void Reset();
	void FileSystemSpaceInfoNotAvailable();
	
	
	Ui::DiskSpaceWidget* pUiDiskSpaceWidget_;
	QColor diskNameColor_;
	QColor spaceInfoColor_;
	QString path_;
	QString mountPoint_;
	QTimer autoRefreshTimer_;
	
	static const QString kDiskNameTextFmt_;
	static const QString kSpaceInfoTextFmt_;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline void DiskSpaceWidget::SetDiskNameColor(const QColor& color) {
	diskNameColor_ = color;
	
	Refresh();
}

inline QColor DiskSpaceWidget::GetDiskNameColor() const {
	return diskNameColor_;
}

inline void DiskSpaceWidget::SetSpaceInfoColor(const QColor& color) {
	spaceInfoColor_ = color;
	
	Refresh();
}

inline QColor DiskSpaceWidget::GetSpaceInfoColor() const {
	return spaceInfoColor_;
}

inline QString DiskSpaceWidget::GetPath() const {
	return path_;
}

inline int DiskSpaceWidget::GetAutoRefreshInterval() const {
	return autoRefreshTimer_.interval() / 1000;
}

inline bool DiskSpaceWidget::IsAutoRefreshEnabled() const {
	return autoRefreshTimer_.isActive();
}

inline void DiskSpaceWidget::SetPath(const QString& path) {
	path_ = path;
	mountPoint_.clear();
	
	Refresh();
}

inline void DiskSpaceWidget::SetAutoRefreshInterval(int sec) {
	autoRefreshTimer_.setInterval(sec * 1000);
}

inline void DiskSpaceWidget::SetAutoRefreshEnabled(bool value) {
	if (value) {
		autoRefreshTimer_.start();
	}
	else {
		autoRefreshTimer_.stop();
	}
}

inline void DiskSpaceWidget::showEvent(QShowEvent* pShowEvent) {
	QWidget::showEvent(pShowEvent);
	
	Refresh();
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_DISK_SPACE_WIDGET_HPP_INCLUDED_
