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

#include "sketchpad.h"
#include "palm.xpm"

#include <QWidget>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QCursor>
#include <QtCore/QEvent>
#include <QLabel>
#include <QMessageBox>
//Added by qt3to4:
#include <QtGui/QMouseEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QPaintEvent>

#include <stdlib.h>

/*!
    Constructs a sketch pad. This is simply a widget that we can draw
    on. Makes the pad white.
*/
SketchPad::SketchPad(QWidget *parent)
    : QWidget(parent), pix(size())
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCursor(QCursor(Qt::CrossCursor));

    pix = QPixmap((const char **)palm_xpm);
}

/*!
    Sets the color of the pen that draws on the sketch pad to be \a
    color.
*/
void SketchPad::setColor(const QColor &color)
{
    c = color;
}

/*!
    Clears the 
*/
void SketchPad::clear()
{
    pix.fill();
    update();
}

/*!
    Returns the current color of the pen on the sketch pad.
*/
QColor SketchPad::color() const
{
    return c;
}

/*! \reimp
*/
void SketchPad::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    QPixmap copy(pix);
    pix = QPixmap(size());
    pix.fill(Qt::white);
    QPainter p(&pix);
    p.drawPixmap(QPoint(0, 0), copy);
    update();
}

/*! \reimp

    Draws a line from the previous drawn point to the current mouse
    cursor position.
*/
void SketchPad::mouseMoveEvent(QMouseEvent *e)
{
    QPainter p(&pix);
    p.setPen(QPen(c, 4));
    for (int i = 0; i < 30; ++i) {
	p.drawPoint(e->pos().x() - rand() % 5 + rand() % 5,
		    e->pos().y() - rand() % 5 + rand() % 5);
    }
    lastPos = e->pos();
    update();
}

/*! \reimp

    Draws a point where the mouse cursor is at currently.
*/
void SketchPad::mousePressEvent(QMouseEvent *e)
{
    QPainter p(&pix);
    p.setPen(QPen(c, 2));
    for (int i = 0; i < 30; ++i) {
	p.drawPoint(e->pos().x() - rand() % 5 + rand() % 5,
		    e->pos().y() - rand() % 5 + rand() % 5);
    }
    lastPos = e->pos();
    update();
}

/*! \reimp
*/
void SketchPad::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(QPoint(0, 0), pix);
}
