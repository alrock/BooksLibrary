#include "googlevolume.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QDebug>

GoogleThumbnailLoader::GoogleThumbnailLoader(Volume *volume, QScriptValue img_links)
	: ThumbnailLoader(volume), img_links_(img_links) {
	manager_ = new QNetworkAccessManager(this);
}

void GoogleThumbnailLoader::load(const QSize &size) {
	QSize sz(size);

	QScriptValue val;
	QNetworkRequest req;

	if (sz.width() <= 80) {
		val = img_links_.property("smallThumbnail");
		if (val.isString() && !val.toString().isEmpty())
			req.setUrl(QUrl(val.toString()));
		else
			sz.setWidth(128);
	}
	if (sz.width() > 80 && sz.width() <= 128) {
		val = img_links_.property("thumbnail");
		if (val.isString() && !val.toString().isEmpty())
			req.setUrl(QUrl(val.toString()));
		else
			sz.setWidth(300);
	}
	if (sz.width() > 128 && sz.width() <= 300) {
		val = img_links_.property("small");
		if (val.isString() && !val.toString().isEmpty())
			req.setUrl(QUrl(val.toString()));
		else
			sz.setWidth(575);
	}
	if (sz.width() > 300 && sz.width() <= 575) {
		val = img_links_.property("medium");
		if (val.isString() && !val.toString().isEmpty())
			req.setUrl(QUrl(val.toString()));
		else
			sz.setWidth(800);
	}
	if (sz.width() > 575 && sz.width() <= 800) {
		val = img_links_.property("large");
		if (val.isString() && !val.toString().isEmpty())
			req.setUrl(QUrl(val.toString()));
		else
			sz.setWidth(1280);
	}
	if (sz.width() > 800 && sz.width() <= 1280) {
		val = img_links_.property("extraLarge");
		if (val.isString() && !val.toString().isEmpty())
			req.setUrl(QUrl(val.toString()));
		else
			ThumbnailLoader::load(size);
	}

	QPixmap *from_cache = 0;
	if (QPixmapCache::find(thumbnail_key(volume_->vid(), sz), from_cache)) {
		qDebug() << "Load thumbnail from cache";
		emit complete(*from_cache, size);
		return;
	}

	if (!req.url().isEmpty()) {
		load_queue_.insert(req.url(), sz);
		QNetworkReply *reply = manager_->get(req);

		connect(reply, SIGNAL(finished()), this, SLOT(reply_finished()));
		connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(reply_error()));
	}
}

void GoogleThumbnailLoader::reply_finished() {
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if (reply) {
		QByteArray data(reply->readAll());

		qDebug() << "Google Thumbnail reply finished. Size:" << data.size();

		QPixmap pixmap;
		pixmap.loadFromData(data);

		if (load_queue_.contains(reply->request().url())) {
			QSize size = load_queue_[reply->request().url()];
			load_queue_.remove(reply->request().url());
			QPixmapCache::insert(thumbnail_key(volume_->vid(), size), pixmap);
			emit complete(pixmap, size);
		} else
			emit complete(pixmap, QSize());

		reply->deleteLater();

	}
}

void GoogleThumbnailLoader::reply_error() {
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	if (reply) {
		qDebug() << "Google Thumbnail reply error:" << reply->errorString();
		reply->deleteLater();
	}
}

//--------------------------------------------------------------------------------------------------

GoogleVolume::GoogleVolume(const QString &json, QObject *parent)
	: Volume(parent), data_(json) {
	svalue_ = QScriptEngine().evaluate(data_);
	volume_info_ = svalue_.property("volumeInfo");
}

GoogleVolume::GoogleVolume(QScriptValue root, QObject *parent)
	: Volume(parent) {
	svalue_ = root;
	volume_info_ = svalue_.property("volumeInfo");
}

QString GoogleVolume::get_string_property(QScriptValue from, const QString &property) const {
	QScriptValue val = from.property(property);

	return (val.isString()) ? val.toString() : QString();
}

QString GoogleVolume::kind() const {
	return get_string_property(svalue_, "kind");
}

QString GoogleVolume::google_id() const {
	return get_string_property(svalue_, "id");
}

QString GoogleVolume::title() const {
	return get_string_property(volume_info_, "title");
}

QString GoogleVolume::description() const {
	return get_string_property(volume_info_, "description");
}

QStringList GoogleVolume::authors() const {
	QScriptValue val = volume_info_.property("authors");

	QStringList slist;
	if (val.isArray())
		qScriptValueToSequence(val, slist);
	return slist;
}

ThumbnailLoader* GoogleVolume::thumbnail(QObject *parent) {
	return new GoogleThumbnailLoader(this, volume_info_.property("imageLinks"));
}
