#include <QIODevice>
#include <QMetaClassInfo>
#include <QObject>
#include <QString>
#include <QXmlStreamWriter>
#include "XMLEncoder.h"

XMLEncoder::XMLEncoder(QIODevice* out) : _out(out) {
	_stream = new QXmlStreamWriter(out);
}

XMLEncoder::XMLEncoder(QIODevice* out, int indentationLevel) : _out(out) {
	_stream = new QXmlStreamWriter(out);
	_stream->setAutoFormatting(true);
	_stream->setAutoFormattingIndent(indentationLevel);
}

XMLEncoder::~XMLEncoder() {
	delete _stream;
}

bool XMLEncoder::encode(QObject* object) {
	auto meta = object->metaObject();

	int namespaceIndex = meta->indexOfClassInfo("xmlNamespace");

	if (namespaceIndex != -1) {
		_stream->writeDefaultNamespace(meta->classInfo(namespaceIndex).value());
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

void XMLEncoder::encode(QObject* object, const QString& tagName) {
	QString tag;
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
	_stream->writeEndElement();
}
