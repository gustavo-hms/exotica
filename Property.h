#include <QString>

class QMetaObject;

class Property {
public:
	Property(const QMetaObject* metaobject, const QString& name,
	         const QVariant& value);

	const QVariant& value() const;
	bool isAttr() const;
	bool isCharData() const;
	bool isInnerXML() const;
	const QString& alias() const;
	const QString& name() const;
	const QString& namespac() const;

private:
	QString _name;
	QMetaObject* _metaobject;
	QVariant _value;
	bool _isAttr;
	bool _isCharData;
	bool _isInnerXML;
	QString _alias;
	QString _namespace;
};
