#ifndef THUMBNAILLOADER_H
#define THUMBNAILLOADER_H

#include <QObject>
#include <QPixmap>
#include <QPixmapCache>

#include "volume.h"

inline QString thumbnail_key(VId vid, QSize size) {
	return QString(vid).append(QString::number(size.width()));
}

class ThumbnailLoader : public QObject {
    Q_OBJECT
public:
	explicit ThumbnailLoader(Volume *volume);

	VId vid() const { return volume_->vid(); }
public slots:
	virtual void load(const QSize &size);
signals:
	void complete(QPixmap, QSize);
protected:
	Volume *volume_;
};

#endif // THUMBNAILLOADER_H
