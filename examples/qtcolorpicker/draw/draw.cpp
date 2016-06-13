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

#include "draw.h"
#include "drawboard.h"

#include <QLayout>
#include <qtcolorpicker.h>
#include <QVBoxLayout>
#include <QHBoxLayout>

/* \class Draw

    \brief The Draw class provides a widget for drawing very simple
    images.
*/

/*
    Constructs the Draw widget. The \a parent argument is
    passed to QWidget's constructor.
*/
Draw::Draw(QWidget *parent)
    : QWidget(parent)
{
    // Create the color picker widget and a drawing board.
    colorPicker = new QtColorPicker(this);
    colorPicker->setStandardColors();
    drawBoard = new DrawBoard(this);
    drawBoard->setColor(colorPicker->currentColor());

    // Arrange them vertically.
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(colorPicker);
    vbox->addWidget(drawBoard);

    // Create the clear and quit buttons.
    clearButton = new QPushButton(tr("&Clear"), this);
    quitButton = new QPushButton(tr("&Quit"), this);

    // Arrange them horizontally
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(clearButton);
    hbox->addWidget(quitButton);

    // Place the clear and quit buttons beneath the drawing board.
    vbox->addLayout(hbox);

    // Connect the signals. If the colorPicker reports a color change,
    // we want the drawboard widget to know, and vice versa. The
    // drawboard's color changes when the right-click popup is
    // activated.
    connect(colorPicker, SIGNAL(colorChanged(const QColor &)), drawBoard, SLOT(setColor(const QColor &)));
    connect(drawBoard, SIGNAL(colorChanged(const QColor &)), colorPicker, SLOT(setCurrentColor(const QColor &)));

    // Connect the clear and quit buttons.
    connect(quitButton, SIGNAL(clicked()), SLOT(close()));
    connect(clearButton, SIGNAL(clicked()), SLOT(clear()));
}

/*
    Clears the drawing board and gives it a white background color.
*/
void Draw::clear()
{
    drawBoard->clear();
}
