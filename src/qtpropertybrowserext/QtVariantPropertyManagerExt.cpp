#include "QtVariantPropertyManagerExt.h"
#include "QtPropertyDefines.h"

#include <cfloat>
#include <limits>


// QtVariantPropertyManagerExt

QtVariantPropertyManagerExt::QtVariantPropertyManagerExt(QObject *parent) : QtVariantPropertyManager(parent)
{
	m_duringChanges = false;
	
	QObject::connect(this, 
		SIGNAL(valueChanged(QtProperty*, const QVariant&)),
		this, 
		SLOT(OnPropertyChanged(QtProperty*, const QVariant&)));
}


QtVariantProperty* QtVariantPropertyManagerExt::addProperty(int propertyType, const QString &name)
{
	m_duringChanges = true;
	
    QtVariantProperty* vp = QtVariantPropertyManager::addProperty(propertyType, name);
    if (vp)
    {
        if (propertyType == QVariant::Vector3D)
        {
            QtVariantProperty* px = QtVariantPropertyManager::addProperty(QVariant::Double, "X");
			vp->addSubProperty(px);
			m_propertySubMap[px] = vp;
            px->setAttribute("minimum", -FLT_MAX);
            px->setAttribute("maximum", FLT_MAX);
			
            QtVariantProperty* py = QtVariantPropertyManager::addProperty(QVariant::Double, "Y");
			vp->addSubProperty(py);
			m_propertySubMap[py] = vp;
            py->setAttribute("minimum", -FLT_MAX);
            py->setAttribute("maximum", FLT_MAX);

            QtVariantProperty* pz = QtVariantPropertyManager::addProperty(QVariant::Double, "Z");
			vp->addSubProperty(pz);		
			m_propertySubMap[pz] = vp;
            pz->setAttribute("minimum", -FLT_MAX);
            pz->setAttribute("maximum", FLT_MAX);
        }
	}
	
	m_duringChanges = false;
	
	return vp;
}


bool QtVariantPropertyManagerExt::isPropertyTypeSupported(int propertyType) const
{
	if (propertyType == QVariant::Url)
		return true;
		
	if (propertyType == QVariant::Vector3D)
		return true;
		
	return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}


int QtVariantPropertyManagerExt::valueType(int propertyType) const
{
	if (propertyType == QVariant::Url)
		return QVariant::String;

	if (propertyType == QVariant::Vector3D)
		return QVariant::Vector3D;
		
	return QtVariantPropertyManager::valueType(propertyType);
}


QVariant QtVariantPropertyManagerExt::value(const QtProperty *property) const
{
	if (m_filePathValues.contains(property))
		return m_filePathValues[property].value;
		
	if (m_vector3dValues.contains(property))
		return m_vector3dValues[property].value;
		
	return QtVariantPropertyManager::value(property);
}


QStringList QtVariantPropertyManagerExt::attributes(int propertyType) const
{
    if (propertyType == QVariant::Url)
    {
		QStringList attr;
		attr << QLatin1String("filter") << QLatin1String("type") << QLatin1String("defaultPath");
		return attr;
	}
	
    if (propertyType == QVariant::Vector3D)
    {
		return QtVariantPropertyManager::attributes(QVariant::Double);
	}

    QStringList result = QtVariantPropertyManager::attributes(propertyType);

    result << "suffix" << "prefix";

    if (propertyType == QVariant::Int || propertyType == QVariant::Double)
    {
        result << "preset";
    }

    return result;
}


int QtVariantPropertyManagerExt::attributeType(int propertyType, const QString &attribute) const
{
    if (propertyType == QVariant::Url)
    {
		if (attribute == QLatin1String("filter"))
			return QVariant::String;
		if (attribute == QLatin1String("defaultPath"))
			return QVariant::String;
		if (attribute == QLatin1String("type"))
			return QVariant::ByteArray;

		return 0;
	}

    if (propertyType == QVariant::Vector3D)
    {
        if (attribute == QLatin1String("minimum") || attribute == QLatin1String("maximum"))
			return QVariant::Vector3D;

		return QtVariantPropertyManager::attributeType(QVariant::Double, attribute);
	}

    if (attribute == "suffix" || attribute == "prefix")
    {
        return QVariant::String;
    }

    if (attribute == "preset")
    {
//        if (propertyType == QVariant::Int)
//            return TIntStringList;

//        if (propertyType == QVariant::Double)
//            return TDoubleStringList;
    }
	
	return QtVariantPropertyManager::attributeType(propertyType, attribute);
}


QVariant QtVariantPropertyManagerExt::attributeValue(const QtProperty *property, const QString &attribute) const
{
    if (m_filePathValues.contains(property))
    {
		if (attribute == QLatin1String("type"))
			return m_filePathValues[property].type;			
		if (attribute == QLatin1String("filter"))
			return m_filePathValues[property].filter;
		if (attribute == QLatin1String("defaultPath"))
			return m_filePathValues[property].defaultPath;
			
		return QVariant();
	}

    if (m_vector3dValues.contains(property))
    {
        if (attribute == QLatin1String("minimum"))
			return m_vector3dValues[property].vmin;
        if (attribute == QLatin1String("maximum"))
			return m_vector3dValues[property].vmax;
		
		QtVariantPropertyManager::attributeValue(property->subProperties().first(), attribute);
	}

    if (attribute == "suffix")
    {
        if (m_suffixes.contains(property))
            return m_suffixes[property];

        return QVariant();
    }

    if (attribute == "prefix")
    {
        if (m_prefixes.contains(property))
            return m_prefixes[property];

        return QVariant();
    }

    if (attribute == "preset")
    {
        if (m_intPresets.contains(property))
            return QVariant::fromValue(m_intPresets[property]);

        if (m_doublePresets.contains(property))
            return QVariant::fromValue(m_doublePresets[property]);

        return QVariant();
    }

	return QtVariantPropertyManager::attributeValue(property, attribute);
}


QString QtVariantPropertyManagerExt::valueText(const QtProperty *property) const
{
	if (m_filePathValues.contains(property))
		return m_filePathValues[property].value;

    if (m_vector3dValues.contains(property))
    {
		QVector3D v = m_vector3dValues[property].value;

        bool ok = false;
        int decimals = QtVariantPropertyManager::attributeValue(property->subProperties().first(), "decimals").toInt(&ok);
        if (!ok)
            decimals = 2;

        return QString("[%1; %2; %3]").arg(v[0],0,'f',decimals).arg(v[1],0,'f',decimals).arg(v[2],0,'f',decimals);
	}

    QString result = QtVariantPropertyManager::valueText(property);

    if (m_suffixes.contains(property))
    {
        result += " " + m_suffixes[property];
    }

    if (m_prefixes.contains(property))
    {
        result.prepend(m_prefixes[property] + " ");
    }
		
    return result;
}


void QtVariantPropertyManagerExt::setValue(QtProperty *property, const QVariant &val)
{
    if (m_filePathValues.contains(property))
    {
		if (val.type() != QVariant::String && !val.canConvert(QVariant::String))
			return;
			
		QString str = val.value<QString>();
		FilePathData d = m_filePathValues[property];
		if (d.value == str)
			return;
			
		d.value = str;
		m_filePathValues[property] = d;
		emit propertyChanged(property);
		emit valueChanged(property, str);
		return;
	}
	
    if (m_vector3dValues.contains(property))
    {
		if (val.type() != QVariant::Vector3D && !val.canConvert(QVariant::Vector3D))
			return;

		QVector3D v = val.value<QVector3D>();
		Vector3dData d = m_vector3dValues[property];
		if (d.value == v)
			return;
			
		d.value = v;
		m_vector3dValues[property] = d;
		
		QList<QtProperty*> subp = property->subProperties();
		Q_ASSERT(subp.size() == 3);
		
		m_duringChanges = true;
		QtVariantPropertyManager::setValue(subp[0], v[0]);
		QtVariantPropertyManager::setValue(subp[1], v[1]);
		QtVariantPropertyManager::setValue(subp[2], v[2]);
		m_duringChanges = false;
		
		Q_EMIT propertyChanged(property);
		Q_EMIT valueChanged(property, v);
		return;
	}
	
	QtVariantPropertyManager::setValue(property, val);
}


void QtVariantPropertyManagerExt::setAttribute(QtProperty *property, const QString &attribute, const QVariant &val)
{
    if (m_filePathValues.contains(property))
    {
		if (attribute == QLatin1String("filter")) {
			if (val.type() != QVariant::String && !val.canConvert(QVariant::String))
				return;
				
			QString str = val.value<QString>();
			FilePathData d = m_filePathValues[property];
			if (d.filter == str)
				return;
				
			d.filter = str;
			m_filePathValues[property] = d;
			Q_EMIT attributeChanged(property, attribute, str);
			return;
		}

		if (attribute == QLatin1String("defaultPath")) {
			if (val.type() != QVariant::String && !val.canConvert(QVariant::String))
				return;

			QString str = val.value<QString>();
			FilePathData d = m_filePathValues[property];
			if (d.defaultPath == str)
				return;

			d.defaultPath = str;
			m_filePathValues[property] = d;
			Q_EMIT attributeChanged(property, attribute, str);
			return;
		}
		
		if (attribute == QLatin1String("type")) {
			if (val.type() != QVariant::UInt && !val.canConvert(QVariant::UInt))
				return;

			int t = val.value<uint>();
			if (t < FPT_FILE_OPEN || t > FPT_DIRECTORY)
				return;
			
			FilePathType typ = (FilePathType)t;
			FilePathData d = m_filePathValues[property];
			if (d.type == typ)
				return;

			d.type = typ;
			m_filePathValues[property] = d;
			Q_EMIT attributeChanged(property, attribute, t);
			return;
		}		
		
		return;
	}
	
    if (m_vector3dValues.contains(property))
    {
		QList<QtProperty*> subp = property->subProperties();
		Q_ASSERT(subp.size() == 3);

        bool isMin = (attribute == QLatin1String("minimum"));
        bool isMax = (attribute == QLatin1String("maximum"));
		if (!isMin && !isMax){
			QtVariantPropertyManager::setAttribute(subp[0], attribute, val);
			QtVariantPropertyManager::setAttribute(subp[1], attribute, val);
			QtVariantPropertyManager::setAttribute(subp[2], attribute, val);
			return;			
		}
		
		if (val.type() != QVariant::Vector3D && !val.canConvert(QVariant::Vector3D))
			return;

		QVector3D v = val.value<QVector3D>();
		Vector3dData d = m_vector3dValues[property];
		if (isMin && d.vmin == v)
			return;
		if (isMax && d.vmax == v)
			return;

		if (isMin) d.vmin = v;
		if (isMax) d.vmax = v;
		
		QtVariantPropertyManager::setAttribute(subp[0], attribute, v[0]);
		QtVariantPropertyManager::setAttribute(subp[1], attribute, v[1]);
		QtVariantPropertyManager::setAttribute(subp[2], attribute, v[2]);	
		
		m_vector3dValues[property] = d;
		Q_EMIT attributeChanged(property, attribute, v);
		return;
	}
	
    if (attribute == "suffix")
    {
        m_suffixes[property] = val.toString();
        return;
    }

    if (attribute == "prefix")
    {
        m_prefixes[property] = val.toString();
        return;
    }

    if (attribute == "preset")
    {
        int type = propertyType(property);

        if (type == QVariant::Int)
        {
            m_intPresets[property] = val.value<TIntStringList>();
            return;
        }

        if (type == QVariant::Double)
        {
            m_doublePresets[property] = val.value<TDoubleStringList>();
            return;
        }

        return;
    }

	QtVariantPropertyManager::setAttribute(property, attribute, val);
}


void QtVariantPropertyManagerExt::OnPropertyChanged(QtProperty *property, const QVariant &val)
{
	if (m_duringChanges)
		return;
		
	if (m_propertySubMap.contains(property)){
		QtProperty* parent = m_propertySubMap[property];
		Q_ASSERT(parent);
		
		if (m_vector3dValues.contains(parent)){
			QList<QtProperty*> subp = parent->subProperties();
			Q_ASSERT(subp.size() == 3);
			
			Vector3dData& data = m_vector3dValues[parent];
			for (int i = 0; i < 3; ++i){
				if (property == subp[i]){
					data.value[i] = val.toFloat();
					
					Q_EMIT propertyChanged(parent);
					Q_EMIT valueChanged(parent, data.value);
					return;
				}		
			}
		}
	}
}


void QtVariantPropertyManagerExt::initializeProperty(QtProperty *property)
{
	if (propertyType(property) == QVariant::Url)
		m_filePathValues[property] = FilePathData();
		
	if (propertyType(property) == QVariant::Vector3D)
		m_vector3dValues[property] = Vector3dData();	
		
	QtVariantPropertyManager::initializeProperty(property);
}


void QtVariantPropertyManagerExt::uninitializeProperty(QtProperty *property)
{
	m_filePathValues.remove(property);
	m_vector3dValues.remove(property);
	m_propertySubMap.remove(property);
    m_suffixes.remove(property);
    m_prefixes.remove(property);
    m_intPresets.remove(property);
    m_doublePresets.remove(property);

	QtVariantPropertyManager::uninitializeProperty(property);
}
