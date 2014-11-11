#ifndef XML_ENCODER_H
#define XML_ENCODER_H

#include <QString>

class QIODevice;
class QMetaObject;
class QObject;
class QVariant;
class QXmlStreamWriter;
class Property;

class Encoder {
public:
	Encoder(QIODevice*);
	Encoder(QIODevice* device, int indentationLevel);
	~Encoder();

	bool encode(QObject*);

private:
	void encode(QObject* obj, const QString& tagName);
	bool encode(const Property&);
	void encode(const QVariant& obj, const QString& tagName);
	QString classInfo(const QMetaObject* metaobject, const QString& item) const;

	QIODevice* _out;
	QXmlStreamWriter* _stream;
	QString _currentNamespace;
};

#endif // XML_ENCODER_H
