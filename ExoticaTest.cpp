#include "ExoticaTest.h"
#include "exotica.h"

auto objectA1 = new ObjectA1 {17, 17.17, "Claro enigma"};
auto objectA2 = new ObjectA2 {objectA1, {1, 2, 3}};
auto objectA3 = new ObjectA3 {{1.2, 1.3}, objectA2};
QString objectA3XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><ObjectA3><a31>1.2</a31><a31>1.3</a31><a32><a21><a11>17</a11><a12>17.17</a12><a13>Claro enigma</a13></a21><a22>1</a22><a22>2</a22><a22>3</a22></a32></ObjectA3>)" "\n";

auto objectA4 = new ObjectA4 {{1.2, 1.3}, objectA2};
QString objectA4XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><object><a31>1.2</a31><a31>1.3</a31><a32><a21><a11>17</a11><a12>17.17</a12><a13>Claro enigma</a13></a21><a22>1</a22><a22>2</a22><a22>3</a22></a32></object>)" "\n";

auto objectA5 = new ObjectA5 {{1.2, 1.3}, objectA2};
QString objectA5XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><ObjectA5 xmlns="http://aqui.ali"><a31>1.2</a31><a31>1.3</a31><a32><a21><a11>17</a11><a12>17.17</a12><a13>Claro enigma</a13></a21><a22>1</a22><a22>2</a22><a22>3</a22></a32></ObjectA5>)" "\n";

auto objectB1 = new ObjectB1 {17, 17.17, "Claro enigma"};
auto objectB2 = new ObjectB2 {1, objectB1, 2};
QString objectB2XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><object xmlns="http://um.nome"><b21>1</b21><b22 xmlns:acolá="http://aqui.ali"><acolá:b11>17</acolá:b11><acolá:b12>17.17</acolá:b12><acolá:b13>Claro enigma</acolá:b13></b22><b23>2</b23></object>)" "\n";

auto objectB3 = new ObjectB3 {17, 17.17, "Claro enigma"};
auto objectB2_3 = new ObjectB2 {1, objectB3, 2};
QString objectB2_3XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><object xmlns="http://um.nome"><b21>1</b21><b22 xmlns:acolá="http://aqui.ali"><acolá:b11>17</acolá:b11><b12>17.17</b12><acolá:b13>Claro enigma</acolá:b13></b22><b23>2</b23></object>)" "\n";

auto objectB4 = new ObjectB4 {17, 17.17, "Claro enigma"};
auto objectB2_4 = new ObjectB2 {1, objectB4, 2};
QString objectB2_4XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><object xmlns="http://um.nome"><b21>1</b21><b22><outro nome>17</outro nome><b12>17.17</b12><b13>Claro enigma</b13></b22><b23>2</b23></object>)" "\n";

auto objectB5 = new ObjectB5 {17, 17.17, "Claro enigma"};
auto objectB2_5 = new ObjectB2 {1, objectB5, 2};
QString objectB2_5XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><object xmlns="http://um.nome"><b21>1</b21><b22 b13="Claro enigma"><b11>17</b11><b12>17.17</b12></b22><b23>2</b23></object>)" "\n";

auto objectB6 = new ObjectB6 {"<Olá!>"};
QString objectB6XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><ObjectB6>&lt;Olá!&gt;</ObjectB6>)" "\n";

auto objectB7 = new ObjectB7 {"<Olá!>"};
QString objectB7XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><ObjectB7><Olá!></ObjectB7>)" "\n";

auto objectB8 = new ObjectB8 {0, 0, ""};
QString objectB8XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><ObjectB8><b12>0</b12></ObjectB8>)" "\n";

auto objectB9 = new ObjectB9 {0, 0, "Itaporanga"};
QString objectB9XML = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><ObjectB9><n1:prop2 xmlns:n1="http://um.dois">0</n1:prop2><b13>Itaporanga</b13></ObjectB9>)" "\n";

void ExoticaTest::marshal_data() {
	QTest::addColumn<QObject*>("object");
	QTest::addColumn<QString>("xml");
	QTest::newRow("Simple structure of objects") << (QObject*) objectA3 << objectA3XML;
	QTest::newRow("Object with a tag name") << (QObject*) objectA4 << objectA4XML;
	QTest::newRow("Object with a global namespace") << (QObject*) objectA5 << objectA5XML;
	QTest::newRow("Inner object with a namespace") << (QObject*) objectB2 << objectB2XML;
	QTest::newRow("Member with the namespace of a parent") << (QObject*) objectB2_3 << objectB2_3XML;
	QTest::newRow("Member with an alias") << (QObject*) objectB2_4 << objectB2_4XML;
	QTest::newRow("An attribute") << (QObject*) objectB2_5 << objectB2_5XML;
	QTest::newRow("Chardata") << (QObject*) objectB6 << objectB6XML;
	QTest::newRow("Inner XML") << (QObject*) objectB7 << objectB7XML;
	QTest::newRow("Omitempty") << (QObject*) objectB8 << objectB8XML;
	QTest::newRow("Mixed member attributes") << (QObject*) objectB9 << objectB9XML;
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
