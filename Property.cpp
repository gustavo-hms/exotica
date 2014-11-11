#include <QMetaObject>
#include <QMetaClassInfo>
#include "Property.h"

Property::Property(const QMetaObject* metaobject, const QString& name,
                   const QVariant& value) :
	_name(name),
	_value(value),
	_isAttr(false),
	_isCharData(false),
	_isInnerXML(false) {
	int index = metaobject->indexOfClassInfo(("xml " + name).toUtf8().data());

	if (index == -1) {
		return;
	}

	QString info = metaobject->classInfo(index).value();
	auto attributes = info.split(",", QString::SkipEmptyParts);

	for (auto attribute : attributes) {
		auto attr = attribute.trimmed();

		if (attr == "attr") {
			_isAttr = true;

		} else if (attr == "chardata") {
			_isCharData = true;

		} else if (attr == "innerxml") {
			_isInnerXML = true;

		} else if (attr.startsWith("alias:")) {
			_alias = attr.section('\'', 1, 1);

		} else if (attr.startsWith("namespace:")) {
			_namespace = attr.section('\'', 1, 1);
		}
	}
}

const QVariant& Property::value() const {
	return _value;
}

bool Property::isAttr() const {
	return _isAttr;
}

bool Property::isCharData() const {
	return _isCharData;
}

bool Property::isInnerXML() const {
	return _isInnerXML;
}

const QString& Property::alias() const {
	return _alias;
}

const QString& Property::name() const {
	return _name;
}

const QString& Property::namespac() const {
	return _namespace;
}
