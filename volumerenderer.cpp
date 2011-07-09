#include "volumerenderer.h"

#include <QLinearGradient>

QColor dominant_color(const QPixmap &pix, quint8 quality = 5) {
	QImage img(pix.toImage());
	int size = img.size().height()*img.size().width();
	int step = qMax(size*5/100, 1);

	int r = 0, g = 0, b = 0, i = 0, count = 0;
	for (; i < size; i+=step) {
		QRgb color = img.pixel(i%img.width(), i/img.height());
		r += qRed(color);
		g += qGreen(color);
		b += qBlue(color);
		++count;
	}
	r /= count;
	g /= count;
	b /= count;
	return QColor::fromRgb(r,g,b);
}

QColor darker(const QColor &color, double percent) {
	return QColor::fromRgb(color.red()-color.red()*percent,
						   color.green()-color.green()*percent,
						   color.blue()-color.blue()*percent);
}

VolumeRenderer::VolumeRenderer(QPixmap thumbnail)
	: thumbnail_(thumbnail), size_(200), painter_(0) {
	create_canvas();
	render_thumbnail();
	render_snipe();
	render_pages();
}

QRect size_to_rect(QSize size, int dx = 0, int dy = 0) {
	return QRect(dx, dy, size.width(), size.height());
}

void VolumeRenderer::create_canvas() {
	pages_height_ = (size_/50 > 0) ? size_/50 : 1;
	pages_height_ *= 2;
	pages_height_ += 1;

	thumbnail_size_.setHeight(thumbnail_.height() + 1);
	thumbnail_size_.setWidth(thumbnail_.width());

	total_height_ = thumbnail_.height() + 2 + pages_height_ + 1;

	snipe_width_ = 16;
	canvas_ = QPixmap(snipe_width_ + thumbnail_.width() + 1, total_height_);
	painter_ = new QPainter(&canvas_);

	dominant_ = dominant_color(thumbnail_);
}

void VolumeRenderer::render_thumbnail() {
	painter_->fillRect(canvas_.rect(), Qt::white);
	painter_->setPen(dominant_);
	painter_->drawRect(size_to_rect(thumbnail_size_, snipe_width_, 0));
	painter_->drawPixmap(snipe_width_, 1, thumbnail_);

	painter_->setPen(QColor::fromRgba(qRgba(255,255,255,100)));
	painter_->drawLine(snipe_width_+1, 1, canvas_.width()-2, 1);

	painter_->setPen(QColor::fromRgba(qRgba(255,255,255,70)));
	painter_->drawLine(canvas_.width()-2, 1, canvas_.width()-2, thumbnail_size_.height()-1);

	//painter_->setPen(QColor::fromRgba(qRgba(255,255,255,50)));
	painter_->drawLine(snipe_width_+1, thumbnail_size_.height()-1, canvas_.width()-2,
					   thumbnail_size_.height()-1);

	painter_->setPen(QColor::fromRgba(qRgba(255,255,255,20)));
	painter_->drawLine(snipe_width_+1, 0, snipe_width_+1, thumbnail_size_.height());
}

void VolumeRenderer::render_snipe() {
	painter_->setPen(darker(dominant_, 0.8));
	QLinearGradient gradient(QPointF(0,thumbnail_size_.height()), QPointF(snipe_width_, 0));
	QGradientStops stops;
	stops.append(QGradientStop(0.8, darker(dominant_, 0.5)));
	stops.append(QGradientStop(0.0, darker(dominant_, 0.7)));
	gradient.setStops(stops);
	gradient.setSpread(QGradient::PadSpread);
	painter_->setBrush(QBrush(gradient));
	painter_->drawRect(0,0,snipe_width_,thumbnail_size_.height());
	painter_->setPen(QColor::fromRgba(qRgba(255,255,255,20)));
	painter_->setBrush(Qt::NoBrush);
	painter_->drawRect(1,1,snipe_width_-2,thumbnail_size_.height()-2);
}

void VolumeRenderer::render_pages() {
	painter_->setPen(dominant_);
	painter_->drawLine(0, canvas_.height()-1, canvas_.width(), canvas_.height()-1);
	QColor bc = darker(dominant_, 0.8);
	bc.setAlpha(240);
	painter_->setPen(bc);
	painter_->drawLine(0, thumbnail_size_.height(), 0, canvas_.height()-2);

	painter_->fillRect(1, thumbnail_size_.height()+1, canvas_.width()-3, pages_height_,
					   Qt::white);
	painter_->setPen(QColor::fromRgba(qRgba(0,0,0,60)));
	for (quint32 i = 1; i < pages_height_; i+=2) {
		painter_->drawLine(1, thumbnail_size_.height()+1+i, canvas_.width()-3,
						   thumbnail_size_.height()+1+i);
	}
	bc.setAlpha(100);
	painter_->setPen(bc);
	painter_->drawLine(canvas_.width()-3, thumbnail_size_.height()+1, canvas_.width()-3,
					   canvas_.height()-2);
}

VolumeRenderer::~VolumeRenderer() {
	delete painter_;
}
