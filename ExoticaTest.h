#ifndef EXOTICA_TEST_H
#define EXOTICA_TEST_H

#include <QtTest/QtTest>

class ExoticaTest : public QObject {
	Q_OBJECT
private slots:
	void marshal();
};

#endif // EXOTICA_TEST_H
