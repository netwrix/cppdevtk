/****************************************************************************
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
** 
** This file is part of a Qt Solutions component.
**
** Commercial Usage  
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage 
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
** 
** If you are unsure which license is appropriate for your use, please
** contact Nokia at qt-info@nokia.com.
** 
****************************************************************************/


#ifndef CPPDEVTK_QTSOLUTIONS_QTCOPYDIALOG_QTCOPYDIALOG_P_H_INCLUDED_
#define CPPDEVTK_QTSOLUTIONS_QTCOPYDIALOG_QTCOPYDIALOG_P_H_INCLUDED_


#include <cppdevtk/QtSolutions/QtCopyDialog/QtCopyDialog>
#include <cppdevtk/QtSolutions/QtCopyDialog/QtFileCopier>

#include "ui_qtcopydialog.h"

#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QMap>
#include <QtCore/QObject>


namespace cppdevtk {
namespace qtsol {


class QtCopyDialogPrivate {
    QtCopyDialog *q_ptr;
    Q_DECLARE_PUBLIC(QtCopyDialog)
public:
    QtCopyDialogPrivate() {}

    void init();

    void error(int id, ::cppdevtk::qtsol::QtFileCopier::Error error, bool stopped);
    void stateChanged(::cppdevtk::qtsol::QtFileCopier::State state);
    void done(bool error);
    void started(int id);
    void dataTransferProgress(int id, qint64 progress);
    void finished(int id, bool error);
    void canceled();
    void childrenCanceled(int id);

    void showProgress();
    void showDialog();
    void reset();
    void setFileLabel(int currentF, int totalFiles);
    void setDirLabel(int currentD, int totalDirs);
    void setFileNames(const QString &source, const QString &dest);
    void setCurrentProgress(qint64 completed, qint64 totalSize);
    void setCompleted(qint64 completed, qint64 totalSize, int msecs);

    void addRequest(int id);

    struct Request {
        QString source;
        QString dest;
        qint64 size;
    };

    QtFileCopier *fileCopier;
    bool autoClose;
    QTimer *showTimer;
    QTime startTime;

    QMap<int, Request> requests;
    int currentFile;
    qint64 totalSize;
    qint64 currentProgress;
    qint64 currentDone;
    int currentProgressTime;
    int currentDoneTime;
    int dirCount;
    int currentDir;

    int lastCurrentId;
    Ui::QtCopyDialog ui;
};


}	// namespace qtsol
}	// namespace cppdevtk


#endif	// CPPDEVTK_QTSOLUTIONS_QTCOPYDIALOG_QTCOPYDIALOG_P_H_INCLUDED_
