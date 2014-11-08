#ifndef XML_ENCODER_H
#define XML_ENCODER_H

#include <QString>

class QIODevice;
class QObject;
class QXmlStreamWriter;

class Encoder {
public:
	Encoder(QIODevice*);
	Encoder(QIODevice* device, int indentationLevel);
	~Encoder();

	bool encode(QObject*);

private:
	void encode(QObject* obj, const QString& tagName);

	QIODevice* _out;
	QXmlStreamWriter* _stream;
	QString _currentNamespace;
};

#endif // XML_ENCODER_H
