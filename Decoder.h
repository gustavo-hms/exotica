#ifndef XML_DECODER_H
#define XML_DECODER_H

class QIODevice;
class QMetaObject;
class QMetaProperty;
class QObject;
class QString;
class QStringRef;
class QXmlStreamReader;
class Property;

class Decoder {
public:
	Decoder(QIODevice*);
	~Decoder();

	bool decode(QObject*);

private:
	bool decode(Property&);
	bool decodeChildrenIntoProperties(QObject*);
	QString classInfo(const QMetaObject* meta, const QString& item) const;
	QMetaProperty propertyWithName(QObject*, const QStringRef&);

	QIODevice* _in;
	QXmlStreamReader* _stream;
};

#endif // XML_DECODER_H
