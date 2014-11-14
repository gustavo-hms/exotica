#ifndef CORPUS_H
#define CORPUS_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QString>

// Simple structure of objects

class ObjectA1 : public QObject {
	Q_OBJECT
	Q_PROPERTY(int prop1 MEMBER prop1)
	Q_PROPERTY(double prop2 MEMBER prop2)
	Q_PROPERTY(QString prop3 MEMBER prop3)
	Q_PROPERTY(bool prop4 MEMBER prop4)

public:
	ObjectA1(QObject* parent = nullptr) :
		QObject(parent),
		prop1(17),
		prop2(17.17),
		prop3("Claro enigma"),
		prop4(true) {}

	int prop1;
	double prop2;
	QString prop3;
	bool prop4;
};

class ObjectA2 : public QObject {
	Q_OBJECT
	Q_PROPERTY(ObjectA1* prop1 MEMBER prop1)
	Q_PROPERTY(QVector<int> prop2 MEMBER prop2)

public:
	ObjectA2(QObject* parent = nullptr) :
		QObject(parent),
		prop1(new ObjectA1(this)),
		prop2({1, 2}) {}

	ObjectA1* prop1;
	QVector<int> prop2;
};

class ObjectA3 : public QObject {
	Q_OBJECT
	Q_PROPERTY(QList<float> prop1 MEMBER prop1)
	Q_PROPERTY(ObjectA2* prop2 MEMBER prop2)

public:
	ObjectA3(QObject* parent = nullptr) :
		QObject(parent),
		prop1({1.2, 3.4}),
	prop2(new ObjectA2(this)) {}

	QList<float> prop1;
	ObjectA2* prop2;
};

#endif // CORPUS_H
