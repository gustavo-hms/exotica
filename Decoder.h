#ifndef XML_DECODER_H
#define XML_DECODER_H

#include <QVariant>
#include "Property.h"

class QIODevice;
class QMetaObject;
class QMetaProperty;
class QObject;
class QString;
class QStringRef;
class QXmlStreamReader;

class Decoder {
public:
	Decoder(QIODevice*);
	~Decoder();

	bool decode(QObject*);

private:
	bool decode(Property&);
	bool decodeIntoList(Property&);
	bool decodeChildrenIntoProperties(QObject*);
	QString classInfo(const QMetaObject* meta, const QString& item) const;
	QMetaProperty propertyWithName(QObject*, const QStringRef&);

	template <class T> void appendToList(Property& property,
	                                     const QVariant& value) {
		auto l = property.value().value<QList<T>>();
		l << value.value<T>();
		QVariant v;
		v.setValue(l);
		property.set(v);
	}

	QIODevice* _in;
	QXmlStreamReader* _stream;
};

#endif // XML_DECODER_H
