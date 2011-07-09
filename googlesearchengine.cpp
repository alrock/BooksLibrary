#include "googlesearchengine.h"
#include "googlevolume.h"
#include "shelf.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtScript>

#include <QDebug>

GoogleSearchEngine::GoogleSearchEngine(const QString &query, quint32 count, QObject *parent)
	: QObject(parent), search_url_("https://www.googleapis.com/books/v1/volumes"),
	  search_query_(query), count_(count), index_(1), shelf_(new Shelf) {

	manager_ = new QNetworkAccessManager(this);
}

void GoogleSearchEngine::next(quint32 count) {
	QUrl url(search_url_);
	url.addQueryItem("q", search_query_);
	quint32 c = (count > 0) ? count : count_;
	url.addQueryItem("start-index", QString::number(index_));
	url.addQueryItem("max-results", QString::number(c));
	index_ += c;

	QNetworkReply *reply = manager_->get(QNetworkRequest(url));

	connect(reply, SIGNAL(finished()), this, SLOT(reply_finished()));
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(reply_error()));
}

void GoogleSearchEngine::reply_finished() {
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	if (!reply) {
		qDebug() << "GSE: Oops. Reply is not reply.";
		return;
	}
	QString data(reply->readAll());
	reply->deleteLater();

	qDebug() << "Search reply finished";

	QScriptEngine engine;
	QScriptValue sv = engine.evaluate("("+data+")").property("items");
	if (sv.isArray()) {
		QScriptValueIterator iter(sv);
		while (iter.hasNext()) {
			iter.next();
			if (iter.flags() & QScriptValue::SkipInEnumeration)
				continue;
			Volume *vol = new GoogleVolume(iter.value());
			shelf_->add_volume(vol, true);
		}
		qDebug() << "Parsed" << shelf_->volumes_count() << "volumes";
	}
	emit complete();
}

void GoogleSearchEngine::reply_error() {
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	if (!reply) {
		qDebug() << "GSE: Oops. Reply is not reply.";
		return;
	}
	qDebug() << "Reply error:" << reply->errorString();
	reply->deleteLater();
	emit complete();
}
