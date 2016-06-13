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
#include <QDialog>
#include <QtGui/QPixmap>
#include <QLayout>
#include <QLabel>
#include <QtGui/QMatrix>

#include <qtthumbwheel.h>

class XForm : public QDialog {
    Q_OBJECT

public:
    XForm(const QString filename, QWidget *parent = 0)
	: QDialog(parent), img(filename)
    {
	// Create one thumb-wheel for rotation, range from 0 to 359
	// degrees, default value 0 and vertical orientation. Then
	// allow the thumbwheel to wrap around.
        QtThumbWheel *rotwheel = new QtThumbWheel(0, 359, 10, 0, Qt::Vertical, this);
	rotwheel->setWrapsAround(true);

	// Create a second thumb-wheel, range from 0 to 200 percent,
	// default value 100 percent, horizontal orientation. This
	// wheel should not wrap. Set a high transmission ratio to allow for
        // finegrained control, and allow the wheel to keep turning
        // when the mouse is dragged out of the widget.
	QtThumbWheel *zoomwheel = new QtThumbWheel(0, 200, 10, 100, Qt::Horizontal, this);
        zoomwheel->setTransmissionRatio(4.0);
        zoomwheel->setLimitedDrag(false);

	// Connect the two thumb-wheels' valueChanged(int) signals to
	// slots.
	connect(rotwheel, SIGNAL(valueChanged(int)), this, SLOT(setRotation(int)));
	connect(zoomwheel, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));

	// Put the pixmap inside a center aligned label.
	label = new QLabel(this);
	label->setAlignment(Qt::AlignCenter);
	label->setMinimumSize(img.size());
        label->setPixmap(QPixmap::fromImage(img));

	// Align all three widgets with a grid-layout.
        QGridLayout *grid = new QGridLayout(this);
	grid->addWidget(rotwheel, 0, 0);
	grid->addWidget(zoomwheel, 1, 1);
	grid->addWidget(label, 0, 1);

	zom = 1.0;
	rot = 0.0;
    }

public slots:
    void setRotation(int r)
    {
        rot = double(r);
        updateImage();
    }

    void setZoom(int z)
    {
        zom = double(z) / 100.0;
	updateImage();
    }

protected:
    void updateImage()
    {
	QMatrix matrix;
	matrix.rotate(double(rot));
	matrix.scale(double(zom), double(zom));

	QImage newImg = img.transformed(matrix, Qt::SmoothTransformation);
	if (!newImg.isNull())
	    label->setPixmap(QPixmap::fromImage(newImg));
	else
	    label->setPixmap(QPixmap());
    }

private:
    double rot;
    double zom;
    QLabel *label;
    QImage img;
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    XForm r(":/logo.png");

    r.show();

    return app.exec();
}

#include "main.moc"
