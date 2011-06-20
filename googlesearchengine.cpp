#include "googlesearchengine.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtScript>

#include <QDebug>

GoogleSearchEngine::GoogleSearchEngine(QObject *parent) :
	QObject(parent), search_url_("https://www.googleapis.com/books/v1/volumes") {

	manager_ = new QNetworkAccessManager(this);
	reply_ = 0;
}

void GoogleSearchEngine::search(const QString &query) {
	QUrl url(search_url_);
	url.addQueryItem("q", query);

	reply_ = manager_->get(QNetworkRequest(url));

	connect(reply_, SIGNAL(finished()), this, SLOT(reply_finished()));
	connect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(reply_error()));
}

void GoogleSearchEngine::reply_finished() {
	QString data(reply_->readAll());
	reply_->deleteLater();
	reply_ = 0;

	qDebug() << "Search reply finished";

	volumes_.clear();

	QScriptEngine engine;
	QScriptValue sv = engine.evaluate("("+data+")").property("items");
	if (sv.isArray()) {
		QScriptValueIterator iter(sv);
		while (iter.hasNext()) {
			iter.next();
			if (iter.flags() & QScriptValue::SkipInEnumeration)
				continue;
			volumes_.append(GoogleVolume(iter.value()));
			ThumbnailLoader *l = volumes_.last().thumbnail(this);
			l->load(QSize(128, 128));
		}
		qDebug() << "Parsed" << volumes_.count() << "volumes";
	}

}

void GoogleSearchEngine::reply_error() {
	qDebug() << "Reply error:" << reply_->errorString();
}
