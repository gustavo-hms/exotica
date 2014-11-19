#ifndef EXOTICA_TEST_H
#define EXOTICA_TEST_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QString>
#include <QtTest/QtTest>

class ExoticaTest : public QObject {
	Q_OBJECT
private slots:
	void marshal();
	void marshal_data();
};

// Simple structure of objects

class ObjectA1 : public QObject {
	Q_OBJECT
	Q_PROPERTY(int a11 MEMBER member1)
	Q_PROPERTY(double a12 MEMBER member2)
	Q_PROPERTY(QString a13 MEMBER member3)
	
public:
	ObjectA1(QObject* parent = nullptr) : QObject(parent) {}
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
	Q_PROPERTY(QVector<int> a22 MEMBER member2)
	
public:
	ObjectA2(QObject* parent = nullptr) : QObject(parent) {}
	ObjectA2(ObjectA1* m1, const QVector<int>& m2) :
	    QObject(nullptr),
	    member1(m1),
	    member2(m2) {}
	
	ObjectA1* member1;
	QVector<int> member2;
};

class ObjectA3 : public QObject {
	Q_OBJECT
	Q_PROPERTY(QList<double> a31 MEMBER member1)
	Q_PROPERTY(ObjectA2* a32 MEMBER member2)
	
public:
	ObjectA3(QObject* parent = nullptr) : QObject(parent) {}
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

class ObjectB1 : public QObject {
	Q_OBJECT
	Q_CLASSINFO("xmlNamespace", "http://aqui.ali acolá")
	Q_PROPERTY(int b11 MEMBER member1)
	Q_PROPERTY(double b12 MEMBER member2)
	Q_PROPERTY(QString b13 MEMBER member3)
	
public:
	ObjectB1(QObject* parent = nullptr) : QObject(parent) {}
	ObjectB1(int m1, double m2, const QString& m3) :
	    QObject(nullptr),
	    member1(m1),
	    member2(m2),
	    member3(m3) {}
	
	int member1;
	double member2;
	QString member3;
};

class ObjectB2 : public QObject {
	Q_OBJECT
	Q_CLASSINFO("xmlName", "object")
	Q_PROPERTY(int b21 MEMBER member1)
	Q_PROPERTY(ObjectB1* b22 MEMBER member2)
	Q_PROPERTY(int b23 MEMBER member3)
	
public:
	ObjectB2(QObject* parent = nullptr) : QObject(parent) {}
	ObjectB2(int m1, ObjectB1* m2, int m3) :
	    QObject(nullptr),
	    member1(m1),
	    member2(m2),
	    member3(m3) {}
	
	int member1;
	ObjectB1* member2;
	int member3;
};

#endif // EXOTICA_TEST_H
