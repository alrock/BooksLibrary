#include "shelf.h"

Shelf::Shelf(QObject *parent) :
	QObject(parent) { }

Shelf::Shelf(QList<volume_t> vlms, QObject *parent)
	: QObject(parent) {
	foreach (volume_t vol, vlms)
		collection_.insert(vol->vid(), vol);
}

Shelf::volume_t Shelf::volume(VId id) {
	return collection_[id];
}

QList<Shelf::volume_t> Shelf::volumes() {
	return collection_.values();
}

void Shelf::add_volume(volume_t vol, bool own) {
	if (own)
		vol->setParent(this);
	collection_.insert(vol->vid(), vol);
	emit volume_added(vol->vid());
}

void Shelf::remove_volume(VId id) {
	collection_.remove(id);
	emit volume_removed(id);
}

void Shelf::remove_volume(volume_t vol) {
	remove_volume(vol->vid());
}
