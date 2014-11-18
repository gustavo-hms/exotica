#include "ExoticaTest.h"
#include "exotica.h"

ObjectA1* objectA1 = new ObjectA1 {17, 17.17, "Claro enigma"};
ObjectA2* objectA2 = new ObjectA2 {objectA1, {1, 2, 3}};
ObjectA3* objectA3 = new ObjectA3 {{1.2, 1.3}, objectA2};
QString objectA3XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><ObjectA3><a31>1.2</a31><a31>1.3</a31><a32><a21><a11>17</a11><a12>17.17</a12><a13>Claro enigma</a13></a21><a22>1</a22><a22>2</a22><a22>3</a22></a32></ObjectA3>)" "\n";

ObjectA4* objectA4 = new ObjectA4 {{1.2, 1.3}, objectA2};
QString objectA4XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><object><a31>1.2</a31><a31>1.3</a31><a32><a21><a11>17</a11><a12>17.17</a12><a13>Claro enigma</a13></a21><a22>1</a22><a22>2</a22><a22>3</a22></a32></object>)" "\n";

ObjectA5* objectA5 = new ObjectA5 {{1.2, 1.3}, objectA2};
QString objectA5XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><ObjectA5 xmlns="http://aqui.ali"><a31>1.2</a31><a31>1.3</a31><a32><a21><a11>17</a11><a12>17.17</a12><a13>Claro enigma</a13></a21><a22>1</a22><a22>2</a22><a22>3</a22></a32></ObjectA5>)" "\n";

ObjectB1* objectB1 = new ObjectB1 {17, 17.17, "Claro enigma"};
ObjectB2* objectB2 = new ObjectB2 {1, objectB1, 2};
QString objectB2XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><object><b21>1</b21><b22 xmlns:acolá="http://aqui.ali"><acolá:b11>17</acolá:b11><acolá:b12>17.17</acolá:b12><acolá:b13>Claro enigma</acolá:b13></b22><b23>2</b23></object>)" "\n";

void ExoticaTest::marshal_data() {
    QTest::addColumn<QObject*>("object");
    QTest::addColumn<QString>("xml");
    QTest::newRow("Simple structure of objects") << (QObject*) objectA3 << objectA3XML;
    QTest::newRow("Object with a tag name") << (QObject*) objectA4 << objectA4XML;
    QTest::newRow("Object with a global namespace") << (QObject*) objectA5 << objectA5XML;
    QTest::newRow("Inner object with a namespace") << (QObject*) objectB2 << objectB2XML;
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
