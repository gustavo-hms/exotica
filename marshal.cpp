#include "exotica.h"
#include "XMLEncoder.h"

class QObject;
class QIODevice;

bool marshal(QObject* object, QIODevice* destination) {
	XMLEncoder encoder(destination);
	return encoder.encode(object);
}

bool marshalIndent(QObject* object, QIODevice* destination, int level) {
	XMLEncoder encoder(destination, level);
	return encoder.encode(object);
}
