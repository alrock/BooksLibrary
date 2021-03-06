#ifndef VOLUME_H
#define VOLUME_H

#include <QList>
#include <QStringList>

typedef QString VId;

class ThumbnailLoader;

class Volume : public QObject {
	Q_OBJECT
public:
	explicit Volume(QObject *parent = 0);

	quint32 id() const;

	virtual VId vid() const;

	virtual QString title() const = 0;
	virtual QString description() const = 0;
	virtual QStringList authors() const = 0;

	virtual ThumbnailLoader* thumbnail(QObject *parent = 0);

	virtual ~Volume();
private:
	quint32 id_;
private:
	static quint32 id_base_;
	static QList<quint32> free_id_;

	static quint32 next_id();
	static void return_id(quint32 id);
};

#endif // VOLUME_H
