#ifndef GOOGLESEARCHENGINE_H
#define GOOGLESEARCHENGINE_H

#include <QObject>
#include "googlevolume.h"

class QNetworkAccessManager;
class QNetworkReply;

class GoogleSearchEngine : public QObject {
    Q_OBJECT
public:
    explicit GoogleSearchEngine(QObject *parent = 0);

	void search(const QString &query);
signals:

private slots:
	void reply_finished();
	void reply_error();
private:
	const QString search_url_;
	QNetworkAccessManager *manager_;
	QNetworkReply *reply_;

	QList<GoogleVolume> volumes_;
};

#endif // GOOGLESEARCHENGINE_H
