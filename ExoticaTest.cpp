#include "ExoticaTest.h"
#include "exotica.h"

ObjectA1* objectA1 = new ObjectA1 {17, 17.17, "Claro enigma"};
ObjectA2* objectA2 = new ObjectA2 {objectA1, {1, 2, 3}};
ObjectA3* objectA3 = new ObjectA3 {{1.2, 1.3}, objectA2};
QString objectA3XML =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><ObjectA3><a31>1.2</a31><a31>1.3</a31><a32><a21><a11>17</a11><a12>17.17</a12><a13>Claro enigma</a13></a21><a22>1</a22><a22>2</a22><a22>3</a22></a32></ObjectA3>)" "\n";

ObjectA4* objectA4 = new ObjectA4 {{1.2, 1.3}, objectA2};
QString objectA4XML =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><object><a31>1.2</a31><a31>1.3</a31><a32><a21><a11>17</a11><a12>17.17</a12><a13>Claro enigma</a13></a21><a22>1</a22><a22>2</a22><a22>3</a22></a32></object>)" "\n";

void ExoticaTest::marshal_data() {
    QTest::addColumn<QObject*>("object");
    QTest::addColumn<QString>("xml");
    QTest::newRow("Simple structure of objects") << (QObject*) objectA3 << objectA3XML;
    QTest::newRow("Object with a tag name") << (QObject*) objectA4 << objectA4XML;
}

void ExoticaTest::marshal() {
    QFETCH(QObject*, object);
    QFETCH(QString, xml);
    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);
    exotica::marshal(object, &buffer);
    QString result = buffer.data();
    QCOMPARE(result, xml);
    buffer.close();
}

QTEST_APPLESS_MAIN(ExoticaTest)
