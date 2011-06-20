#ifndef THUMBNAILLOADER_H
#define THUMBNAILLOADER_H

#include <QObject>
#include <QPixmap>

class ThumbnailLoader : public QObject {
    Q_OBJECT
public:
    explicit ThumbnailLoader(QObject *parent = 0);

	QPixmap thumbnail() const { return thumbnail_; }

public slots:
	virtual void load(const QSize &size);
signals:
	void complete();
protected:
	QPixmap thumbnail_;
};

#endif // THUMBNAILLOADER_H
