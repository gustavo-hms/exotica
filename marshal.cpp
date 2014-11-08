#include "exotica.h"
#include "Encoder.h"

class QObject;
class QIODevice;

bool marshal(QObject* object, QIODevice* destination) {
	Encoder encoder(destination);
	return encoder.encode(object);
}

bool marshalIndent(QObject* object, QIODevice* destination, int level) {
	Encoder encoder(destination, level);
	return encoder.encode(object);
}
