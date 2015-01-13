#include <QMetaObject>
#include <QMetaClassInfo>
#include <QVector>
#include "Property.h"

Property::Property() :
	_object(nullptr),
	_name(""),
	_omitempty(false),
	_isAttr(false),
	_isCharData(false),
	_isInnerXML(false),
	_alias(""),
	_namespace("") {
}

Property::Property(QObject* object, const QString& name) :
	_object(object),
	_name(name),
	_omitempty(false),
	_isAttr(false),
	_isCharData(false),
	_isInnerXML(false) {
	auto metaobject = object->metaObject();
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

		} else if (attr == "omitempty") {
			_omitempty = true;

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

QVector<Property> Property::extractAll(QObject* object) {
	auto meta = object->metaObject();
	int offset = meta->propertyOffset();
	int numberOfProperties = meta->propertyCount() - offset;
	QVector<Property> properties(numberOfProperties);

	for (int i = 0; i < numberOfProperties; i++) {
		auto prop = meta->property(i + offset);
		Property property(object, prop.name());
		properties[i] = property;
	}

	return properties;
}

void Property::set(const QVariant& value) {
	_object->setProperty(_name.toUtf8(), value);
}

QVariant Property::value() const {
	return _object->property(_name.toUtf8());
}

bool Property::omitempty() const {
	return _omitempty;
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
