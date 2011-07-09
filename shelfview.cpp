#include "shelfview.h"
#include "thumbnailloader.h"
#include "shelf.h"
#include "volumerenderer.h"

#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

#include <QDebug>

VolumeView::VolumeView(Volume *vol, QGraphicsItem *parent)
	: QGraphicsItem(parent), volume_(vol) {
	setFlag(QGraphicsItem::ItemIsMovable);
	//setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	//label_ = new QGraphicsTextItem(volume_->title(), this);
	//label_->setZValue(10);
	thumbnail_ = new QGraphicsPixmapItem(this);
	thumbnail_->setZValue(5);
	ThumbnailLoader *loader = volume_->thumbnail(this);
	connect(loader, SIGNAL(complete(QPixmap,QSize)), this, SLOT(set_thumbnail(QPixmap,QSize)));
	loader->load(QSize(128,128));

	thumbnail_->setPos(2, 2);
	//label_->setPos(0, 180);
}

void VolumeView::set_thumbnail(QPixmap pixmap, QSize) {
	ThumbnailLoader *loader = qobject_cast<ThumbnailLoader*>(sender());
	if (loader) {
		pixmap = pixmap.scaled(QSize(128, 128*1.41), Qt::KeepAspectRatio,
					 Qt::SmoothTransformation);
		VolumeRenderer renderer(pixmap);
		thumbnail_->setPixmap(renderer.result());
		renderer.result().save("hello.png");

		double dx = double(128-pixmap.width())/2.0;
		double dy = double(128*1.41-pixmap.height())/2.0;

		thumbnail_->setOffset((dx > 0)? dx : 0, (dy > 0)? dy : 0);
		thumbnail_->update();
	}
}

void VolumeView::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*) {
	//p->drawRect(0, 0, 132, 204);
	//thumbnail_->update();
	//label_->update();
}

ShelfView::ShelfView(QWidget *parent)
	: QWidget(parent) {
	initialize();
}

ShelfView::ShelfView(QSharedPointer<Shelf> shelf, QWidget *parent)
	: QWidget(parent) {

	initialize();
	set_shelf(shelf);
}

void ShelfView::initialize() {
	QVBoxLayout *main_l = new QVBoxLayout(this);
	main_l->setSpacing(0);
	main_l->setMargin(0);

	QGraphicsScene *scene = new QGraphicsScene;
	view_ = new QGraphicsView(scene);

	main_l->addWidget(view_);
}

void ShelfView::set_shelf(QSharedPointer<Shelf> shelf) {
	shelf_ = shelf;

	connect(shelf.data(), SIGNAL(volume_added(VId)), this, SLOT(add_item(VId)));
	connect(shelf.data(), SIGNAL(volume_removed(VId)), this, SLOT(remove_item(VId)));

	foreach (Volume *v, shelf_->volumes()) {
		items_.insert(v->vid(), new VolumeView(v));
		view_->scene()->addItem(items_[v->vid()]);
	}

	update_view();
}

void ShelfView::add_item(VId vid) {
	items_.insert(vid, new VolumeView(shelf_->volume(vid)));
	view_->scene()->addItem(items_[vid]);
	update_view();
}

void ShelfView::remove_item(VId vid) {
	view_->scene()->removeItem(items_[vid]);
	items_.remove(vid);
	update_view();
}

void ShelfView::resizeEvent(QResizeEvent *) {
	view_->scene()->setSceneRect(view_->geometry());
	update_view();
}

void ShelfView::update_view() {
	if (!shelf_.isNull()) {
		int count = width()/(150+2);
		int i = 0;
		foreach (VolumeView *v, items_.values()) {
			v->setPos((i%count)*(150+2), (i/count)*206);
			++i;
		}
	}
}
