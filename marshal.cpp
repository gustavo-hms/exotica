#include "exotica.h"
#include "Encoder.h"

class QObject;
class QIODevice;

bool exotica::marshal(QObject* object, QIODevice* destination) {
	Encoder encoder(destination);
	return encoder.encode(object);
}

bool exotica::marshalIndent(QObject* object, QIODevice* destination, int level) {
	Encoder encoder(destination, level);
	return encoder.encode(object);
}
