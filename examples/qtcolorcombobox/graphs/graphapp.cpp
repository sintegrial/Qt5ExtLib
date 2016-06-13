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

#include "graphapp.h"
#include "graphview.h"

#include <QLabel>
#include <QLayout>
#include <QWidget>

#include <qtcolorcombobox.h>

App::App(QWidget *parent) : QWidget(parent)
{
    // Create one vertical layout and insert an instance of GraphView.
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    GraphView *graphs = new GraphView(this);
    vlayout->addWidget(graphs);

    // Create a grid of labels and color comboboxes for selecting the
    // colors of the graphs.
    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel("USD", this), 0, 0);
    grid->addWidget(new QLabel("JPY", this), 1, 0);
    grid->addWidget(new QLabel("EUR", this), 2, 0);

    // Initialize each color combobox with standard colors and give
    // them a start color. Enable the color dialog.
    QtColorComboBox *usd = new QtColorComboBox(this);
    usd->setStandardColors();
    usd->setCurrentColor(Qt::red);
    usd->setColorDialogEnabled(true);

    QtColorComboBox *jpy = new QtColorComboBox(this);
    jpy->setStandardColors();
    jpy->setCurrentColor(Qt::green);
    usd->setColorDialogEnabled(true);

    QtColorComboBox *eur = new QtColorComboBox(this);
    eur->setStandardColors();
    eur->setCurrentColor(Qt::black);
    usd->setColorDialogEnabled(true);

    grid->addWidget(usd, 0, 1);
    grid->addWidget(jpy, 1, 1);
    grid->addWidget(eur, 2, 1);
    vlayout->addLayout(grid);

    // Connect the color comboboxes to the graph view.
    connect(usd, SIGNAL(activated(const QColor &)), graphs, SLOT(setColor1(const QColor &)));
    connect(jpy, SIGNAL(activated(const QColor &)), graphs, SLOT(setColor2(const QColor &)));
    connect(eur, SIGNAL(activated(const QColor &)), graphs, SLOT(setColor3(const QColor &)));
}
