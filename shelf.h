#ifndef SHELF_H
#define SHELF_H

#include <QObject>
#include <QHash>

#include "volume.h"

class Shelf : public QObject {
    Q_OBJECT

	typedef Volume* volume_t;
public:
	explicit Shelf(QObject *parent = 0);
	explicit Shelf(QList<volume_t> vlms, QObject *parent = 0);

	volume_t volume(VId id);
	QList<volume_t> volumes();

	int volumes_count() const
		{ return collection_.count(); }

	void add_volume(volume_t vol, bool own = false);
	void remove_volume(volume_t vol);
	void remove_volume(VId id);
signals:
	void volume_added(VId);
	void volume_removed(VId);
private:
	QHash<VId, volume_t> collection_;

};

#endif // SHELF_H
