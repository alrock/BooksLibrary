#include "volume.h"
#include "thumbnailloader.h"

quint32 Volume::id_base_ = 0;
QList<quint32> Volume::free_id_ = QList<quint32>();

Volume::Volume(QObject *parent)
	: QObject(parent), id_(next_id()) {}

VId Volume::vid() const {
	return QString("default:%1").arg(id_);
}

quint32 Volume::next_id() {
	if (free_id_.isEmpty())
		return ++id_base_;
	return free_id_.takeLast();
}

void Volume::return_id(quint32 id) {
	if (!free_id_.contains(id))
		free_id_.append(id);
}

ThumbnailLoader* Volume::thumbnail(QObject *parent) {
	return new ThumbnailLoader(this);
}

Volume::~Volume() {
	return_id(id_);
}
