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

#ifndef QTTHUMBWHEEL_H
#define QTTHUMBWHEEL_H

#include <QtGui/QPixmap>
#include <QtWidgets/QAbstractSlider>

#if defined(Q_WS_WIN)
#  if !defined(QT_QTTHUMBWHEEL_EXPORT) && !defined(QT_QTTHUMBWHEEL_IMPORT)
#    define QT_QTTHUMBWHEEL_EXPORT
#  elif defined(QT_QTTHUMBWHEEL_IMPORT)
#    if defined(QT_QTTHUMBWHEEL_EXPORT)
#      undef QT_QTTHUMBWHEEL_EXPORT
#    endif
#    define QT_QTTHUMBWHEEL_EXPORT __declspec(dllimport)
#  elif defined(QT_QTTHUMBWHEEL_EXPORT)
#    undef QT_QTTHUMBWHEEL_EXPORT
#    define QT_QTTHUMBWHEEL_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTTHUMBWHEEL_EXPORT
#endif

class QtThumbWheelPrivate;
class QT_QTTHUMBWHEEL_EXPORT QtThumbWheel : public QAbstractSlider
{
    Q_OBJECT
    Q_PROPERTY(double transmissionRatio READ transmissionRatio WRITE setTransmissionRatio)
    Q_PROPERTY(bool wrapsAround READ wrapsAround WRITE setWrapsAround)
    Q_PROPERTY(int cogCount READ cogCount WRITE setCogCount)
    Q_PROPERTY(bool limitedDrag READ limitedDrag WRITE setLimitedDrag)

public:
    QtThumbWheel(QWidget *parent = 0);
    QtThumbWheel(int minimum, int maximum, int pageStep,
		 int value, Qt::Orientation orientation,
		 QWidget *parent = 0);

    ~QtThumbWheel();

    bool wrapsAround() const;
    void setWrapsAround(bool enabled);

    double transmissionRatio() const;
    virtual void setTransmissionRatio(double r);

    int cogCount() const;
    void setCogCount(int c);

    bool limitedDrag() const;
    void setLimitedDrag(bool enabled);

    QSize sizeHint() const;

protected:
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void resizeEvent(QResizeEvent *e);

    void paintEvent(QPaintEvent *);
    bool event(QEvent *e);

private:
    Q_DECLARE_PRIVATE(QtThumbWheel)
    Q_DISABLE_COPY(QtThumbWheel)
    QtThumbWheelPrivate *d_ptr;
};

#endif // QTTHUMBWHEEL_H
