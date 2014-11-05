#ifndef XML_ENCODER_H
#define XML_ENCODER_H

#include <QString>

class QIODevice;
class QObject;
class QXmlStreamWriter;

class XMLEncoder {
public:
	XMLEncoder(QIODevice*);
	XMLEncoder(QIODevice* device, int indentationLevel);
	~XMLEncoder();

	bool encode(QObject*);

private:
	void encode(QObject* obj, const QString& tagName);

	QIODevice* _out;
	QXmlStreamWriter* _stream;
	QString _currentNamespace;
};

#endif // XML_ENCODER_H
