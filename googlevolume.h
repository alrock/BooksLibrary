#ifndef GOOGLEVOLUME_H
#define GOOGLEVOLUME_H

#include "volume.h"
#include "thumbnailloader.h"

#include <QtScript>

class QNetworkAccessManager;

class GoogleThumbnailLoader : public ThumbnailLoader {
	Q_OBJECT
public:
	GoogleThumbnailLoader(VId vid, QScriptValue img_links, QObject *parent = 0);
public slots:
	void load(const QSize &size);
private slots:
	void reply_finished();
	void reply_error();
private:
	QScriptValue img_links_;
	QNetworkAccessManager *manager_;
	QMap<QUrl, QSize> load_queue_;
};

class GoogleVolume : public Volume {
	Q_OBJECT
public:
	explicit GoogleVolume(const QString &json, QObject *parent = 0);
	explicit GoogleVolume(QScriptValue root, QObject *parent = 0);

	QString kind() const;
	QString google_id() const;

	QString title() const;
	QString description() const;
	QStringList authors() const;
	ThumbnailLoader* thumbnail(QObject *parent);
private:
	QString get_string_property(QScriptValue from, const QString &property) const;
private:
	QString data_;

	QScriptValue svalue_;
	QScriptValue volume_info_;
};

#endif // GOOGLEVOLUME_H
