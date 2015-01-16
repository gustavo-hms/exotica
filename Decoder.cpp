#include <QMetaProperty>
#include <QXmlStreamReader>
#include "Decoder.h"

Decoder::Decoder(QIODevice* in) : _in(in), _stream(new QXmlStreamReader(in)) {
}

Decoder::~Decoder() {
	delete _stream;
}

bool Decoder::decode(QObject* object) {
	while (_stream->readNextStartElement()) {
		auto meta = object->metaObject();
		auto xmlName = classInfo(meta, "xmlName");

		if (xmlName.isNull()) {
			xmlName = meta->className();
		}

		if (xmlName != _stream->name()) {
			continue;
		}

		auto xmlNamespace = classInfo(meta, "xmlNamespace");

		if (xmlNamespace != _stream->namespaceUri()) {
			continue;
		}

		bool ok = decodeChildrenIntoProperties(object);

		if (not ok) {
			return false;
		}

		break;
	}

	return not _stream->hasError();
}

bool Decoder::decodeChildrenIntoProperties(QObject* object) {
	auto properties = Property::extractAll(object);
	int startFromProperty = 0;
	QStringRef lastXMLElement;

	while (_stream->readNextStartElement()) {
		for (int i = startFromProperty; i < properties.size(); i++) {
			auto name =
			    properties[i].alias().isNull() ? properties[i].name() : properties[i].alias();
			auto xmlName = _stream->name();

			if (xmlName == name) {
				if (xmlName != lastXMLElement) {
					startFromProperty = i;
					lastXMLElement = xmlName;
				}

				bool ok = decode(properties[i]);

				if (not ok) {
					return false;
				}

				auto attributes = _stream->attributes();

				for (int k = i + 1; k < properties.size() && properties[k].isAttr(); k++) {
					auto name =
					    properties[k].alias().isNull() ? properties[k].name() : properties[k].alias();

					if (attributes.hasAttribute(name)) {
						properties[k].set(attributes.value(name).string());
					}

					startFromProperty++;
				}

				break;
			}
		}
	}

	return not _stream->hasError();
}

bool Decoder::decode(Property& property) {
	auto namespac = property.namespac();

	if (not namespac.isNull()) {
		auto declarations = _stream->namespaceDeclarations();
		bool found = false;

		for (auto declaration : declarations) {
			if (declaration.namespaceUri() == namespac) {
				found = true;
				break;
			}
		}

		if (not found) {
			return not _stream->hasError();
		}

	} else if (not _stream->namespaceDeclarations().empty()) {
		return not _stream->hasError();
	}

	auto value = property.value();

	if (value.canConvert<QObject*>()) {
		return decodeChildrenIntoProperties(value.value<QObject*>());
	}

	if (value.canConvert<QVariantList>()) {
		return decodeIntoList(property);
	}

	if (property.isInnerXML()) {
		property.set(_stream->readElementText(QXmlStreamReader::IncludeChildElements));
		return not _stream->hasError();
	}

	auto xmlName = _stream->name();
	property.set(_stream->readElementText());
	return not _stream->hasError();
}

bool Decoder::decodeIntoList(Property& property) {
	QVariant xmlValue = _stream->readElementText();
	auto prop = property.value();

	if (prop.canConvert<QList<int>>()) {
		appendToList<int>(property, xmlValue);
		return not _stream->hasError();
	}

	if (prop.canConvert<QList<double>>()) {
		appendToList<double>(property, xmlValue);
		return not _stream->hasError();
	}

	return true;
}

QString Decoder::classInfo(const QMetaObject* meta, const QString& item) const {
	return meta->classInfo(meta->indexOfClassInfo(item.toUtf8().data())).value();
}
