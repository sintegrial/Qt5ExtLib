#include "QtVariantEditorFactoryExt.h"
#include "QtVariantPropertyManagerExt.h"
#include "QtFilePathEditor.h"


QtVariantEditorFactoryExt::~QtVariantEditorFactoryExt()
{
    qDeleteAll(m_editorToProperty);
}


void QtVariantEditorFactoryExt::connectPropertyManager(QtVariantPropertyManager *manager)
{
	connect(manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
		this, SLOT(slotPropertyChanged(QtProperty *, const QVariant &)));
		
	connect(manager, SIGNAL(attributeChanged(QtProperty *, const QString &, const QVariant &)),
		this, SLOT(slotPropertyAttributeChanged(QtProperty *, const QString &, const QVariant &)));
		
	QtVariantEditorFactory::connectPropertyManager(manager);
}


void QtVariantEditorFactoryExt::disconnectPropertyManager(QtVariantPropertyManager *manager)
{
	disconnect(manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
		this, SLOT(slotPropertyChanged(QtProperty *, const QVariant &)));
		
	disconnect(manager, SIGNAL(attributeChanged(QtProperty *, const QString &, const QVariant &)),
		this, SLOT(slotPropertyAttributeChanged(QtProperty *, const QString &, const QVariant &)));
		
	QtVariantEditorFactory::disconnectPropertyManager(manager);
}


QWidget *QtVariantEditorFactoryExt::createEditor(QtVariantPropertyManager *manager, QtProperty *property, QWidget *parent)
{
	if (manager->propertyType(property) == QVariant::Url) {
		QtFilePathEditor *editor = new QtFilePathEditor(parent);
		editor->setFilePath(manager->value(property).toString());
		editor->setFilter(manager->attributeValue(property, QLatin1String("filter")).toString());
		editor->setDefaultPath(manager->attributeValue(property, QLatin1String("defaultPath")).toString());
		editor->setOpenType((FilePathType)manager->attributeValue(property, QLatin1String("type")).toInt());
        m_createdEditors[property].append(editor);
        m_editorToProperty[editor] = property;

		connect(editor, SIGNAL(filePathChanged(const QString &)), this, SLOT(slotSetValue(const QString &)));
		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
			
		return editor;
	}
	
	return QtVariantEditorFactory::createEditor(manager, property, parent);
}


// slots

void QtVariantEditorFactoryExt::slotPropertyChanged(QtProperty *property, const QVariant &value)
{
    if (!m_createdEditors.contains(property))
		return;
		
    QtVariantPropertyManagerExt* manager = dynamic_cast<QtVariantPropertyManagerExt*>(property->propertyManager());
	if (!manager)
		return;
		
	int propertyType = manager->propertyType(property);

    QList<QWidget*> editors = m_createdEditors[property];
	QListIterator<QWidget*> itEditor(editors);
	
    if (propertyType == QVariant::Url)
    {
        while (itEditor.hasNext())
			(dynamic_cast<QtFilePathEditor*>(itEditor.next()))->setFilePath(value.toString());
	}	
}


void QtVariantEditorFactoryExt::slotPropertyAttributeChanged(QtProperty *property, const QString &attribute, const QVariant &value)
{
    if (!m_createdEditors.contains(property))
		return;

    QtVariantPropertyManagerExt* manager = dynamic_cast<QtVariantPropertyManagerExt*>(property->propertyManager());
	if (!manager)
		return;

	int propertyType = manager->propertyType(property);
	
    QList<QWidget*> editors = m_createdEditors[property];
	QListIterator<QWidget*> itEditor(editors);	
	
    if (propertyType == QVariant::Url)
    {
		if (attribute != QLatin1String("filter"))
			return;

        while (itEditor.hasNext())
			(dynamic_cast<QtFilePathEditor*>(itEditor.next()))->setFilter(value.toString());
	}	
}


void QtVariantEditorFactoryExt::slotSetValue(const QString &value)
{
	QObject *object = sender();
    QMap<QWidget*, QtProperty*>::ConstIterator itEditor = m_editorToProperty.constBegin();
    while (itEditor != m_editorToProperty.constEnd())
    {
        if (itEditor.key() == object)
        {
			QtProperty *property = itEditor.value();
			QtVariantPropertyManager *manager = propertyManager(property);
			if (!manager)
				return;
				
			manager->setValue(property, value);
			return;
		}
		
		itEditor++;
	}
}


void QtVariantEditorFactoryExt::slotEditorDestroyed(QObject *object)
{
    QMap<QWidget*, QtProperty*>::ConstIterator itEditor = m_editorToProperty.constBegin();
    while (itEditor != m_editorToProperty.constEnd())
    {
        if (itEditor.key() == object)
        {
			QWidget *editor = itEditor.key();
			QtProperty *property = itEditor.value();
            m_editorToProperty.remove(editor);
            m_createdEditors[property].removeAll(editor);
            if (m_createdEditors[property].isEmpty())
                m_createdEditors.remove(property);
			return;
		}
		
		itEditor++;
	}
}


