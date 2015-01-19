#ifndef EXOTICA_TEST_H
#define EXOTICA_TEST_H

#include <QObject>
#include <QList>
#include <QString>
#include <QtTest/QtTest>

class ExoticaTest : public QObject {
	Q_OBJECT
private slots:
	void marshal();
	void marshal_data();
	void marshalIndent();
	void marshalIndent_data();
	void marshalWithBrokenIODevice();
	void marshalWithBrokenIODevice_data();
	void unmarshal();
	void unmarshal_data();
};

bool equal(const QObject*, const QObject*);

// Simple structure of objects

class ObjectA1 : public QObject {
	Q_OBJECT
	Q_PROPERTY(int a11 MEMBER member1)
	Q_PROPERTY(double a12 MEMBER member2)
	Q_PROPERTY(QString a13 MEMBER member3)

public:
	ObjectA1(QObject* parent = nullptr) :
	   	QObject(parent),
		member1(0),
		member2(0) {}
	ObjectA1(int m1, double m2, const QString& m3) :
		QObject(nullptr),
		member1(m1),
		member2(m2),
		member3(m3) {}

	int member1;
	double member2;
	QString member3;
};

class ObjectA2 : public QObject {
	Q_OBJECT
	Q_PROPERTY(ObjectA1* a21 MEMBER member1)
	Q_PROPERTY(QList<int> a22 MEMBER member2)

public:
	ObjectA2(QObject* parent = nullptr) :
	   	QObject(parent),
		member1(new ObjectA1(this)) {}
	ObjectA2(ObjectA1* m1, const QList<int>& m2) :
		QObject(nullptr),
		member1(m1),
		member2(m2) {}

	ObjectA1* member1;
	QList<int> member2;
};

class ObjectA3 : public QObject {
	Q_OBJECT
	Q_PROPERTY(QList<double> a31 MEMBER member1)
	Q_PROPERTY(ObjectA2* a32 MEMBER member2)

public:
	ObjectA3(QObject* parent = nullptr) :
	   	QObject(parent),
		member2(new ObjectA2(this)) {}
	ObjectA3(const QList<double>& m1, ObjectA2* m2) :
		QObject(nullptr),
		member1(m1),
		member2(m2) {}

	QList<double> member1;
	ObjectA2* member2;
};

// Object with a tag name

class ObjectA4 : public ObjectA3 {
	Q_OBJECT
	Q_CLASSINFO("xmlName", "object")
	Q_PROPERTY(QList<double> a31 MEMBER member1)
	Q_PROPERTY(ObjectA2* a32 MEMBER member2)

public:
	using ObjectA3::ObjectA3;
};

// Object with a global namespace

class ObjectA5 : public ObjectA3 {
	Q_OBJECT
	Q_CLASSINFO("xmlNamespace", "http://aqui.ali")
	Q_PROPERTY(QList<double> a31 MEMBER member1)
	Q_PROPERTY(ObjectA2* a32 MEMBER member2)

public:
	using ObjectA3::ObjectA3;
};

// Inner object with a namespace

class ObjectB1 : public ObjectA1 {
	Q_OBJECT
	Q_CLASSINFO("xmlNamespace", "http://aqui.ali acolá")
	Q_PROPERTY(int b11 MEMBER member1)
	Q_PROPERTY(double b12 MEMBER member2)
	Q_PROPERTY(QString b13 MEMBER member3)

public:
	using ObjectA1::ObjectA1;
};

class ObjectB2 : public QObject {
	Q_OBJECT
	Q_CLASSINFO("xmlName", "object")
	Q_CLASSINFO("xmlNamespace", "http://um.nome")
	Q_PROPERTY(int b21 MEMBER member1)
	Q_PROPERTY(QObject* b22 MEMBER member2)
	Q_PROPERTY(int b23 MEMBER member3)

public:
	ObjectB2(QObject* parent = nullptr) :
	   	QObject(parent),
		member1(0),
		member2(nullptr),
		member3(0) {}
	ObjectB2(int m1, QObject* m2, int m3) :
		QObject(nullptr),
		member1(m1),
		member2(m2),
		member3(m3) {}

	int member1;
	QObject* member2;
	int member3;
};

// Member with the namespace of a parent

class ObjectB3 : public ObjectB1 {
	Q_OBJECT
	Q_CLASSINFO("xmlNamespace", "http://aqui.ali acolá")
	Q_CLASSINFO("xml b12", "namespace:'http://um.nome'")
	Q_PROPERTY(int b11 MEMBER member1)
	Q_PROPERTY(double b12 MEMBER member2)
	Q_PROPERTY(QString b13 MEMBER member3)

public:
	using ObjectB1::ObjectB1;
};

// Member with an alias

class ObjectB4 : public ObjectA1 {
	Q_OBJECT
	Q_CLASSINFO("xml b11", "alias:'outro nome'")
	Q_PROPERTY(int b11 MEMBER member1)
	Q_PROPERTY(double b12 MEMBER member2)
	Q_PROPERTY(QString b13 MEMBER member3)

public:
	using ObjectA1::ObjectA1;
};

// An attribute

class ObjectB5 : public ObjectA1 {
	Q_OBJECT
	Q_CLASSINFO("xml b13", "attr")
	Q_PROPERTY(QString b13 MEMBER member3)
	Q_PROPERTY(int b11 MEMBER member1)
	Q_PROPERTY(double b12 MEMBER member2)

public:
	using ObjectA1::ObjectA1;
};

// Chardata

class ObjectB6 : public ObjectA1 {
	Q_OBJECT
	Q_CLASSINFO("xml b13", "chardata")
	Q_PROPERTY(QString b13 MEMBER member3)

public:
	ObjectB6(QObject* parent = nullptr) : ObjectA1(parent) {}
	ObjectB6(const QString& m3) :
		ObjectA1(0, 0, m3) {}
};

// Inner XML

class ObjectB7 : public ObjectA1 {
	Q_OBJECT
	Q_CLASSINFO("xml b13", "innerxml")
	Q_PROPERTY(QString b13 MEMBER member3)

public:
	ObjectB7(QObject* parent = nullptr) : ObjectA1(parent) {}
	ObjectB7(const QString& m3) :
		ObjectA1(0, 0, m3) {}
};

// Omitempty

class ObjectB8 : public ObjectA1 {
	Q_OBJECT
	Q_CLASSINFO("xml b11", "omitempty")
	Q_CLASSINFO("xml b13", "omitempty")
	Q_PROPERTY(int b11 MEMBER member1)
	Q_PROPERTY(double b12 MEMBER member2)
	Q_PROPERTY(QString b13 MEMBER member3)

public:
	using ObjectA1::ObjectA1;
};

// Mixed member attributes

class ObjectB9 : public ObjectA1 {
	Q_OBJECT
	Q_CLASSINFO("xml b11", "alias:'prop1',attr")
	Q_CLASSINFO("xml b12", "alias:'prop2',namespace:'http://um.dois'")
	Q_PROPERTY(int b11 MEMBER member1)
	Q_PROPERTY(double b12 MEMBER member2)
	Q_PROPERTY(QString b13 MEMBER member3)

public:
	using ObjectA1::ObjectA1;
};

class MockIODevice : public QIODevice {
	Q_OBJECT

public:
	MockIODevice(unsigned writesBeforeFail);

	qint64 readData(char*, qint64);
	qint64 writeData(const char*, qint64);
	unsigned counter;

private:
	unsigned _writesBeforeFail;
};

#endif // EXOTICA_TEST_H
