#ifndef QTVARIANTEDITORFACTORYEXT_H
#define QTVARIANTEDITORFACTORYEXT_H


#include "qtvariantproperty.h"


class QtVariantEditorFactoryExt: public QtVariantEditorFactory
{
	Q_OBJECT

public:
    QtVariantEditorFactoryExt(QObject *parent = 0)
		: QtVariantEditorFactory(parent)
	{ }

    virtual ~QtVariantEditorFactoryExt();

protected:
	virtual void connectPropertyManager(QtVariantPropertyManager *manager);
	virtual void disconnectPropertyManager(QtVariantPropertyManager *manager);

	virtual QWidget *createEditor(QtVariantPropertyManager *manager, QtProperty *property, QWidget *parent);
		
private Q_SLOTS:
	void slotPropertyChanged(QtProperty *property, const QVariant &value);
	void slotPropertyAttributeChanged(QtProperty *property, const QString &attribute, const QVariant &value);
	void slotSetValue(const QString &value);
    void slotSetValue(int);
    void slotSetValue(double);
    void slotEditorDestroyed(QObject *object);
			
private:
    void doSetValue(const QVariant& value);

    QMap<QtProperty*, QList<QWidget*> > m_createdEditors;
    QMap<QWidget*, QtProperty*> m_editorToProperty;
};


#endif //!_QTVARIANTEDITORFACTORYEXT_H
