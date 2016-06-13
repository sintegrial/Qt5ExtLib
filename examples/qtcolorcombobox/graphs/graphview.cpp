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

#include "graphview.h"

#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtCore/QVector>
#include <QtGui/QPaintEvent>
#include "data.cpp"


GraphView::GraphView(QWidget *parent)
    : QFrame(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFrameShape(QFrame::Panel);

    setFrameShadow(Sunken);
    setLineWidth(1);
    setMidLineWidth(2);
    graph1Color = Qt::red;
    graph2Color = Qt::green;
    graph3Color = Qt::black;
    graphNeedsRedrawing = true;
}

int GraphView::heightForWidth(int w) const
{
    return w;
}

QSize GraphView::sizeHint() const
{
    return QSize(400, 300);
}


void GraphView::setColor1(const QColor &color)
{
    graph1Color = color;
    graphNeedsRedrawing = true;
    update();
}

void GraphView::setColor2(const QColor &color)
{
    graph2Color = color;
    graphNeedsRedrawing = true;
    update();
}

void GraphView::setColor3(const QColor &color)
{
    graph3Color = color;
    graphNeedsRedrawing = true;
    update();
}

void GraphView::drawGraphs()
{
    pix.fill(Qt::white);

    QPainter p(&pix);

    int max = graph1[0];
    int min = max;

    int i;
    for (i = 0; i < 1000; ++i) {
	if (min > graph1[i]) min = graph1[i];
	if (min > graph2[i]) min = graph2[i];
	if (min > graph3[i]) min = graph3[i];
	if (max < graph1[i]) max = graph1[i];
	if (max < graph2[i]) max = graph2[i];
	if (max < graph3[i]) max = graph3[i];
    }

    int voffset = height() / 2;
    double scale = (double)(height() - height()/10) / (double)(max - min);

    p.setPen(Qt::gray);
    QFont f = p.font();
    f.setPointSize(5);
    QFontMetrics fm(f);
    p.setFont(f);
    int fheight = p.fontMetrics().height();

    for (i = 0; i <= 10; ++i) {
	int x = (i * width()) / 10;
	p.drawLine(x, 0, x, height() - 1);
	p.drawText(x + 4, height() - fheight, QString::number(i));
    }

    for (i = 0; i <= 10; ++i) {
	int y = (i * height()) / 10;
	p.drawLine(0, y, width() - 1, y);
	p.drawText(4, y + 10, QString::number(5 - i));
    }

    p.setBrush(Qt::white);
    p.setPen(Qt::black);
    p.drawRect(12, 5, fm.width("USD  ") + fheight/2 + 6, fheight * 3 + 4);

    if (graph1Color.isValid()) {
	p.setPen(QPen(graph1Color, 1));
	p.setBrush(QBrush(graph1Color));

	for (i = 0; i < width() - 2; ++i) {
	    int leftScaledLeft = (i * 1000) / width();
	    int leftScaledRight = ((i + 1) * 1000) / width();
	    int rightScaledLeft = ((i + 1) * 1000) / width();
	    int rightScaledRight = ((i + 2) * 1000) / width();

	    int lval = 0;
            int j;
	    for (j = leftScaledLeft; j <= leftScaledRight; ++j)
		lval += graph1[j];
	    lval /= (leftScaledRight - leftScaledLeft + 1);

	    int rval = 0;
	    for (j = rightScaledLeft; j <= rightScaledRight; ++j)
		rval += graph1[j];
	    rval /= (rightScaledRight - rightScaledLeft + 1);

	    p.drawLine(i, voffset + (int)(lval * scale),
		       i + 1, voffset + (int)(rval * scale));
	}

	p.drawText(15, 5 + fheight, "USD");
	p.drawRect(15 + fm.width("USD  "), 5 + fheight/2, fheight/2, fheight/2);
    }

    if (graph2Color.isValid()) {
	p.setPen(QPen(graph2Color, 1));
	p.setBrush(QBrush(graph2Color));

	for (i = 0; i < width() - 2; ++i) {
	    int leftScaledLeft = (i * 1000) / width();
	    int leftScaledRight = ((i + 1) * 1000) / width();
	    int rightScaledLeft = ((i + 1) * 1000) / width();
	    int rightScaledRight = ((i + 2) * 1000) / width();

	    int lval = 0;
            int j;
	    for (j = leftScaledLeft; j <= leftScaledRight; ++j)
		lval += graph2[j];
	    lval /= (leftScaledRight - leftScaledLeft + 1);

	    int rval = 0;
	    for (j = rightScaledLeft; j <= rightScaledRight; ++j)
		rval += graph2[j];
	    rval /= (rightScaledRight - rightScaledLeft + 1);

	    p.drawLine(i, voffset + (int)(lval * scale),
		       i + 1, voffset + (int)(rval * scale));
	}

	p.drawText(15, 5 + 2 * fheight, "JPY");
	p.drawRect(15 + fm.width("USD  "), 5 + fheight + fheight/2, fheight/2, fheight/2);
    }

    if (graph3Color.isValid()) {
	p.setPen(QPen(graph3Color, 1));
	p.setBrush(QBrush(graph3Color));

	for (i = 0; i < width() - 2; ++i) {
	    int leftScaledLeft = (i * 1000) / width();
	    int leftScaledRight = ((i + 1) * 1000) / width();
	    int rightScaledLeft = ((i + 1) * 1000) / width();
	    int rightScaledRight = ((i + 2) * 1000) / width();

	    int lval = 0;
            int j;
	    for (j = leftScaledLeft; j <= leftScaledRight; ++j)
		lval += graph3[j];
	    lval /= (leftScaledRight - leftScaledLeft + 1);

	    int rval = 0;
	    for (j = rightScaledLeft; j <= rightScaledRight; ++j)
		rval += graph3[j];
	    rval /= (rightScaledRight - rightScaledLeft + 1);

	    p.drawLine(i, voffset + (int)(lval * scale),
		       i + 1, voffset + (int)(rval * scale));
	}

	p.drawText(15, 5 + 3 * fheight, "EUR");
	p.drawRect(15 + fm.width("USD  "), 5 + 2 * fheight + fheight/2, fheight/2, fheight/2);
    }

    update();
}

void GraphView::resizeEvent(QResizeEvent *)
{
    pix = QPixmap(size());
    graphNeedsRedrawing = true;
    update();
}

void GraphView::paintEvent(QPaintEvent *e)
{
    // Redraw the graphs if we must.
    if (graphNeedsRedrawing) {
	graphNeedsRedrawing = false;
	drawGraphs();
    }

    // Only repaint the rects that need repainting.
    QVector<QRect> rects = e->region().rects();
    QImage img;
    for (int i = 0; i < rects.count(); i++) {
	QRect r = rects[i];

        QPainter pt(this);

        pt.drawPixmap(QPoint(r.x(), r.y()), pix, r);
    }

    QFrame::paintEvent(e);
}


