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

#include <QtGui/QImageReader>
#include <QtWidgets/QFileDialog>
#include <QtGui/QImage>
#include <QtWidgets/QMessageBox>

#include "imagetoolwidget.h"
#include "qtimagefilter.h"
#include "mirrorfilter.h"

ImageToolWidget::ImageToolWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);

    // Register our mirror filter.
    qtRegisterImageFilter<MirrorFilter>(QLatin1String("MirrorFilter"));

    // Iterate through all registered filters and append them to the vector
    QStringList filters = QtImageFilterFactory::imageFilterList();
    for (int i = 0; i < filters.count(); ++i) {
        m_imageFilters += QtImageFilterFactory::createImageFilter(filters[i]);
    }

    for (int i = 0; i < m_imageFilters.count(); ++i) {
        ui.FiltersCombo->addItem(m_imageFilters[i]->name());
    }

    ui.FiltersCombo->insertItem(0, QLatin1String("--Choose filter--"));

    m_imageFilters.prepend((QtImageFilter*)0);

    QObject::connect(ui.LoadButton, SIGNAL(clicked()), this, SLOT(loadImage()));
    QObject::connect(ui.ReloadButton, SIGNAL(clicked()), this, SLOT(reloadImage()));
    QObject::connect(ui.FilterButton, SIGNAL(clicked()), this, SLOT(filterImage()));
    QObject::connect(ui.FiltersCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(filterIndexChanged(int)));

    ui.FiltersCombo->setCurrentIndex(0);
    m_currentFilename = QLatin1String("images/qtlogo.png");
    reloadImage();
}

ImageToolWidget::~ImageToolWidget()
{

}

void ImageToolWidget::loadImage()
{
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    QString strFormats;
    int i;
    for (i = 0; i < formats.size(); i++) {
        if (i != 0) strFormats+=" ";
        strFormats += QString("*.") + formats.at(i);
    }
    strFormats = "Images (" + strFormats + ")";
    QString s = QFileDialog::getOpenFileName(
            this,
            "Choose a file",
            QString(),
            strFormats);
    QImage img;
    if(img.load(s)) {
        m_currentFilename = s;
        QPixmap pixmap = QPixmap::fromImage(img);
        setPixmapAndResize(pixmap);
    }
}

void ImageToolWidget::reloadImage()
{
    QImage img;
    if(img.load(m_currentFilename)) {
        QPixmap pixmap = QPixmap::fromImage(img);
        setPixmapAndResize(pixmap);
    }
}

void ImageToolWidget::filterIndexChanged(int index)
{
    if (index == 0 || index >= m_imageFilters.count()) {
        ui.FilterButton->setToolTip(QLatin1String("No image filter chosen"));
        ui.FilterButton->setEnabled(false);
        ui.gbBorderPolicy->setVisible( false );
        ui.gbChannels->setVisible( false );
        ui.gbMirror->setVisible( false );
    } else {
        QtImageFilter *filter = m_imageFilters[index];
        ui.FilterButton->setToolTip(filter->description());
        ui.FilterButton->setEnabled(true);
        ui.gbBorderPolicy->setVisible( filter->supportsOption(QtImageFilter::FilterBorderPolicy)  );
        ui.gbChannels->setVisible( filter->supportsOption(QtImageFilter::FilterChannels)  );
        ui.gbMirror->setVisible( filter->supportsOption(MirrorFilter::MirrorHorizontal) || filter->supportsOption(MirrorFilter::MirrorVertical) );
    }
}

void ImageToolWidget::filterImage()
{
    if (ui.PixmapLabel->pixmap() == 0) {
        QMessageBox::information(this, "QImageTool", "Sorry, you must load an image first\n");
    } else {
        setCursor(Qt::WaitCursor);
        QImage imgToFilter = ui.PixmapLabel->pixmap()->toImage();
        QtImageFilter *filter = m_imageFilters[ui.FiltersCombo->currentIndex()];
        if (filter->name() == "Punch") {
            filter->setOption(QtImageFilter::Radius, qMin(imgToFilter.width(), imgToFilter.height())/2);
            filter->setOption(QtImageFilter::Center, QPointF(imgToFilter.width()/2.0,imgToFilter.height()/2.0));
            filter->setOption(QtImageFilter::Force, 0.5);
        }else if (filter->name() == "ConvolutionFilter") {
            // A simple mean filter just to demonstrate that we can add our own kernels.
            static int kernelElements[9] =
            {    1,  1,  1,
                 1,  1,  1,
                 1,  1,  1 };
            QtConvolutionKernelMatrix kernel(kernelElements, 3, 3);
            QVariant value;
            qVariantSetValue<QtConvolutionKernelMatrix>(value, kernel);
            filter->setOption(QtImageFilter::ConvolutionKernelMatrix, value);

        }
        if (filter->supportsOption(MirrorFilter::MirrorHorizontal))
            filter->setOption(MirrorFilter::MirrorHorizontal, ui.ckHorizontal->isChecked() );
        if (filter->supportsOption(MirrorFilter::MirrorVertical))
            filter->setOption(MirrorFilter::MirrorVertical, ui.ckVertical->isChecked());

        if (filter->supportsOption(QtImageFilter::FilterChannels)) {
            QString rgba = ui.ckRed->isChecked() ? "r" : "";
            rgba+= ui.ckGreen->isChecked() ? "g" : "";
            rgba+= ui.ckBlue->isChecked() ? "b" : "";
            rgba+= ui.ckAlpha->isChecked() ? "a" : "";
            filter->setOption(QtImageFilter::FilterChannels, rgba);
        }

        if (filter->supportsOption(QtImageFilter::FilterBorderPolicy)) {
            QString borderPolicy;
            if (ui.rbExtend->isChecked()) borderPolicy = "Extend";
            else if (ui.rbMirror->isChecked()) borderPolicy = "Mirror";
            else borderPolicy = "Wrap";
            filter->setOption(QtImageFilter::FilterBorderPolicy, borderPolicy);
        }

        imgToFilter = filter->apply(imgToFilter);
        setCursor(Qt::ArrowCursor);
        setPixmapAndResize(QPixmap::fromImage(imgToFilter));
    }
}

void ImageToolWidget::setPixmapAndResize(const QPixmap &pixmap)
{
    ui.PixmapLabel->setPixmap(pixmap);
    ui.PixmapLabel->resize(pixmap.size());
}
