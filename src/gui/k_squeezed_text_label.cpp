/* This file is part of the KDE libraries
   Copyright (C) 2000 Ronny Standtke <Ronny.Standtke@gmx.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <cppdevtk/gui/k_squeezed_text_label.hpp>

#include "k_squeezed_text_label_p.h"

#include <QtCore/QtDebug>
#include <QtCore/QLocale>
#include <QtCore/QMimeData>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QClipboard>
#include <QtGui/QTextDocument>
#include <QtGui/QTextDocumentFragment>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#else
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#endif


namespace cppdevtk {
namespace gui {


KSqueezedTextLabel::KSqueezedTextLabel(const QString &text, QWidget *parent)
    : QLabel(parent), WidgetBase(),
      d(new KSqueezedTextLabelPrivate)
{
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "ksqueezed_text_label");
	
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    d->fullText = text;
    d->elideMode = Qt::ElideMiddle;
    squeezeTextToLabel();
}

KSqueezedTextLabel::KSqueezedTextLabel(QWidget *parent)
    : QLabel(parent), WidgetBase(),
      d(new KSqueezedTextLabelPrivate)
{
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "ksqueezed_text_label");
	
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    d->elideMode = Qt::ElideMiddle;
}

KSqueezedTextLabel::~KSqueezedTextLabel()
{
    delete d;
}

void KSqueezedTextLabel::resizeEvent(QResizeEvent *)
{
    squeezeTextToLabel();
}

QSize KSqueezedTextLabel::minimumSizeHint() const
{
    QSize sh = QLabel::minimumSizeHint();
    sh.setWidth(-1);
    return sh;
}

QSize KSqueezedTextLabel::sizeHint() const
{
    int maxWidth = QApplication::desktop()->screenGeometry(this).width() * 3 / 4;
    QFontMetrics fm(fontMetrics());
    int textWidth = fm.width(plainText());
    if (textWidth > maxWidth) {
        textWidth = maxWidth;
    }
    const int chromeWidth = width() - contentsRect().width();
    return QSize(textWidth + chromeWidth, QLabel::sizeHint().height());
}

void KSqueezedTextLabel::setIndent(int indent)
{
    QLabel::setIndent(indent);
    squeezeTextToLabel();
}

void KSqueezedTextLabel::setMargin(int margin)
{
    QLabel::setMargin(margin);
    squeezeTextToLabel();
}

void KSqueezedTextLabel::setText(const QString &text)
{
    d->fullText = text;
    squeezeTextToLabel();
}

void KSqueezedTextLabel::clear()
{
    d->fullText.clear();
    QLabel::clear();
}

void KSqueezedTextLabel::squeezeTextToLabel()
{
    QFontMetrics fm(fontMetrics());
    const int labelWidth = contentsRect().width();
    QStringList squeezedLines;
    bool squeezed = false;
    Q_FOREACH (const QString &line, plainText().split(QLatin1Char('\n'))) {
        int lineWidth = fm.width(line);
        if (lineWidth > labelWidth) {
            squeezed = true;
            squeezedLines << fm.elidedText(line, d->elideMode, labelWidth);
        } else {
            squeezedLines << line;
        }
    }

    if (squeezed) {
        QLabel::setText(squeezedLines.join("\n"));
        setToolTip(d->fullText);
    } else {
        QLabel::setText(d->fullText);
        setToolTip(QString());
    }
}

QRect KSqueezedTextLabel::contentsRect() const
{
    // calculation according to API docs for QLabel::indent
    const int margin = this->margin();
    int indent = this->indent();
    if (indent < 0) {
        if (frameWidth() == 0) {
            indent = 0;
        } else {
            indent = fontMetrics().width(QLatin1Char('x')) / 2 - margin;
        }
    }

    QRect result = QLabel::contentsRect();
    if (indent > 0) {
        const int alignment = this->alignment();
        if (alignment & Qt::AlignLeft) {
            result.setLeft(result.left() + indent);
        }
        if (alignment & Qt::AlignTop) {
            result.setTop(result.top() + indent);
        }
        if (alignment & Qt::AlignRight) {
            result.setRight(result.right() - indent);
        }
        if (alignment & Qt::AlignBottom) {
            result.setBottom(result.bottom() - indent);
        }
    }

    result.adjust(margin, margin, -margin, -margin);
    return result;
}

void KSqueezedTextLabel::setAlignment(Qt::Alignment alignment)
{
    // save fullText and restore it
    QString tmpFull(d->fullText);
    QLabel::setAlignment(alignment);
    d->fullText = tmpFull;
}

Qt::TextElideMode KSqueezedTextLabel::textElideMode() const
{
    return d->elideMode;
}

void KSqueezedTextLabel::setTextElideMode(Qt::TextElideMode mode)
{
    d->elideMode = mode;
    squeezeTextToLabel();
}

QString KSqueezedTextLabel::fullText() const
{
    return d->fullText;
}

QString KSqueezedTextLabel::plainText() const {
	QString plainText = d->fullText;
	if (textFormat() == Qt::RichText || (textFormat() == Qt::AutoText && Qt::mightBeRichText(plainText))) {
		plainText = QTextDocumentFragment::fromHtml(plainText).toPlainText();
	}
	return plainText;
}

bool KSqueezedTextLabel::isSqueezed() const
{
    return d->fullText != text();
}

void KSqueezedTextLabel::contextMenuEvent(QContextMenuEvent *ev)
{
    // We want to reimplement "Copy" to include the elided text.
    // But this means reimplementing the full popup menu, so no more
    // copy-link-address or copy-selection support anymore, since we
    // have no access to the QTextDocument.
    // Maybe we should have a boolean flag in KSqueezedTextLabel itself for
    // whether to show the "Copy Full Text" custom popup?
    // For now I chose to show it when the text is squeezed; when it's not, the
    // standard popup menu can do the job (select all, copy).

    if (isSqueezed()) {
        QMenu menu(this);

        QAction *act = new QAction(tr("&Copy Full Text"), &menu);
        connect(act, SIGNAL(triggered()), d, SLOT(_k_copyFullText()));
        menu.addAction(act);

        ev->accept();
        menu.exec(ev->globalPos());
    } else {
        QLabel::contextMenuEvent(ev);
    }
}

void KSqueezedTextLabel::mousePressEvent(QMouseEvent* pEvent) {
	QLabel::mousePressEvent(pEvent);
	
	Q_EMIT Pressed();
}

void KSqueezedTextLabel::mouseReleaseEvent(QMouseEvent *ev)
{
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
    if (QApplication::clipboard()->supportsSelection() &&
            textInteractionFlags() != Qt::NoTextInteraction &&
            ev->button() == Qt::LeftButton &&
            !d->fullText.isEmpty() &&
            hasSelectedText()) {
        // Expand "..." when selecting with the mouse
        QString txt = selectedText();
        const QChar ellipsisChar(0x2026); // from qtextengine.cpp
        const int dotsPos = txt.indexOf(ellipsisChar);
        if (dotsPos > -1) {
            // Ex: abcde...yz, selecting de...y  (selectionStart=3)
            // charsBeforeSelection = selectionStart = 2 (ab)
            // charsAfterSelection = 1 (z)
            // final selection length= 26 - 2 - 1 = 23
            const int start = selectionStart();
            int charsAfterSelection = text().length() - start - selectedText().length();
            txt = plainText();	// Strip markup tags
			charsAfterSelection -= d->fullText.length() - txt.length();	// account for stripped characters
            txt = txt.mid(selectionStart(), txt.length() - start - charsAfterSelection);
        }
        QApplication::clipboard()->setText(txt, QClipboard::Selection);
    } else
#endif
    {
        QLabel::mouseReleaseEvent(ev);
    }
	
	Q_EMIT Released();
}

void KSqueezedTextLabel::mouseDoubleClickEvent(QMouseEvent* pEvent) {
	QLabel::mouseDoubleClickEvent(pEvent);
	
	Q_EMIT DoubleClicked();
}


}	// namespace gui
}	// namespace cppdevtk
