#ifndef GOOGLESEARCHENGINE_H
#define GOOGLESEARCHENGINE_H

#include <QObject>
#include <QSharedPointer>

class QNetworkAccessManager;
class QNetworkReply;
class Shelf;

class GoogleSearchEngine : public QObject {
    Q_OBJECT
public:
	explicit GoogleSearchEngine(const QString &query, quint32 count = 10, QObject *parent = 0);

	QSharedPointer<Shelf> shelf() const
		 { return shelf_; }
public slots:
	void next(quint32 count = 0);
signals:
	void complete();
private slots:
	void reply_finished();
	void reply_error();
private:
	const QString search_url_;
	const QString search_query_;
	quint32 count_;
	quint32 index_;
	QNetworkAccessManager *manager_;

	QSharedPointer<Shelf> shelf_;
};

#endif // GOOGLESEARCHENGINE_H
