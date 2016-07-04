#ifndef QTPROPERTYDEFINES_H
#define QTPROPERTYDEFINES_H

#include <QPair>
#include <QString>
#include <QList>


// definitions

enum FilePathType
{
    FPT_FILE_OPEN,
    FPT_FILE_SAVE,
    FPT_DIRECTORY
};


typedef QPair<int, QString> TIntStringPair;
typedef QList<TIntStringPair> TIntStringList;

typedef QPair<double, QString> TDoubleStringPair;
typedef QList<TDoubleStringPair> TDoubleStringList;


#endif // QTPROPERTYDEFINES_H
