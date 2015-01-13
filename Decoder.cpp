#include <QMetaProperty>
#include <QXmlStreamReader>
#include "Decoder.h"
#include "Property.h"

Decoder::Decoder(QIODevice* in) : _in(in), _stream(new QXmlStreamReader(in)) {
}

Decoder::~Decoder() {
	delete _stream;
}

bool Decoder::decode(QObject* object) {
	while (not _stream->atEnd()) {
		bool thereIsMore = _stream->readNextStartElement();

		if (not thereIsMore) {
			break;
		}

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
	}

	return not _stream->hasError();
}

bool Decoder::decodeChildrenIntoProperties(QObject* object) {
	auto properties = Property::extractAll(object);
	int lastMatchProperty = -1;

	while (_stream->readNextStartElement()) {
		for (int i = lastMatchProperty + 1; i < properties.size(); i++) {
			auto name =
			    properties[i].alias().isNull() ? properties[i].name() : properties[i].alias();

			if (_stream->name() == name) {
				lastMatchProperty = i;
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

					lastMatchProperty++;
				}

				break;
			}
		}
	}

	return not _stream->hasError();
}

bool Decoder::decode(Property& property) {
	auto value = property.value();

	if (value.canConvert<QObject*>()) {
		return decodeChildrenIntoProperties(value.value<QObject*>());
	}

	if (property.isInnerXML()) {
		property.set(_stream->readElementText(QXmlStreamReader::IncludeChildElements));
		return not _stream->hasError();
	}

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

	auto xmlName = _stream->name();

	do {
		_stream->readNext();
	} while (_stream->name() != xmlName and not _stream->isCharacters());

	if (not _stream->isCharacters()) {
		return not _stream->hasError();
	}

	property.set(_stream->text().toString());
	return not _stream->hasError();
}

QString Decoder::classInfo(const QMetaObject* meta, const QString& item) const {
	return meta->classInfo(meta->indexOfClassInfo(item.toUtf8().data())).value();
}
