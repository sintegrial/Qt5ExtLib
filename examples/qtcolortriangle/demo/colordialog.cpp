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

#include "colordialog.h"
#include <qtcolortriangle.h>
#include <QtGui/QColor>
#include <QLCDNumber>
#include <QLabel>
#include <QLayout>
#include <QSlider>
#include <QPushButton>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "sketchpad.h"

/*!
    Constructs the color dialog. The \a parent argument is
    passed to QWidget's constructor.

    Lays out the color triangle, the slider controls, the lcd displays
    and the sketch pad.
*/
ColorDialog::ColorDialog(QWidget *parent)
    : QWidget(parent)
{
    // Create the color triangle
    triangle = new QtColorTriangle(this);
    triangle->setFixedSize(200, 230);

    // Extract the r,g,b and h,s,v values from the current color on
    // the triangle.
    QColor col = triangle->color();
    int r,g,b;
    col.getRgb(&r, &g, &b);

    // Create the slider controls and LCD displays, and set the
    // initial values to those corresponding with the triangle's
    // current color.
    spRed = new QSlider(Qt::Horizontal, this);
    spRed->setRange(0, 255);
    spRed->setPageStep(1);
    spRed->setValue(r);
    lcdRed = new QLCDNumber(3, this);

    spGreen = new QSlider(Qt::Horizontal, this);
    spGreen->setRange(0, 255);
    spGreen->setPageStep(1);
    spGreen->setValue(g);
    lcdGreen = new QLCDNumber(3, this);

    spBlue = new QSlider(Qt::Horizontal, this);
    spBlue->setRange(0, 255);
    spBlue->setPageStep(1);
    spBlue->setValue(b);
    lcdBlue = new QLCDNumber(3, this);

    lcdRed->display(r);
    lcdGreen->display(g);
    lcdBlue->display(b);

    // Create a sketch pad and set its color to the current color of
    // the color triangle.
    pad = new SketchPad(this);
    pad->setColor(triangle->color());

    QPushButton *clearButton = new QPushButton(tr("&Clear"), this);
    connect(clearButton, SIGNAL(clicked()), pad, SLOT(clear()));

    // Lay out the widgets
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(triangle);

    QHBoxLayout *rgbLabelLayout = new QHBoxLayout();
    rgbLabelLayout->addWidget(new QLabel(tr("Red"), this));
    rgbLabelLayout->addWidget(new QLabel(tr("Green"), this));
    rgbLabelLayout->addWidget(new QLabel(tr("Blue"), this));

    QHBoxLayout *rgbSpinBoxLayout = new QHBoxLayout();
    rgbSpinBoxLayout->addWidget(spRed);
    rgbSpinBoxLayout->addWidget(spGreen);
    rgbSpinBoxLayout->addWidget(spBlue);

    QHBoxLayout *rgbLCDLayout = new QHBoxLayout();
    rgbLCDLayout->addWidget(lcdRed);
    rgbLCDLayout->addWidget(lcdGreen);
    rgbLCDLayout->addWidget(lcdBlue);

    QHBoxLayout *rightPadLayout = new QHBoxLayout();
    rightPadLayout->addWidget(clearButton);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignTop);
    rightLayout->addLayout(rgbLabelLayout);
    rightLayout->addLayout(rgbSpinBoxLayout);
    rightLayout->addLayout(rgbLCDLayout);
    rightLayout->addWidget(pad);
    rightLayout->addLayout(rightPadLayout);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    // Connect all signals
    connect(triangle, SIGNAL(colorChanged(const QColor &)), SLOT(setColor(const QColor &)));
    connect(triangle, SIGNAL(colorChanged(const QColor &)), pad, SLOT(setColor(const QColor &)));
    connect(spRed, SIGNAL(valueChanged(int)), SLOT(setRed(int)));
    connect(spGreen, SIGNAL(valueChanged(int)), SLOT(setGreen(int)));
    connect(spBlue, SIGNAL(valueChanged(int)), SLOT(setBlue(int)));
}

/*!
    Destructs the color dialog.
 */
ColorDialog::~ColorDialog()
{
}

/*!
    Sets the current color on the triangle and the controls. The
    signals of the controls are temporarily blocked to prevent
    looping.
*/
void ColorDialog::setColor(const QColor &c)
{
    int h,s,v;
    c.getHsv(&h, &s, &v);

    // To avoid feed-back, we disable the signals emitted by the spin
    // boxes as their values are set.
    spRed->blockSignals(true);
    spGreen->blockSignals(true);
    spBlue->blockSignals(true);

    spRed->setValue(c.red());
    spGreen->setValue(c.green());
    spBlue->setValue(c.blue());

    spRed->blockSignals(false);
    spGreen->blockSignals(false);
    spBlue->blockSignals(false);

    lcdRed->display(c.red());
    lcdGreen->display(c.green());
    lcdBlue->display(c.blue());
}

/*!
    Sets the current red component to \a r. Uses the existing green
    and blue values from the color triangle. Updates the display and
    triangle with the new composite color.
*/
void ColorDialog::setRed(int r)
{
    lcdRed->display(r);

    QColor c(r, spGreen->value(), spBlue->value());
    QColor tc = triangle->color();
    if (tc != c)
	triangle->setColor(c);
}

void ColorDialog::setGreen(int g)
{
    lcdGreen->display(g);

    QColor c(spRed->value(), g, spBlue->value());
    QColor tc = triangle->color();
    if (tc != c)
	triangle->setColor(c);
}

void ColorDialog::setBlue(int b)
{
    lcdBlue->display(b);

    QColor c(spRed->value(), spGreen->value(), b);
    QColor tc = triangle->color();
    if (tc != c)
	triangle->setColor(c);
}
