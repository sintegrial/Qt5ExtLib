#ifndef FILEEDIT_H
#define FILEEDIT_H

#include <QLineEdit>

#include "QtPropertyDefines.h"


class QtFilePathEditor : public QWidget
{
    Q_OBJECT

public:
    QtFilePathEditor(QWidget *parent = 0);
    
    void setFilePath(const QString &filePath);
    QString filePath() const;
    
    void setFilter(const QString &filter);
    QString filter() const;
    
    void setDefaultPath(const QString &filePath);
    void setOpenType(FilePathType t);
    
Q_SIGNALS:
    void filePathChanged(const QString &filePath);
    
protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    
private Q_SLOTS:
    void buttonClicked();
    
private:
    QLineEdit *m_lineEdit;
    QString m_filter;
    QString m_defPath;
    FilePathType m_openType;
};

#endif
