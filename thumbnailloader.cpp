#include "thumbnailloader.h"

ThumbnailLoader::ThumbnailLoader(VId vid, QObject *parent)
	: QObject(parent), vid_(vid) {
}


void ThumbnailLoader::load(const QSize &size) {
	emit complete(QPixmap(), size);
}
