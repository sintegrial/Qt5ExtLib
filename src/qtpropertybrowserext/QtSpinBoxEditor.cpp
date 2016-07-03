#include "QtSpinBoxEditor.h"

#include <cfloat>
#include <limits>


// QtIntSpinBoxEditor

QtIntSpinBoxEditor::QtIntSpinBoxEditor(QWidget *parent) : Super(parent)
{
    connect(m_editor, SIGNAL(valueChanged(int)), this, SLOT(OnEditorValueChanged(int)));
    connect(m_editor, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(OnSliderMoved(int)));
    connect(m_minButton, SIGNAL(clicked()), this, SLOT(OnMinButtonClicked()));
    connect(m_maxButton, SIGNAL(clicked()), this, SLOT(OnMaxButtonClicked()));
}


// protected members

void QtIntSpinBoxEditor::UpdateConstrains()
{
    m_slider->blockSignals(true);

    double vmin = m_editor->minimum() * m_sliderMultiplier;
    double vmax = m_editor->maximum() * m_sliderMultiplier;
    if (vmin <= -INT_MAX || vmax >= INT_MAX){
        m_sliderMultiplier = 1;
        vmin = m_editor->minimum();
        vmax = m_editor->maximum();
    }

    //qDebug() << "min" << m_editor->minimum();

    if (m_editor->minimum() > -INT_MAX){
        m_minButton->setText(QString::number(m_editor->minimum()));
    } else {
        m_minButton->setText("min<");
        vmin = -INT_MAX;
    }


    if (m_editor->maximum() < INT_MAX){
        m_maxButton->setText(QString::number(m_editor->maximum()));
    } else {
        m_maxButton->setText(">max");
        vmax = INT_MAX;
    }

    m_slider->setRange(vmin, vmax);

    m_slider->blockSignals(false);
}


void QtIntSpinBoxEditor::OnEditorValueChanged(int val)
{
    slotEditorValueChanged(val);
}

void QtIntSpinBoxEditor::OnSliderMoved(int val)
{
    slotSliderMoved(val);
}

void QtIntSpinBoxEditor::OnMinButtonClicked()
{
    slotMinButtonClicked();
}

void QtIntSpinBoxEditor::OnMaxButtonClicked()
{
    slotMaxButtonClicked();
}


// QtRealSpinBoxEditor

QtRealSpinBoxEditor::QtRealSpinBoxEditor(QWidget *parent) : Super(parent)
{
    connect(m_editor, SIGNAL(valueChanged(double)), this, SLOT(OnEditorValueChanged(double)));
    connect(m_editor, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(OnSliderMoved(int)));
    connect(m_minButton, SIGNAL(clicked()), this, SLOT(OnMinButtonClicked()));
    connect(m_maxButton, SIGNAL(clicked()), this, SLOT(OnMaxButtonClicked()));

    setSliderMultiplier(pow(10, m_editor->decimals()));

    UpdateConstrains();
}

void QtRealSpinBoxEditor::setDecimals(int d)
{
    m_editor->setDecimals(d);

    setSliderMultiplier(pow(10, m_editor->decimals()));

    UpdateConstrains();
}


// protected members

void QtRealSpinBoxEditor::UpdateConstrains()
{
    m_slider->blockSignals(true);

    double vmin = m_editor->minimum();
    double vmax = m_editor->maximum();

    m_minButton->setText(QString::number(m_editor->minimum()));
    m_maxButton->setText(QString::number(m_editor->maximum()));

    if (vmin <= -INT_MAX || vmax >= INT_MAX){
        m_sliderMultiplier = -1;

        if (vmin <= -INT_MAX){
            m_minButton->setText("min<");
        }

        if (vmax >= INT_MAX){
            m_maxButton->setText(">max");
        }

        m_slider->setEnabled(false);
        m_slider->setVisible(false);
    }
    else{
        vmin = m_editor->minimum() * m_sliderMultiplier;
        vmax = m_editor->maximum() * m_sliderMultiplier;

        m_slider->setEnabled(true);
        m_slider->setVisible(true);

        m_slider->setRange(vmin, vmax);
    }

    m_slider->blockSignals(false);
}


void QtRealSpinBoxEditor::OnEditorValueChanged(double val)
{
    slotEditorValueChanged(val);
}

void QtRealSpinBoxEditor::OnSliderMoved(int val)
{
    slotSliderMoved(val);
}

void QtRealSpinBoxEditor::OnMinButtonClicked()
{
    slotMinButtonClicked();
}

void QtRealSpinBoxEditor::OnMaxButtonClicked()
{
    slotMaxButtonClicked();
}
