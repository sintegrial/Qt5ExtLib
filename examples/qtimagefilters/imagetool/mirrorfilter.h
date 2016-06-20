/****************************************************************************
**
** This file is part of a Qt Solutions component.
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** 
** Contact:  Qt Software Information (qt-info@nokia.com)
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
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
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
** contact the sales department at qt-sales@nokia.com.
** 
****************************************************************************/

#ifndef MIRRORFILTER_H
#define MIRRORFILTER_H

#include "qtimagefilter.h"

class MirrorFilter : public QtImageFilter {
public:
    enum MirrorFilterOption {
        MirrorHorizontal = UserOption,
        MirrorVertical
    };

    MirrorFilter()
    {
        m_mirrorVertical = false;
        m_mirrorHorizontal = true;
    }

    QImage apply(const QImage &img, const QRect& clipRect /*= QRect()*/ ) const
    {
        Q_UNUSED(clipRect);
        return img.mirrored(m_mirrorHorizontal, m_mirrorVertical);
    }

    QString name() const { return QLatin1String("Mirror"); }

    QVariant option(int filteroption) const
    {
        switch ((MirrorFilterOption)filteroption) {
        case MirrorVertical:
            return QVariant(m_mirrorVertical);
        break;
        case MirrorHorizontal:
            return QVariant(m_mirrorHorizontal);
        break;
        default:
            break;
        }
        return QVariant();
    }

    bool setOption(int filteroption, const QVariant &value)
    {
        switch ((MirrorFilterOption)filteroption) {
            case MirrorVertical:
                m_mirrorVertical = value.toBool();
            break;
            case MirrorHorizontal:
                m_mirrorHorizontal = value.toBool();
            break;
            default:
                break;
        }
        return true;
    }

    bool supportsOption(int option) const
    {
        bool supports = false;
        switch ((MirrorFilterOption)option) {
            case MirrorVertical:
            case MirrorHorizontal:
                supports = true;
            break;
            default:
                break;
        }
        return supports;
    }

    QString description() const { return QObject::tr("A simple mirroring filter."); }

private:
    bool m_mirrorVertical;
    bool m_mirrorHorizontal;
};
#endif  /* MIRRORFILTER_H */
