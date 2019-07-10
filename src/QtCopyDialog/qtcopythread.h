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


#ifndef CPPDEVTK_QTSOLUTIONS_QTCOPYDIALOG_QTCOPYTHREAD_H_INCLUDED_
#define CPPDEVTK_QTSOLUTIONS_QTCOPYDIALOG_QTCOPYTHREAD_H_INCLUDED_


#include <cppdevtk/QtSolutions/QtCopyDialog/config.hpp>
#include <cppdevtk/QtSolutions/QtCopyDialog/QtFileCopier>

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QMutexLocker>
#include <QtCore/QMap>
#include <QtCore/QtGlobal>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QSet>
#include <QtCore/QQueue>
#include <QtCore/QString>
#if (QT_VERSION >= QT_VERSION_CHECK(4, 4, 0))
#include <QtCore/QAtomicInt>
#else
#include <QtCore/qatomic.h>
#endif


namespace cppdevtk {
namespace qtsol {


struct CopyRequest {
    CopyRequest() {
        move = false;
        dir = false;
    }
    QQueue<int> childrenQueue;
    QString source;
    QString dest;
    bool move;
    bool dir;
    QtFileCopier::CopyFlags copyFlags;
};


class QtCopyThread : public QThread
{
    Q_OBJECT
public:
    QtCopyThread(QtFileCopier *fileCopier);
    ~QtCopyThread();

    struct Request {
        Request() {
            canceled = false;
            overwrite = false;
            moveError = false;
        }
        Request(const CopyRequest &r) {
            request = r;
            canceled = false;
            overwrite = false;
            moveError = false;
        }
        CopyRequest request;
        bool canceled;
        bool overwrite;
        bool moveError;
    };

    void emitProgress(int id, qint64 progress) {
        QMutexLocker l(&mutex);
        Q_EMIT dataTransferProgress(id, progress);
        progressRequest = 0;
    }
    bool isCanceled(int id) const {
        QMutexLocker l(&mutex);
        if (cancelRequest)
            return true;
        if (requestQueue.empty())
            return false;
        return requestQueue[id].canceled;
    }
    bool isMoveError(int id) const {
        QMutexLocker l(&mutex);
        if (requestQueue.empty())
            return false;
        return requestQueue[id].moveError;
    }
    bool isProgressRequest() const {
        return (progressRequest != 0);
    }
    void setMoveError(int id, bool error) {
        QMutexLocker l(&mutex);
        if (requestQueue.empty())
            return;
        requestQueue[id].moveError = error;
    }
    void handle(int id);
    void lockCancelChildren(int id);
    void renameChildren(int id);
    void cancelChildRequests(int id);
    void overwriteChildRequests(int id);

    void setAutoReset(bool on);
public Q_SLOTS:
    void restart();

    void copy(int id, const CopyRequest &request);
    void copy(const QMap<int, CopyRequest> &requests);

    void cancel();
    void cancel(int id);

    void skip();
    void skipAll();
    void retry();

    void overwrite();
    void overwriteAll();

    void resetOverwrite();
    void resetSkip();

    void progress();
Q_SIGNALS:
    void error(int id, ::cppdevtk::qtsol::QtFileCopier::Error error, bool stopped);
    void started(int id);
    void dataTransferProgress(int id, qint64 progress);
    void finished(int id, bool error);
    void canceled();
protected:
    void run();
protected Q_SLOTS:
    void copierDestroyed();
private:

    void cancelChildren(int id);

    QtFileCopier *copier;
    QMap<int, Request> requestQueue;
    mutable QMutex mutex;
    QWaitCondition newCopyCondition;
    QWaitCondition interactionCondition;
    bool waitingForInteraction;
    bool stopRequest;
    bool skipAllRequest;
    QSet<QtFileCopier::Error> skipAllError;
    bool overwriteAllRequest;
    bool cancelRequest;
    int currentId;
#if (QT_VERSION >= QT_VERSION_CHECK(4, 4, 0))
    QAtomicInt progressRequest;
#else
    QAtomic progressRequest;
#endif
    bool autoReset;
};


}	// namespace qtsol
}	// namespace cppdevtk


#endif	// CPPDEVTK_QTSOLUTIONS_QTCOPYDIALOG_QTCOPYTHREAD_H_INCLUDED_
