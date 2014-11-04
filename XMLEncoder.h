#ifndef XML_ENCODER_H
#define XML_ENCODER_H

class QIODevice;
class QObject;
class QString;
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
};

#endif // XML_ENCODER_H
