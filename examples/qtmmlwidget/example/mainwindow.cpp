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

#include <stdlib.h>
#include <errno.h>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QLabel>
#include <QLayout>
#include <QtGui/QPixmap>
#include <QToolButton>
#include <QToolBar>
#include <QSplitter>
#include <QMessageBox>

#include <qtmmlwidget.h>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "filebrowser.h"

void showWarning(const QString &msg)
{
    QMessageBox::warning(0, "Warning", msg,
                            QMessageBox::Ok,
                            QMessageBox::NoButton,
                            QMessageBox::NoButton);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MathML Browser");    

    QSplitter *splitter = new QSplitter(this);
    splitter->setChildrenCollapsible(false);

    m_file_browser = new FileBrowser("*.mml", splitter);

    QWidget *formula_widget = new QWidget(splitter);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setMargin(2);
    m_mml_widget = new QtMmlWidget(formula_widget);
    layout->addWidget(m_mml_widget);
    m_mml_widget->setBackgroundRole(QPalette::Base);
    m_compare_image = new QLabel;
    layout->addWidget(m_compare_image);
    m_compare_image->setAlignment(Qt::AlignCenter);
    m_compare_image->setBackgroundRole(QPalette::Base);
   
    formula_widget->setLayout(layout);
    setCentralWidget(splitter);

    QToolBar *tool_bar = new QToolBar(this);
    tool_bar->addAction(QIcon(":/images/zoom_in.png"),
		    "Zoom in", this, SLOT(zoomIn()));
    tool_bar->addAction(QIcon(":/images/zoom_out.png"),
		    "Zoom out", this, SLOT(zoomOut()));
    tool_bar->addAction(QIcon(":/images/frames.png"),
		    "Toggle frames", this, SLOT(toggleDrawFrames()));
   
    addToolBar(tool_bar);
    connect(m_file_browser, SIGNAL(fileSelected(const QString &)), this, SLOT(openFile(const QString &)));
}

void MainWindow::zoomIn()
{
    m_mml_widget->setBaseFontPointSize(m_mml_widget->baseFontPointSize() + 2);
}

void MainWindow::zoomOut()
{
    m_mml_widget->setBaseFontPointSize(m_mml_widget->baseFontPointSize() - 2);
}

void MainWindow::toggleDrawFrames()
{
    m_mml_widget->setDrawFrames(!m_mml_widget->drawFrames());
}

void MainWindow::openFile(const QString &mml_file_name)
{
    m_mml_widget->clear();
    m_compare_image->clear();

    QFile file(mml_file_name);
    if (!file.open(QIODevice::ReadOnly)) {
	showWarning("File error: Could not open \""
			+ mml_file_name
			+ "\": " + file.errorString());
	return;
    }

    QTextStream stream(&file);
//    stream.setEncoding(QTextStream::UnicodeUTF8);
    QString text = stream.readAll();
    file.close();
   
    QString error_msg;
    int error_line, error_column;
    bool result = m_mml_widget->setContent(text, &error_msg, &error_line,
						&error_column);

    if (!result) {
    	showWarning("Parse error: line " + QString::number(error_line)
	    	    	+ ", col " + QString::number(error_column)
			+ ": " + error_msg);
	return;
    }

    QPixmap pm;
    int idx = mml_file_name.lastIndexOf('.');
    if (idx != -1) {
	QString image_file_name = mml_file_name.mid(0, idx + 1) + "png";
	if (pm.load(image_file_name))
	    m_compare_image->setPixmap(pm);
    }
}

