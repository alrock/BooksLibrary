#ifndef SHELFVIEW_H
#define SHELFVIEW_H

#include <QWidget>
#include <QGraphicsItem>
#include <QSharedPointer>
#include <QHash>

#include "volume.h"

class QGraphicsView;
class QGraphicsPixmapItem;
class QGraphicsTextItem;
class Volume;
class Shelf;


class VolumeView : public QObject, public QGraphicsItem {
	Q_OBJECT
public:
	explicit VolumeView(Volume *vol, QGraphicsItem *parent = 0);
	QRectF boundingRect() const { return QRectF(0, 0, 132, 204); }
	void paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*);
private slots:
	void set_thumbnail(QPixmap,QSize);
private:
	Volume *volume_;
	QGraphicsPixmapItem *thumbnail_;
	QGraphicsTextItem *label_;
};


class ShelfView : public QWidget {
    Q_OBJECT
public:
	explicit ShelfView(QWidget *parent = 0);
	explicit ShelfView(QSharedPointer<Shelf> shelf, QWidget *parent = 0);

	QSharedPointer<Shelf> shelf() const
		{ return shelf_; }
	void set_shelf(QSharedPointer<Shelf> shelf);
protected:
	void resizeEvent(QResizeEvent *);
private slots:
	void add_item(VId);
	void remove_item(VId);

	void update_view();
private:
	void initialize();
private:
	QSharedPointer<Shelf> shelf_;
	QGraphicsView *view_;
	QHash<VId, VolumeView*> items_;
};

#endif // SHELFVIEW_H
