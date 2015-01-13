#include <QString>

class QObject;

class Property {
public:
	Property();
	Property(QObject* object, const QString& name);

	static QVector<Property> extractAll(QObject* object);

	void set(const QVariant&);
	QVariant value() const;
	bool omitempty() const;
	bool isAttr() const;
	bool isCharData() const;
	bool isInnerXML() const;
	const QString& alias() const;
	const QString& name() const;
	const QString& namespac() const;

private:
	QObject* _object;
	QString _name;
	bool _omitempty;
	bool _isAttr;
	bool _isCharData;
	bool _isInnerXML;
	QString _alias;
	QString _namespace;
};
