#include "shelfview.h"
#include "thumbnailloader.h"
#include "shelf.h"
#include "volumerenderer.h"

#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsDropShadowEffect>

#include <QDebug>

VolumeView::VolumeView(Volume *vol, QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), volume_(vol) {
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemClipsChildrenToShape);

	ThumbnailLoader *loader = volume_->thumbnail(this);
	connect(loader, SIGNAL(complete(QPixmap,QSize)), this, SLOT(set_thumbnail(QPixmap,QSize)));
	loader->load(QSize(128,128));

	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
	shadow->setColor(Qt::black);
	shadow->setBlurRadius(15.0);
	shadow->setOffset(0, 0);

	setGraphicsEffect(shadow);
}

void VolumeView::set_thumbnail(QPixmap pixmap, QSize) {
	ThumbnailLoader *loader = qobject_cast<ThumbnailLoader*>(sender());
	if (loader) {
		pixmap = pixmap.scaled(QSize(128, 128*1.41), Qt::KeepAspectRatio,
					 Qt::SmoothTransformation);
		VolumeRenderer renderer(pixmap);
		setPixmap(renderer.result());
	}
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
		int w = 0;
		int h = 0, hh = 0;
		QList<VolumeView*> lst;
		foreach (VolumeView *v, items_.values()) {
			if ((w + v->boundingRect().width() + 8) < width()) {
				w += v->boundingRect().width() + 8;
				lst.append(v);
				hh = (hh < v->boundingRect().height()) ? v->boundingRect().height() : hh;
			} else {
				h += hh+8;
				qDebug() << "new line" << h;
				hh = 0;
				int step = 8;
				foreach (VolumeView *vv, lst) {
					vv->setPos(step, h - vv->boundingRect().height());
					step += vv->boundingRect().width() + 8;
				}
				lst.clear();
				lst.append(v);
				w = v->boundingRect().width() + 8;
			}
		}
		h += hh + 8;
		qDebug() << "new line" << h;
		hh = 0;
		int step = 8;
		foreach (VolumeView *vv, lst) {
			vv->setPos(step, h - vv->boundingRect().height());
			step += vv->boundingRect().width() + 8;
		}
		lst.clear();
		w = 0;
	}
}
