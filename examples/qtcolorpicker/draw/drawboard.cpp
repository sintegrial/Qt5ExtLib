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

#include "drawboard.h"
#include "dump.xpm"
#include "draw.h"

#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <qtcolorpicker.h>
#include <QtGui/QCursor>
#include <QtGui/QImage>
#include <QSizePolicy>
#include <QtGui/QMouseEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QPaintEvent>

#include <stdlib.h>

/* \class DrawBoard

    \brief The DrawBoard class provides a widget that the user can
    draw on with the mouse cursor.
*/

/*
    Constructs a draw board. Gives it a white background color.
*/
DrawBoard::DrawBoard(QWidget *parent)
    : QWidget(parent), f(0.0), pixmapLoaded(false)
{
    QPalette p = palette();
    p.setColor(backgroundRole(), Qt::white);
    setPalette(p);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCursor(Qt::CrossCursor);
}

/* \reimp
*/
void DrawBoard::paintEvent(QPaintEvent *)
{
    // Lazy load of the initial background pixmap.
    if (!pixmapLoaded)
	pix = QPixmap((const char **)dump_xpm);

    // Only repaint the rects that need repainting.
    QPainter p(this);
    p.drawPixmap(rect(), pix, pix.rect());
}

/* \reimp

    Keeps the original QPixmap contents when enlarging, but clear the
    parts that are not drawn when shrinking.
*/
void DrawBoard::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    if (size().isEmpty())
	return;

    QPixmap copy(pixmapLoaded ? pix : QPixmap((const char **)dump_xpm));
    pixmapLoaded = true;

    pix = QPixmap(size());
    pix.fill(Qt::white);
    QPainter p(&pix);
    p.drawPixmap(copy.rect(), copy, copy.rect());
    update();
}

/*
    If the mouse button is held, draws on 30 random points surrounding
    the cursor. This gives a chalky look.
*/
void DrawBoard::mouseMoveEvent(QMouseEvent *e)
{
    QPainter p(&pix);
    p.setPen(QPen(QColor(col)));

    for (int i = 0; i < 30; ++i) {
	p.drawPoint(e->x() + rand() % 5 - rand() % 5,
		    e->y() + rand() % 4 - rand() % 4);
    }

    update();
}

/*
    Draws on 30 random points surrounding the cursor. This gives a
    chalky look.
*/
void DrawBoard::mousePressEvent(QMouseEvent *e)
{
    if (e->button() & Qt::RightButton) {
	QColor col = QtColorPicker::getColor(mapToGlobal(e->pos()), true);
	if (col.isValid())
	    setColor(col);
    }

    if (e->button() & Qt::LeftButton) {
	QPainter p(&pix);
	p.setPen(QPen(QColor(col)));

	for (int i = 0; i < 30; ++i) {
	    p.drawPoint(e->x() + rand() % 5 - rand() % 5,
			e->y() + rand() % 4 - rand() % 4);
	}

	update();
    }
}

/* \fn DrawBoard::colorChanged(const QColor &color)

    This signal is emitted when the DrawBoard's color is changed.
    \a color is the new color.
*/

/*
    Sets the color used for drawing on the draw board.
*/
void DrawBoard::setColor(const QColor &c)
{
    if (c == col)
	return;

    col = c;
    emit colorChanged(c);
}

/*
    Clears the draw board, giving it a white background.
*/
void DrawBoard::clear()
{
    pix.fill(Qt::white);
    update();
}

/* \reimp
*/
QSize DrawBoard::sizeHint() const
{
    return QSize(200, 220);
}

