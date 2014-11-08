#include <QIODevice>
#include <QMetaClassInfo>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QXmlStreamWriter>
#include "Encoder.h"

Encoder::Encoder(QIODevice* out) :
	_out(out) {
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

	int namespaceIndex = meta->indexOfClassInfo("xmlNamespace");

	if (namespaceIndex != -1) {
		QString xmlNamespace = meta->classInfo(namespaceIndex).value();
		_stream->writeDefaultNamespace(xmlNamespace);
		_currentNamespace = xmlNamespace;
	}

	int versionIndex = meta->indexOfClassInfo("xmlVersion");
	QString version =
		versionIndex != -1 ? meta->classInfo(versionIndex).value() : "1.0";

	int standaloneIndex = meta->indexOfClassInfo("xmlStandalone");
	bool standalone = false;

	if (standaloneIndex != -1
		&& QString(meta->classInfo(standaloneIndex).value()) == "true") {
		standalone = true;
	}

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

	int xmlNameIndex = meta->indexOfClassInfo("xmlName");
	QString xmlName;

	if (xmlNameIndex != -1) {
		xmlName = meta->classInfo(xmlNameIndex).value();

	} else if (tagName.size()) {
		xmlName = tagName;

	} else {
		xmlName = meta->className();
	}

	_stream->writeStartElement(xmlName);

	int namespaceIndex = meta->indexOfClassInfo("xmlNamespace");

	if (namespaceIndex != -1) {
		QString xmlNamespace = meta->classInfo(namespaceIndex).value();
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

void Encoder::encode(const QVariant& obj, const QString& tagName)
{
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
	case QMetaType::QStringList:
	case QMetaType::QTime:
	case QMetaType::UInt:
	case QMetaType::ULongLong:
		QString xmlName;

		if (tagName.size()) {
			xmlName = tagName;

		} else {
			xmlName = obj.typeName();
		}

		_stream->writeStartElement(xmlName);
		_stream->writeCharacters(obj.toString());
		_stream->writeEndElement();
	}
}
