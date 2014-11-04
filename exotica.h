#ifndef EXOTICA_H
#define EXOTICA_H

class QObject;
class QIODevice;

bool marshal(QObject* object, QIODevice* destination);
bool marshalIndent(QObject* object, QIODevice* destination, int level = 2);
/* bool unmarshal(QIODevice* source, QObject* object); */

#endif // EXOTICA_H
