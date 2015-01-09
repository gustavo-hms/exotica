#include <QIODevice>
#include <QMetaClassInfo>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QXmlStreamWriter>
#include "Property.h"
#include "Encoder.h"

Encoder::Encoder(QIODevice* out) : _out(out), _stream(new QXmlStreamWriter(out)) {
}

Encoder::Encoder(QIODevice* out, int indentationLevel) :
	_out(out), _stream(new QXmlStreamWriter(out))  {
	_stream->setAutoFormatting(true);
	_stream->setAutoFormattingIndent(indentationLevel);
}

Encoder::~Encoder() {
	delete _stream;
}

bool Encoder::encode(QObject* object) {
	auto meta = object->metaObject();
	auto xmlNamespace = classInfo(meta, "xmlNamespace");

	if (not xmlNamespace.isNull()) {
		_stream->writeDefaultNamespace(xmlNamespace);

		if (_stream->hasError()) {
			return false;
		}

		_currentNamespace = xmlNamespace;
	}

	auto xmlVersion = classInfo(meta, "xmlVersion");
	QString version = xmlVersion.isNull() ? "1.0" : xmlVersion;

	auto xmlStandalone = classInfo(meta, "xmlStandalone");
	bool standalone = xmlStandalone == "true";

	_stream->writeStartDocument(version, standalone);
	bool ok = encode(object, "");

	if (not ok) {
		return false;
	}

	_stream->writeEndDocument();
	return not _stream->hasError();
}

bool Encoder::encode(QObject* object, const QString& tagName) {
	auto meta = object->metaObject();

	int marshalMethod =
	    meta->indexOfMethod(QMetaObject::normalizedSignature("marshalXML(QIODevice*)"));

	if (marshalMethod != -1) {
		bool returnValue;
		auto method = meta->method(marshalMethod);
		method.invoke(object, Q_RETURN_ARG(bool, returnValue), Q_ARG(QIODevice*, _out));
		return returnValue;
	}

	auto xmlName = classInfo(meta, "xmlName");

	if (xmlName.isNull()) {
		if (not tagName.isEmpty()) {
			xmlName = tagName;

		} else {
			xmlName = meta->className();
		}
	}

	_stream->writeStartElement(_currentNamespace, xmlName);
	
	if (_stream->hasError()) {
		return false;
	}

	QString currentNamespace = _currentNamespace;
	auto xmlNamespace = classInfo(meta, "xmlNamespace");

	if (not xmlNamespace.isNull()) {
		auto namespaceAndPrefix = xmlNamespace.split(" ", QString::SkipEmptyParts);

		if (namespaceAndPrefix.size() > 0
		    && namespaceAndPrefix[0] != _currentNamespace) {
			QString prefix = namespaceAndPrefix.size() > 1 ? namespaceAndPrefix[1] : "";
			_stream->writeNamespace(namespaceAndPrefix[0], prefix);

			if (_stream->hasError()) {
				return false;
			}

			_currentNamespace = namespaceAndPrefix[0];
		}
	}

	for (int i = meta->propertyOffset(); i < meta->propertyCount(); i++) {
		auto prop = meta->property(i);
		Property property(meta, prop.name(), prop.read(object));
		encode(property);
	}

	_stream->writeEndElement();
	_currentNamespace = currentNamespace;
	return not _stream->hasError();
}

bool Encoder::encode(const Property& property) {
	if (property.omitempty()
	    and (property.value().isNull() or not property.value().toBool())) {
		return true;
	}

	if (property.isCharData()) {
		_stream->writeCharacters(property.value().toString());
		return not _stream->hasError();
	}

	if (property.isInnerXML()) {
		_stream->writeCharacters("");
		return _out->write(property.value().toString().toUtf8()) != -1;
	}

	QString name = property.alias().isNull() ? property.name() : property.alias();

	if (property.isAttr()) {
		_stream->writeAttribute(name, property.value().toString());
		return not _stream->hasError();
	}

	QString currentNamespace = _currentNamespace;

	if (not property.namespac().isNull()) {
		_currentNamespace = property.namespac();
	}

	encode(property.value(), name);
	_currentNamespace = currentNamespace;
	return not _stream->hasError();
}

bool Encoder::encode(const QVariant& obj, const QString& tagName) {
	switch (obj.userType()) {
	case QMetaType::QString:
	case QMetaType::Bool:
	case QMetaType::QByteArray:
	case QMetaType::QChar:
	case QMetaType::QDate:
	case QMetaType::QDateTime:
	case QMetaType::Double:
	case QMetaType::Int:
	case QMetaType::LongLong:
	case QMetaType::QTime:
	case QMetaType::UInt:
	case QMetaType::ULongLong:
		_stream->writeTextElement(_currentNamespace, tagName, obj.toString());
		return not _stream->hasError();
	}

	if (obj.canConvert<QObject*>()) {
		return encode(obj.value<QObject*>(), tagName);
	}

	if (obj.canConvert<QVariantList>()) {
		for (auto element : obj.value<QSequentialIterable>()) {
			bool ok = encode(element, tagName);

			if (not ok) {
				return false;
			}
		}
	}

	return true;
}

QString Encoder::classInfo(const QMetaObject* meta, const QString& item) const {
	return meta->classInfo(meta->indexOfClassInfo(item.toUtf8().data())).value();
}
