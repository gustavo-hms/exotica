#include <QIODevice>
#include <QMetaClassInfo>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QXmlStreamWriter>
#include "Property.h"
#include "Encoder.h"

Encoder::Encoder(QIODevice* out) : _out(out) {
	_stream = new QXmlStreamWriter(out);
}

Encoder::Encoder(QIODevice* out, int indentationLevel) : _out(out) {
	_stream = new QXmlStreamWriter(out);
	_stream->setAutoFormatting(true);
	_stream->setAutoFormattingIndent(indentationLevel);
}

Encoder::~Encoder() {
	delete _stream;
}

bool Encoder::encode(QObject* object) {
	auto meta = object->metaObject();
	auto xmlNamespace = classInfo(meta, "xmlNamespace");

	if (!xmlNamespace.isNull()) {
		_stream->writeDefaultNamespace(xmlNamespace);
		_currentNamespace = xmlNamespace;
	}

	auto xmlVersion = classInfo(meta, "xmlVersion");
	QString version = xmlVersion.isNull() ? "1.0" : xmlVersion;

	auto xmlStandalone = classInfo(meta, "xmlStandalone");
	bool standalone = xmlStandalone == "true" ? true : false;

	_stream->writeStartDocument(version, standalone);
	encode(object, "");
	_stream->writeEndDocument();
	return _stream->hasError();
}

void Encoder::encode(QObject* object, const QString& tagName) {
	auto meta = object->metaObject();

	int marshalMethod =
	    meta->indexOfMethod(QMetaObject::normalizedSignature("marshalXML(QIODevice*)"));

	if (marshalMethod != -1) {
		bool returnValue;
		auto method = meta->method(marshalMethod);
		method.invoke(object, Q_RETURN_ARG(bool, returnValue), Q_ARG(QIODevice*, _out));
		return;
	}

	auto xmlName = classInfo(meta, "xmlName");

	if (xmlName.isNull()) {
		if (!tagName.isEmpty()) {
			xmlName = tagName;

		} else {
			xmlName = meta->className();
		}
	}

	_stream->writeStartElement(xmlName);

	auto xmlNamespace = classInfo(meta, "xmlNamespace");

	if (!xmlNamespace.isNull()) {
		auto namespaceAndPrefix = xmlNamespace.split(" ", QString::SkipEmptyParts);

		if (namespaceAndPrefix.size() > 1
		    && namespaceAndPrefix[0] != _currentNamespace) {
			_stream->writeNamespace(namespaceAndPrefix[0], namespaceAndPrefix[1]);
			_currentNamespace = namespaceAndPrefix[0];
		}
	}

	for (int i = meta->propertyOffset(); i < meta->propertyCount(); i++) {
		auto property = meta->property(i);
		auto content = property.read(object);
		encode(content, QString(property.name()));
	}

	_stream->writeEndElement();
}

bool Encoder::encode(const Property& property) {
	if (property.isAttr()) {
		_stream->writeAttribute(property.name(), property.value().toString());
		return _stream->hasError();
	}

	if (property.isCharData()) {
		_stream->writeCharacters(property.value().toString());
		return _stream->hasError();
	}

	if (property.isInnerXML()) {
		return _out->write(property.value().toString().toUtf8()) != -1;
	}

	if (property.alias().isNull()) {
		encode(property.value(), property.name());

	} else {
		encode(property.value(), property.alias());
	}

	return _stream->hasError();
}

void Encoder::encode(const QVariant& obj, const QString& tagName) {
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
		_stream->writeTextElement(tagName, obj.toString());
		return;
	}

	if (obj.canConvert<QObject*>()) {
		encode(obj.value<QObject*>(), tagName);
		return;
	}

	if (obj.canConvert<QVariantList>()) {
		for (auto element : obj.value<QSequentialIterable>()) {
			encode(element, tagName);
		}

		return;
	}
}

QString Encoder::classInfo(const QMetaObject* meta, const QString& item) const {
	return meta->classInfo(meta->indexOfClassInfo(item.toUtf8().data())).value();
}
