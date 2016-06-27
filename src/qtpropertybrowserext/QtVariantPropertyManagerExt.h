#ifndef QTVARIANTPROPERTYMANAGEREXT_H
#define QTVARIANTPROPERTYMANAGEREXT_H


#include <QtGui/QVector3D>

#include "qtvariantproperty.h"

#include "QtPropertyDefines.h"


// custom property manager

class QtVariantPropertyManagerExt: public QtVariantPropertyManager
{
	Q_OBJECT

public:
    QtVariantPropertyManagerExt(QObject *parent = 0);
	
	virtual QtVariantProperty *addProperty(int propertyType, const QString &name = QString());
	
	virtual QVariant value(const QtProperty *property) const;
	virtual int valueType(int propertyType) const;
	virtual bool isPropertyTypeSupported(int propertyType) const;

	virtual QStringList attributes(int propertyType) const;
	virtual int attributeType(int propertyType, const QString &attribute) const;
	virtual QVariant attributeValue(const QtProperty *property, const QString &attribute) const;

public Q_SLOTS:
	virtual void setValue(QtProperty *property, const QVariant &val);
	virtual void setAttribute(QtProperty *property,	const QString &attribute, const QVariant &value);
	
private Q_SLOTS:
	virtual void OnPropertyChanged(QtProperty*, const QVariant&);
	
protected:
	virtual QString valueText(const QtProperty *property) const;
	virtual void initializeProperty(QtProperty *property);
	virtual void uninitializeProperty(QtProperty *property);

protected:
	bool m_duringChanges;
	QMap<QtProperty*, QtProperty*> m_propertySubMap;

	// file path property
	struct FilePathData{
		QString value;
		QString filter;
		QString defaultPath;
		FilePathType type;
	};	
	
	QMap<const QtProperty*, FilePathData> m_filePathValues;
	
	// QVariant3d property	
	struct Vector3dData{
		QVector3D value;
		QVector3D vmin, vmax;
	};
	
	QMap<const QtProperty*, Vector3dData> m_vector3dValues;
};


#endif //!QTVARIANTPROPERTYMANAGEREXT_H
