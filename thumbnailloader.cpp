#include "thumbnailloader.h"

ThumbnailLoader::ThumbnailLoader(QObject *parent) :
	QObject(parent) {
}

void ThumbnailLoader::load(const QSize &size) {
	thumbnail_ = QPixmap(size);
	emit complete();
}
