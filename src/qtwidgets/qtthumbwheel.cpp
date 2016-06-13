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

#include "qtthumbwheel.h"

#include <qdrawutil.h>
#include <QPainter>
#include <QPixmap>
#include <math.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QColor>

enum Boundary {
    Minimum,
    Maximum
};

class QtThumbWheelPrivate
{
    Q_DECLARE_PUBLIC(QtThumbWheel)
public:
    inline QtThumbWheelPrivate(QtThumbWheel *qptr)
    {
        q_ptr = qptr;
        init();
    }

    inline void init()
    {
        Q_Q(QtThumbWheel);
        wrapsAround = false;
        limitedDrag = true;
        q->setAttribute(Qt::WA_NoBackground, true);
        cogs = 17;
        mousePressed = false;
        pressedAt = -1;
        transmissionRatio = 1.00;
        q->setFocusPolicy(Qt::WheelFocus);
        pix = QPixmap(q->size());
    }
    void generateBackground();
    int valueFromPosition(const QPoint &) const;

    inline void setValue(int val)
    {
        Q_Q(QtThumbWheel);
        if (wrapsAround) {
            const int range = q->maximum() - q->minimum();
            while (val < q->minimum())
                val += range;
            while (val > q->maximum())
                val -= range;
        } else {
            if (val > q->maximum())
                val = q->maximum();
            if (val < q->minimum())
                val = q->minimum();
        }
        q->setValue(val);
    }

    int cogs;
    bool wrapsAround;
    bool limitedDrag;
    double transmissionRatio;
    int pressedAt;
    bool mousePressed;

    QPixmap pix;
    QPixmap background;
    bool mustGenerateBackground;
    QtThumbWheel *q_ptr;
};


static const double m_pi = 3.14159265358979323846;
static const double rad_factor = 180.0 / m_pi;

/*! \class QtThumbWheel qtthumbwheel.h

    \brief The QtThumbWheel widget provides a vertical or horizontal
    thumb-wheel widget.

    The widget is often used in 2D and 3D applications to control
    rotation, zooming and scrolling. The rotation of the thumb-wheel
    is translated into an integer value within a specified range.

    QtThumbWheel inherits QAbstractSlider, which provides the
    "integer" component of the thumb-wheel. setRange() and value() are
    likely to be used by practically all thumb-wheel users; see the
    QAbstractSlider documentation for information about the many other
    functions that class provides.

    The main functions provided by the thumb-wheel are the
    transmission ratio, cog and wrap around controls; you can use
    setTransmissionRatio() to adjust the ratio of the speed of the
    wheel to the speed at which the value changes, and setCogCount()
    to set the number of cogs on the wheel. Use setOrientation() to
    indicate whether the thumb-wheel is vertically or horizontally
    oriented.

    A thumb-wheel accepts focus on Tab and supports the mouse wheel.
    It also provides a keyboard interface.

    \img qtthumbwheel-h.png
*/


/*!
    Constructs a QtThumbWheel object. The \a parent parameter
    is passed to the QAbstractSlider constructor.
*/
QtThumbWheel::QtThumbWheel(QWidget *parent)
    : QAbstractSlider(parent), d_ptr(new QtThumbWheelPrivate(this))
{
    setOrientation(Qt::Horizontal);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

/*!
    Constructs a QtThumbWheel object. \a minimum and \a maximum set
    the initial minimum and maximum values, and \a pageStep sets the
    size of each page step. \a value sets the initial value and \a
    orientation is either Vertical or Horizontal. The \a parent
    parameter is passed to the QAbstractSlider constructor.
*/
QtThumbWheel::QtThumbWheel(int minimum, int maximum, int pageStep,
			   int value, Qt::Orientation orientation,
			   QWidget *parent)
    : QAbstractSlider(parent), d_ptr(new QtThumbWheelPrivate(this))
{
    setRange(minimum, maximum);
    setValue(value);
    setSingleStep(1);
    setPageStep(pageStep);
    setOrientation(orientation);
    if (orientation == Qt::Horizontal)
      setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    else
      setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

/*!
    Destroys the QtThumbWheel.
*/
QtThumbWheel::~QtThumbWheel()
{
    delete d_ptr;
}

/*!
    \property QtThumbWheel::wrapsAround

    \brief Whether the values should wrap around.

    If enabled, when the value exceeds maximum, it is reset to
    minimum, and when the value goes below minimum, it is reset to
    maximum value. This is useful when the thumb-wheel should not
    stop at the minimum and maximum value boundaries.
*/
void QtThumbWheel::setWrapsAround(bool enabled)
{
    Q_D(QtThumbWheel);
    d->wrapsAround = enabled;
}

bool QtThumbWheel::wrapsAround() const
{
    Q_D(const QtThumbWheel);
    return d->wrapsAround;
}


/*!
    \property QtThumbWheel::limitedDrag

    \brief Whether drags out of the wheel will keep turning it.

    If enabled, a click and drag operation will not turn the wheel
    further once the mouse goes outside the minimum or maximum edges
    of the QtThumbWheel widget. Set this property to disabled to allow
    for long drags, i.e. large value changes with a single mouse
    move. By default this property is enabled, i.e. drags are limited.
*/

void QtThumbWheel::setLimitedDrag(bool enabled)
{
    Q_D(QtThumbWheel);
    d->limitedDrag = enabled;
}

bool QtThumbWheel::limitedDrag() const
{
    Q_D(const QtThumbWheel);
    return d->limitedDrag;
}

/*!
    \property QtThumbWheel::cogCount

    \brief Sets the number of cogs on the thumb-wheel(the default is
    17).

    Use this property to adapt the number of cogs to the size of the
    wheel. The property only has a visual effect.
*/
void QtThumbWheel::setCogCount(int c)
{
    Q_D(QtThumbWheel);
    if (c > 0)
        d->cogs = c;
    update();
}

int QtThumbWheel::cogCount() const
{
    Q_D(const QtThumbWheel);
    return d->cogs;
}

/*!
    \property QtThumbWheel::transmissionRatio

    \brief The transmission ratio between the thumb-wheel and the
    value range.

    The default ratio is 1.0, which means that the user must turn the
    wheel 360 degress(one full spin) to go from minimum to
    maximum. Giving a ratio of 2.0 will require the user to spin the
    wheel twice to cross the same range, and a value of 0.5 will
    require only half a turn.
*/
void QtThumbWheel::setTransmissionRatio(double r)
{
    Q_D(QtThumbWheel);
    d->transmissionRatio = r;
}

double QtThumbWheel::transmissionRatio() const
{
    Q_D(const QtThumbWheel);
    return d->transmissionRatio;
}

/*!
    \reimp
*/
void QtThumbWheel::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Left:
	if (orientation() == Qt::Horizontal)
            triggerAction(SliderSingleStepSub);
	break;
    case Qt::Key_Right:
	if (orientation() == Qt::Horizontal)
            triggerAction(SliderSingleStepAdd);
	break;
    case Qt::Key_Up:
	if (orientation() == Qt::Vertical)
            triggerAction(SliderSingleStepSub);
	break;
    case Qt::Key_Down:
	if (orientation() == Qt::Vertical)
            triggerAction(SliderSingleStepAdd);
	break;
    case Qt::Key_PageUp:
        triggerAction(SliderPageStepSub);
	break;
    case Qt::Key_PageDown:
        triggerAction(SliderPageStepAdd);
	break;
    case Qt::Key_Home:
	setValue(minimum());
	break;
    case Qt::Key_End:
	setValue(maximum());
	break;
    default:
	e->ignore();
	return;
    };
}

/*!
    \reimp
*/
void QtThumbWheel::mousePressEvent(QMouseEvent *e)
{
    Q_D(QtThumbWheel);
    if (e->button() == Qt::LeftButton) {
	d->mousePressed = true;
	d->pressedAt = d->valueFromPosition(e->pos());
    }
}

/*!
    \reimp
*/
void QtThumbWheel::mouseReleaseEvent(QMouseEvent *e)
{
    Q_D(QtThumbWheel);
    int movedTo = d->valueFromPosition(e->pos());
    d->setValue(value() + movedTo - d->pressedAt);
    d->pressedAt = movedTo;
    d->mousePressed = false;
}

/*!
    \reimp
*/
void QtThumbWheel::mouseMoveEvent(QMouseEvent *e)
{
    Q_D(QtThumbWheel);
    if (!d->mousePressed)
	return;
    if (hasTracking()) {
	int movedTo = d->valueFromPosition(e->pos());
	d->setValue(value() + movedTo - d->pressedAt);
	d->pressedAt = movedTo;
    }
}

/*!
    \reimp
*/
void QtThumbWheel::wheelEvent(QWheelEvent *e)
{
    int step = (e->modifiers() & Qt::ControlModifier) ? singleStep() : pageStep();
    setValue(value() - e->delta()*step/ (150*4));
    e->accept();
}

/*!
    \reimp
*/
void QtThumbWheel::resizeEvent(QResizeEvent *)
{
    Q_D(QtThumbWheel);
    d->mustGenerateBackground = true;
}

/*!
    \reimp
*/
bool QtThumbWheel::event(QEvent *e)
{
    Q_D(QtThumbWheel);
    if (e->type() == QEvent::PaletteChange)
        d->mustGenerateBackground = true;
    return QAbstractSlider::event(e);
}

void QtThumbWheelPrivate::generateBackground()
{
    Q_Q(QtThumbWheel);

    background = pix = QPixmap(q->size());
    pix.fill(Qt::green);
    QPainter p(&background);

    background.fill(Qt::red);

    QRect cr = q->contentsRect();

    int h, s, v;
    q->palette().background().color().getHsv(&h, &s, &v);

    QColor cblack;
    cblack.setHsv(h, s, 0);

    if (q->orientation() == Qt::Horizontal) {
        int y0 = cr.y() + 1;
        int y1 = cr.bottom() - 1;
        int len = q->width();

        for (int i = 0; i < len; ++i) {
            double _shade = 0.5 + fabs(sin(m_pi * (double(i) / double(len))) / 2.0);
            if (_shade > 1.0) _shade = 1.0;
            int tmpc = int(1.2 * double(v) * _shade);
            if (tmpc > 255) tmpc = 255;
            if (tmpc < 0) tmpc = 0;

            // Five shaded color compontents
            QColor clight;
            clight.setHsv(h, s, int(double(v) * _shade));
            QColor cdark;
            cdark.setHsv(h, s, int(0.5  * double(v) * _shade));
            QColor cdim;
            cdim.setHsv(h, s, int(0.75 * double(v) * _shade));

            // Draw the top black line
            p.setPen(QPen(cblack));
            p.drawPoint(i, y0);

            // Draw the top bright border
            p.setPen(QPen(clight));
            p.drawLine(i, y0 + 1, i, y0 + 1 + 2);

            p.setPen(QPen(cdark));
            p.drawPoint(i, y0 + 3);
            p.setPen(QPen(cdim));
            p.drawLine(i, y0 + 4, i, y1 - 2 - 1);

            // Draw the botton bright border
            p.setPen(QPen(clight));
            p.drawLine(i, y1 - 2, i, y1 - 1);

            // Draw the bottom black line
            p.setPen(QPen(cblack));
            p.drawPoint(i, y1);
        }
    } else {
        // vertical orientation
	int x0 = cr.x() + 1;
	int x1 = cr.right() - 1;
        int len = q->height();

        for (int i = 0; i < len; ++i) {
            double _shade = 0.5 + fabs(sin(m_pi * (double(i) / double(len))) / 2.0);
            if (_shade > 1.0) _shade = 1.0;
            int tmpc = int(1.2 * double(v) * _shade);
            if (tmpc > 255) tmpc = 255;
            if (tmpc < 0) tmpc = 0;

            // Five shaded color compontents
            QColor clight;
            clight.setHsv(h, s, int(double(v) * _shade));
            QColor cdark;
            cdark.setHsv(h, s, int(0.5  * double(v) * _shade));
            QColor cdim;
            cdim.setHsv(h, s, int(0.75 * double(v) * _shade));

            // Draw the left black line
            p.setPen(QPen(cblack));
            p.drawPoint(x0, i);

            // Draw the left bright border
            p.setPen(QPen(clight));
            p.drawLine(x0 + 1, i, x0 + 1 + 2, i);

            p.setPen(QPen(cdark));
            p.drawPoint(x0 + 3, i);
            p.setPen(QPen(cdim));
            p.drawLine(x0 + 4, i, x1 - 2 - 1, i);

            // Draw the right bright border
            p.setPen(QPen(clight));
            p.drawLine(x1 - 1 - 2, i, x1 - 1, i);

            // Draw the right black line
            p.setPen(QPen(cblack));
            p.drawPoint(x1, i);
        }
    }
}

/*!
    \reimp
*/
void QtThumbWheel::paintEvent(QPaintEvent *)
{
    Q_D(QtThumbWheel);
    if (d->mustGenerateBackground) {
        d->mustGenerateBackground = false;
        d->generateBackground();
    }

    QBrush brush = palette().background();

    QPainter pt(&d->pix);
    pt.drawPixmap(QPoint(0, 0), d->background);

    QRect cr = contentsRect();

    double delta = m_pi / double(d->cogs);
    double alpha = 2 * m_pi * double(value() - minimum()) / double(maximum() - minimum());

    alpha = fmod(alpha, delta);

    int h, s, v;
    palette().background().color().getHsv(&h, &s, &v);

    if (orientation() == Qt::Horizontal) {
	double r = 0.5 * double(cr.width());
	int y0 = cr.y() + 1;
	int y1 = cr.bottom() - 1;

        // For all n segments
	for (int i = 0; i <= d->cogs; i++) {
	    // _x is the previous segment; x is the current one.
	    double _t = r * (1.0 - cos(delta * double(i - 1) + alpha));
	    double _t2 = r * (1.0 - cos(delta * double(i) + alpha));

	    int _x = cr.x() + ((i == 0) ? 0 : int(_t));
	    int x = (i == d->cogs) ? cr.right() : cr.x() + int(_t2);

	    // Draw a shaded transition from the former segment to, but
	    // not uncluding, the new one.
	    for (int ii = _x; ii <= x; ++ii) {

                // The shade is simply a sine of x/cr.width(). Needs
		// to be ceiled because of rounding errors in double
		// cast, fabs and sin.
		double _shade = 0.5 + fabs(sin(m_pi * (double(ii)/double(cr.width()))) / 2.0);
		if (_shade > 1.0) _shade = 1.0;

		int tmpc = int(1.2 * double(v) * _shade);
		if (tmpc > 255) tmpc = 255;
		if (tmpc < 0) tmpc = 0;

		// Five shaded color compontents
		QColor cbright;
                cbright.setHsv(h, s, tmpc);
		QColor clight;
                clight.setHsv(h, s, int(double(v) * _shade));
		QColor cdim;
                cdim.setHsv(h, s, int(0.75 * double(v) * _shade));
		QColor cdark;
                cdark.setHsv(h, s, int(0.5  * double(v) * _shade));

		// Draw the sunken box, four vertical segments.
		int h_1 = y0 + 1 + 2 ;
		int h_2 = y0 + 1 + 2 + 1;
		int h_3 = y1 - 1 - 2 - 1;
		int h_4 = y1 - 1 - 2;

		// 0 1 2 3 4 5 6
		// --------------+
		// 0 0 0 0 0 0 0 |
		// 4 4 4 4 4 4 4 |
		// 4 4 4 4 4 4 4 | h_1
		// 4 1 1 1 1 1 1 | h_2
		// 4 3 2 2 2 2 1 |
		// 4 3 2 2 2 2 1 |
		// 4 3 2 2 2 2 1 |
		// 4 3 2 2 2 2 1 |
		// 4 3 2 2 2 2 1 |
		// 4 3 2 2 2 2 1 | h_3
		// 4 3 2 2 2 2 2 | h_4
		// 4 4 4 4 4 4 4 |
		// 4 4 4 4 4 4 4 |
		// 0 0 0 0 0 0 0 |

		if (ii == _x) {
                    pt.setPen(QPen(clight));
                    pt.drawLine(ii, h_1, ii, h_4);
		} else if (ii == _x + 1) {
		    pt.setPen(QPen(cdark));
		    pt.drawLine(ii, h_1, ii, h_2);
		    pt.setPen(QPen(cbright));
		    pt.drawLine(ii, h_2, ii, h_4);
		} else if (ii > x - 2) {
		    pt.setPen(QPen(cdark));
		    pt.drawLine(ii, h_1, ii, h_3);
		    pt.setPen(QPen(cdim));
		    pt.drawLine(ii, h_3, ii, h_4);
		}
	    }
	}
    } else {
        // vertical orientation
	double r = 0.5 * double(cr.height());
	int x0 = cr.x() + 1;
	int x1 = cr.right() - 1;

        // For all n segments
	for (int i = 0; i <= d->cogs; i++) {
	    // _y is the previous segment; y is the current one.
	    double _t = r * (1.0 - cos(delta * double(i - 1) + alpha));
	    double _t2 = r * (1.0 - cos(delta * double(i) + alpha));

	    int _y = cr.y() + ((i == 0) ? 0 : int(_t));
	    int y = (i == d->cogs) ? cr.bottom() : cr.y() + int(_t2);

	    // Draw a shaded transition from the former segment to, but
	    // not uncluding, the new one.
	    for (int ii = _y; ii < y; ++ii) {

                // The shade is simply a sine of x/cr.width(). Needs
		// to be ceiled because of rounding errors in double
		// cast, fabs and sin.
		double _shade = 0.5 + fabs(sin(m_pi * (double(ii)/double(cr.height()))) / 2.0);
		if (_shade > 1.0) _shade = 1.0;

		int tmpc = int(1.2 * double(v) * _shade);
		if (tmpc > 255) tmpc = 255;
		if (tmpc < 0) tmpc = 0;

		// Five shaded color compontents
		QColor cbright;
                cbright.setHsv(h, s, tmpc);
		QColor clight;
                clight.setHsv(h, s, int(double(v) * _shade));
		QColor cdim;
                cdim.setHsv(h, s, int(0.75 * double(v) * _shade));
		QColor cdark;
                cdark.setHsv(h, s, int(0.5  * double(v) * _shade));

		// Draw the sunken box, four vertical segments.
		int h_1 = x0 + 1 + 2 ;
		int h_2 = x0 + 1 + 2 + 1;
		int h_3 = x1 - 1 - 2 - 1;
		int h_4 = x1 - 1 - 2;

                //      hh     hh
		//      __     __
                //      12     34
                //   +--------------
                // 0 |04444444444444
                // 1 |04413333333440
                // 2 |04412222222440
                // 3 |04412222222440
                // 4 |04412222222440
                // 5 |04412222222440
                // 6 |04411111112440

		if (ii == _y) {
		    pt.setPen(QPen(clight));
		    pt.drawLine(h_1, ii, h_4, ii);
		} else if (ii == _y + 1) {
		    pt.setPen(QPen(cdark));
		    pt.drawLine(h_1, ii, h_2, ii);
		    pt.setPen(QPen(cbright));
		    pt.drawLine(h_2, ii, h_4, ii);
		} else if (ii > y - 2) {
		    pt.setPen(QPen(cdark));
		    pt.drawLine(h_1, ii, h_3, ii);
		    pt.setPen(QPen(cdim));
		    pt.drawLine(h_3, ii, h_4, ii);
		}
	    }
	}
    }

    qDrawShadePanel(&pt, cr, palette());
    QPainter painter(this);
    painter.drawPixmap(QPoint(0, 0), d->pix);
}

/*!
    \internal

    Determines the value from the "rotation" of the thumb-wheel.
 */
int QtThumbWheelPrivate::valueFromPosition(const QPoint &p) const
{
    Q_Q(const QtThumbWheel);
    QRect wrec = q->contentsRect();
    int pos, min, max;

    if (q->orientation() == Qt::Horizontal) {
	pos = p.x();
	min = wrec.left();
	max = wrec.right();
    } else {
	pos = p.y();
	min = wrec.top();
	max = wrec.bottom();
    }

    if (!limitedDrag && (pos < min || pos > max)) {
        qreal pixPerUnit = ((max - min) * 2.0 * transmissionRatio) / (q->maximum() - q->minimum());
        if (pos < min)
            return qRound(q->minimum() - (min-pos) / pixPerUnit);
        else
            return qRound((q->maximum() / (2.0 * transmissionRatio)) + (pos-max) / pixPerUnit);
    }

    double alpha;
    if (pos < min)
	alpha = 0;
    else if (pos > max)
	alpha = m_pi;
    else
	alpha = acos(1.0 - 2.0 * double(pos - min) / double(max - min));// ### taylor

    double deg = alpha * rad_factor / transmissionRatio;
    return q->minimum() + int(double(q->maximum() - q->minimum()) * deg / 360.0);
}

/*!
    \reimp
*/
QSize QtThumbWheel::sizeHint() const
{
    return (orientation() == Qt::Horizontal) ? QSize(130, 20) : QSize(20, 130);
}

