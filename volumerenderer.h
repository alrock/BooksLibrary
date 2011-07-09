#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

#include <QPainter>
#include <QPixmap>

#include <volume.h>

class VolumeRenderer {
public:
	VolumeRenderer(QPixmap thumbnail);

	QPixmap result() const { return canvas_; }

	~VolumeRenderer();
private:
	void create_canvas();
	void render_thumbnail();
	void render_snipe();
	void render_pages();
private:
	quint32 size_;
	QPixmap thumbnail_;
	QPixmap canvas_;

	QPainter *painter_;

	quint32 pages_height_;
	quint32 snipe_width_;
	QSize thumbnail_size_;

	quint32 thumbnail_height_;
	quint32 thumbnail_width_;
	quint32 total_width_;
	quint32 total_height_;

	QColor dominant_;
};

#endif // VOLUMERENDERER_H
