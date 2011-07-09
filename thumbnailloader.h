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
	explicit ThumbnailLoader(VId vid, QObject *parent = 0);

	VId vid() const { return vid_; }
public slots:
	virtual void load(const QSize &size);
signals:
	void complete(QPixmap, QSize);
protected:
	VId vid_;
};

#endif // THUMBNAILLOADER_H
