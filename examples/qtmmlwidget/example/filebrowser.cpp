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

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#include "filebrowser.h"
#include "mainwindow.h"

FileBrowser::FileBrowser(const QString &name_filter, QWidget *parent)
    : QListWidget(parent)
{
    m_name_filter = name_filter;
    readDir(QDir::currentPath());
    
    connect(this, SIGNAL(itemActivated(QListWidgetItem *)),
            this, SLOT(activatedSlot(QListWidgetItem *)));
    connect(this, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(currentChangedSlot(QListWidgetItem *)));
}

void FileBrowser::readDir(const QString &path)
{
    QDir dir(path, m_name_filter);
    dir.setFilter(QDir::AllDirs | QDir::Files);
    if (!dir.isReadable()) {
    	showWarning("Could not open \"" + path + "\"");
	return;
    }
    clear();
    
    QStringList entry_list = dir.entryList();
    QStringList::const_iterator it = entry_list.begin();
    for (; it != entry_list.end(); ++it) {
	if (*it != ".")
	    addItem(*it);
    }
    m_path = path;
}

void FileBrowser::activatedSlot(QListWidgetItem *itm)
{
    if (!itm)
        return;
    
    QString name = itm->text();
    QString path = m_path + "/" + name;
    
    if (QFileInfo(path).isDir())
    	readDir(path);
}

void FileBrowser::currentChangedSlot(QListWidgetItem *itm)
{
    if (!itm)
        return;

    QString name = itm->text();
    QString path = m_path + "/" + name;

    if (QFileInfo(path).isFile())
    	emit fileSelected(path);
}


