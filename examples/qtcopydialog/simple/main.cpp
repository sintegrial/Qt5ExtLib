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

#include <QApplication>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QSpinBox>

#include "qtcopydialog.h"
#include "qtfilecopier.h"
#include "ui_filedialog.h"

class FileDialog : public QDialog
{
    Q_OBJECT
public:
    FileDialog(QWidget *parent = 0) : QDialog(parent) {
        ui.setupUi(this);

        createdDir  = QString::fromLatin1("created");
        copiedDir   = QString::fromLatin1("copied");
        movedDir    = QString::fromLatin1("moved");
        copiedFiles = QString::fromLatin1("copiedFiles");
        movedFiles  = QString::fromLatin1("movedFiles");

        connect(ui.createButton,        SIGNAL(clicked()), this, SLOT(create()));
        connect(ui.copyDirButton,       SIGNAL(clicked()), this, SLOT(copyDir()));
        connect(ui.moveDirButton,       SIGNAL(clicked()), this, SLOT(moveDir()));
        connect(ui.copyFilesButton,     SIGNAL(clicked()), this, SLOT(copyFiles()));
        connect(ui.moveFilesButton,     SIGNAL(clicked()), this, SLOT(moveFiles()));
        connect(ui.clearButton,         SIGNAL(clicked()), this, SLOT(clearFiles()));
        connect(ui.makeLinksCheckBox,   SIGNAL(clicked()), this, SLOT(setButtons()));
        connect(ui.followLinksCheckBox, SIGNAL(clicked()), this, SLOT(setButtons()));

        copier = new QtFileCopier(this);
        dialog = new QtCopyDialog(this);
        dialog->setFileCopier(copier);

        connect(copier, SIGNAL(stateChanged(QtFileCopier::State)),
                    this, SLOT(stateChanged(QtFileCopier::State)));
    }
    ~FileDialog()
    {
    }
public slots:

private slots:
    void create();
    void copyDir();
    void moveDir();
    void copyFiles();
    void moveFiles();
    void clearFiles();
    void setButtons();
    void setButtons(bool idle);

    void stateChanged(QtFileCopier::State state);

private:
    QtFileCopier::CopyFlags currentFlags() const;
    void createDestDir(const QString &dest);
    QStringList sourceFiles() const;
    void removeDir(const QString &dir);

    QtFileCopier *copier;
    QtCopyDialog *dialog;
    Ui::FileDialog ui;

    QString createdDir;
    QString copiedDir;
    QString movedDir;
    QString copiedFiles;
    QString movedFiles;

};

QtFileCopier::CopyFlags FileDialog::currentFlags() const
{
    QtFileCopier::CopyFlags f = 0;
    bool makeLinksFlag = ui.makeLinksCheckBox->isChecked();
    bool followLinksFlag = ui.followLinksCheckBox->isChecked();
    bool forceFlag = ui.forceCheckBox->isChecked();
    if (makeLinksFlag)
        f = f | QtFileCopier::MakeLinks;
    if (followLinksFlag)
        f = f | QtFileCopier::FollowLinks;
    if (forceFlag)
        f = f | QtFileCopier::Force;
    return f;
}

void FileDialog::createDestDir(const QString &dest)
{
    QDir destDir;
    if (!destDir.exists(dest))
        destDir.mkdir(dest);
}

QStringList FileDialog::sourceFiles() const
{
    QDir sourceDir(createdDir);
    QStringList files;
    QFileInfoList entries = sourceDir.entryInfoList();
    QListIterator<QFileInfo> it(entries);
    while (it.hasNext()) {
        QFileInfo fi = it.next();
        files.append(fi.absoluteFilePath());
    }
    return files;
}

void FileDialog::removeDir(const QString &dir)
{
    QDir d(dir);
    QFileInfoList dirList = d.entryInfoList(QDir::Dirs);
    QListIterator<QFileInfo> itDir(dirList);
    while (itDir.hasNext()) {
        QFileInfo fi = itDir.next();
        QString dirName = fi.fileName();
        if (fi.isDir() && dirName != QString(".") && dirName != QString("..")) {
            removeDir(fi.absoluteFilePath());
        }
    }
    QFileInfoList fileList = d.entryInfoList(QDir::Files |
                    QDir::Hidden | QDir::System);
    QListIterator<QFileInfo> itFile(fileList);
    while (itFile.hasNext()) {
        QFileInfo fi = itFile.next();
        QString fileName = fi.fileName();
        if (!fi.isDir()) {
            d.remove(fileName);
        }
    }
    QFileInfo fi(dir);
    QDir dirUp = fi.dir();
    dirUp.rmdir(fi.fileName());
}

void FileDialog::create()
{
    QFileInfo fi(createdDir);
    if (fi.exists() && (!fi.isDir() || fi.isSymLink()))
        return;
    if (!fi.exists()) {
        QDir dir = fi.dir();
        if (!dir.mkdir(createdDir))
            return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (int i = 0; i < ui.filesSpinBox->value(); i++) {
        QDir dir(createdDir);
        QString fileName = QString("file%1.tmp").arg(i);
        QFile f(dir.filePath(fileName));
        if (f.open(QFile::WriteOnly)) {
            QByteArray a(1024, 'X');
            for (int j = 0; j < ui.sizeSpinBox->value(); j++)
                f.write(a, 1024);
            f.close();
        }
    }
    QApplication::restoreOverrideCursor();
}

void FileDialog::copyDir()
{
    QtFileCopier::CopyFlags f = currentFlags();
    copier->copyDirectory(createdDir, copiedDir, f);
}

void FileDialog::moveDir()
{
    QtFileCopier::CopyFlags f = currentFlags();
    copier->moveDirectory(createdDir, movedDir, f);
}

void FileDialog::copyFiles()
{
    QtFileCopier::CopyFlags f = currentFlags();
    createDestDir(copiedFiles);
    QStringList files = sourceFiles();
    copier->copyFiles(files, copiedFiles, f);
}

void FileDialog::moveFiles()
{
    QtFileCopier::CopyFlags f = currentFlags();
    createDestDir(movedFiles);
    QStringList files = sourceFiles();
    copier->moveFiles(files, movedFiles, f);
}

void FileDialog::clearFiles()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    removeDir(createdDir);
    removeDir(copiedDir);
    removeDir(movedDir);
    removeDir(copiedFiles);
    removeDir(movedFiles);
    QApplication::restoreOverrideCursor();
}

void FileDialog::setButtons()
{
    setButtons(copier->state() == QtFileCopier::Idle);
}

void FileDialog::setButtons(bool idle)
{
    bool buttonsEnabled = idle;
    bool moveButtonsEnabled = buttonsEnabled;
    if (moveButtonsEnabled && (ui.makeLinksCheckBox->isChecked() ||
                    ui.followLinksCheckBox->isChecked()))
        moveButtonsEnabled = false;
    ui.moveDirButton->setEnabled(moveButtonsEnabled);
    ui.moveFilesButton->setEnabled(moveButtonsEnabled);
    ui.createButton->setEnabled(buttonsEnabled);
    ui.copyDirButton->setEnabled(buttonsEnabled);
    ui.copyFilesButton->setEnabled(buttonsEnabled);
    ui.clearButton->setEnabled(buttonsEnabled);
}

void FileDialog::stateChanged(QtFileCopier::State state)
{
    setButtons(state == QtFileCopier::Idle);
}

#include "main.moc"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FileDialog dlg(NULL);
    dlg.show();

    return app.exec();
}
