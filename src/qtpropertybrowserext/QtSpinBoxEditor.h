#ifndef TSPINBOXEDITOR_H
#define TSPINBOXEDITOR_H

#include <QSlider>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QDebug>
#include <QFocusEvent>
#include <QMenu>


template<typename UnitType, class EditorType>
class TSpinBoxEditor : public QWidget
{
public:
    explicit TSpinBoxEditor(QWidget *parent = 0);

    UnitType value() const           { return m_editor->value(); }
    UnitType minimum() const         { return m_editor->minimum(); }
    UnitType maximum() const         { return m_editor->maximum(); }

    void setValue(UnitType val)      { m_editor->setValue(val); }
    void setMinimum(UnitType val);
    void setMaximum(UnitType val);
    void setSingleStep(UnitType val);

    int sliderMultiplier() const               { return m_sliderMultiplier; }
    void setSliderMultiplier(int val);

    bool ticksEnabled() const                  { return m_slider->tickPosition() != QSlider::NoTicks; }
    void enableTicks(bool on = true);

    void enableSlider(bool on = true);

    void expandVertically(bool on = true);

    void setEditorWidth(int width);
    void setUnitLabelWidth(int width);

    QString unitText() const                { return m_unitLabel->text(); }
    void setUnitText(const QString& val);

    void setDefaultValue(UnitType def);
    void disableDefaultValue();

    typedef QList<QPair<UnitType, QString> > PresetList;
    void setPredefinedValues(const PresetList& preset);
    void disablePredefinedValues();

protected /*Q_SLOTS*/:
    void slotEditorValueChanged(UnitType val);
    void slotSliderMoved(int val);
    void slotMinButtonClicked();
    void slotMaxButtonClicked();
    void slotDefaultButtonClicked();
    void slotPresetTriggered(QAction*);

protected:
    virtual void focusInEvent(QFocusEvent *e);
    virtual void UpdateConstrains() {}

    EditorType *m_editor;
    QSlider *m_slider;
    QToolButton *m_minButton, *m_maxButton;
    QLabel *m_unitLabel;

    int m_sliderMultiplier;
    bool m_sliderOn;

    UnitType m_defaultValue;
    QToolButton *m_defaultButton;

    PresetList m_presetList;
    QToolButton *m_presetButton;
    QMenu m_presetMenu;
};


template<typename UnitType, class EditorType>
TSpinBoxEditor<UnitType, EditorType>::TSpinBoxEditor(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);

    m_editor = new EditorType(this);
    mainLayout->addWidget(m_editor);
    setFocusProxy(m_editor);
    setFocusPolicy(Qt::StrongFocus);

    m_unitLabel = new QLabel(this);
    m_unitLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    m_unitLabel->hide();
    mainLayout->addWidget(m_unitLabel);

    m_defaultButton = new QToolButton(this);
    m_defaultButton->setText("<>");
    mainLayout->addWidget(m_defaultButton);
    setDefaultValue(0);

    m_minButton = new QToolButton(this);
    mainLayout->addWidget(m_minButton);

    m_slider = new QSlider(this);
    m_slider->setOrientation(Qt::Horizontal);
    mainLayout->addWidget(m_slider);

    m_maxButton = new QToolButton(this);
    mainLayout->addWidget(m_maxButton);

    m_presetButton = new QToolButton(this);
    m_presetButton->setText("... ");
    m_presetButton->setToolTip(tr("Pick from preset"));
    m_presetButton->setPopupMode(QToolButton::InstantPopup);
    mainLayout->addWidget(m_presetButton);
    m_presetButton->hide();


    enableSlider(false);

    m_sliderMultiplier = 1;

    UpdateConstrains();
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::setMinimum(UnitType val)
{
    m_editor->setMinimum(val);

    UpdateConstrains();
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::setMaximum(UnitType val)
{
    m_editor->setMaximum(val);

    UpdateConstrains();
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::setSingleStep(UnitType val)
{
    m_editor->setSingleStep(val);
    m_slider->setSingleStep(val);
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::setSliderMultiplier(int val)
{
    m_sliderMultiplier = val;

    UpdateConstrains();
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::enableTicks(bool on)
{
    m_slider->setTickPosition(on ? QSlider::TicksBelow : QSlider::NoTicks);
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::enableSlider(bool on)
{
    m_sliderOn = on;
    m_minButton->setVisible(on);
    m_maxButton->setVisible(on);
    m_slider->setVisible(on);
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::expandVertically(bool on)
{
    if (on){
        m_editor->setSizePolicy(m_editor->sizePolicy().horizontalPolicy(), QSizePolicy::Preferred);
        m_minButton->setSizePolicy(m_minButton->sizePolicy().horizontalPolicy(), QSizePolicy::Preferred);
        m_maxButton->setSizePolicy(m_maxButton->sizePolicy().horizontalPolicy(), QSizePolicy::Preferred);
    }
    else{
        m_editor->setSizePolicy(m_editor->sizePolicy().horizontalPolicy(), QSizePolicy::Maximum);
        m_minButton->setSizePolicy(m_minButton->sizePolicy().horizontalPolicy(), QSizePolicy::Maximum);
        m_maxButton->setSizePolicy(m_maxButton->sizePolicy().horizontalPolicy(), QSizePolicy::Maximum);
    }
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::setEditorWidth(int width)
{
    m_editor->setFixedWidth(width);
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::setUnitLabelWidth(int width)
{
    m_unitLabel->setFixedWidth(width);
    m_unitLabel->setVisible(width > 0);
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::setUnitText(const QString &val)
{
    m_unitLabel->setText(val);
    m_unitLabel->setVisible(!val.isEmpty());
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::setDefaultValue(UnitType def)
{
    m_defaultValue = def;
    m_defaultButton->setVisible(true);
    m_defaultButton->setToolTip(tr("Reset to %1").arg(def));
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::disableDefaultValue()
{
    m_defaultButton->setVisible(false);
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::setPredefinedValues(const PresetList& preset)
{
    if (!preset.count())
    {
        m_presetButton->setVisible(false);
        return;
    }

    m_presetMenu.clear();
    for (int i = 0; i < preset.count(); ++i)
    {
        const QPair<UnitType, QString>& pair = preset.at(i);
        UnitType value = pair.first;
        QString text = pair.second;
        if (text.isEmpty())
            text = QString::number(value);
        QAction *action = m_presetMenu.addAction(text);
        action->setData(value);
    }

    m_presetButton->setMenu(&m_presetMenu);
    m_presetButton->setVisible(true);
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::disablePredefinedValues()
{
    m_presetButton->setVisible(false);
}


// reimp

template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::focusInEvent(QFocusEvent *e)
{
    //m_editor->lineEdit()->event(e);

    if (e->reason() == Qt::TabFocusReason || e->reason() == Qt::BacktabFocusReason) {
        m_editor->selectAll();
    }

    QWidget::focusInEvent(e);
}


// protected slots

template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::slotEditorValueChanged(UnitType val)
{
    if (m_sliderMultiplier > 0)
    {
        m_slider->blockSignals(true);
        m_slider->setValue(val * m_sliderMultiplier);
        m_slider->blockSignals(false);
    }

    m_minButton->setEnabled(val != minimum());
    m_maxButton->setEnabled(val != maximum());
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::slotSliderMoved(int val)
{
    //qDebug() << "slider" << val << m_sliderMultiplier << val / m_sliderMultiplier;

    m_editor->setValue((double)val / (double)m_sliderMultiplier);
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::slotMinButtonClicked()
{
    m_editor->setValue(minimum());
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::slotMaxButtonClicked()
{
    m_editor->setValue(maximum());
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::slotDefaultButtonClicked()
{
    m_editor->setValue(m_defaultValue);
}


template<typename UnitType, class EditorType>
void TSpinBoxEditor<UnitType, EditorType>::slotPresetTriggered(QAction* action)
{
    m_editor->setValue(action->data().value<UnitType>());
}


// integer editor (based on QSpinBox)

class QtIntSpinBoxEditor : public TSpinBoxEditor<int, QSpinBox>
{
    Q_OBJECT

public:
    typedef TSpinBoxEditor<int, QSpinBox> Super;

    explicit QtIntSpinBoxEditor(QWidget *parent = 0);

protected:
    virtual void UpdateConstrains();

Q_SIGNALS:
    void valueChanged(int);

protected Q_SLOTS:
    void OnEditorValueChanged(int val);
    void OnSliderMoved(int val);
    void OnMinButtonClicked();
    void OnMaxButtonClicked();
    void OnDefaultButtonClicked();
    void OnPresetTriggered(QAction* action);
};


// real editor (based on QDoubleSpinBox)

class QtRealSpinBoxEditor : public TSpinBoxEditor<double, QDoubleSpinBox>
{
    Q_OBJECT

public:
    typedef TSpinBoxEditor<double, QDoubleSpinBox> Super;

    explicit QtRealSpinBoxEditor(QWidget *parent = 0);

    void setDecimals(int d);

protected:
    virtual void UpdateConstrains();

Q_SIGNALS:
    void valueChanged(double);

protected Q_SLOTS:
    void OnEditorValueChanged(double val);
    void OnSliderMoved(int val);
    void OnMinButtonClicked();
    void OnMaxButtonClicked();
    void OnDefaultButtonClicked();
    void OnPresetTriggered(QAction* action);
};


#endif // TSPINBOXEDITOR_H
