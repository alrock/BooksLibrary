#include "thumbnailloader.h"

ThumbnailLoader::ThumbnailLoader(Volume *volume)
	: QObject(volume), volume_(volume) {
}

#include <QLinearGradient>
#include <QPainter>
#include <QDebug>

int text_pt(QFont font, QRect rect, const QString &text, int max_pt = 30,
			int min_pt = 6) {

	for (int pt = max_pt; pt >= min_pt; --pt) {
		font.setPointSize(pt);
		QFontMetrics m(font);
		QRect r = m.boundingRect(rect, Qt::TextWordWrap | Qt::AlignTop | Qt::AlignHCenter, text);
		if (r.width() <= rect.width() && r.height() <= rect.height()) {
			qDebug() << r;
			return pt;
		}
	}
	return 0;
}

void ThumbnailLoader::load(const QSize &size) {
	QPixmap pixmap(128, 192);
	QPainter p(&pixmap);

	QLinearGradient grad(0,0,128,192);
	grad.setColorAt(0, QColor::fromRgb(94,55,19));
	grad.setColorAt(1, QColor::fromRgb(67,34,11));

	p.fillRect(pixmap.rect(), QBrush(grad));

	QFont f = p.font();
	QString title = volume_->title();
	QString author = "Author P.R.";
	QStringList art = volume_->authors();
	if (!art.isEmpty())
		author = art.first();

	QRect rect(8,10,128-16,192-20);


	f.setPointSize(8);
	QFontMetrics m(f);
	author = m.elidedText(author, Qt::ElideRight, rect.width());
	int auth_h = m.boundingRect(author).height();
	QRect rect2(8, rect.bottom()-auth_h+5, rect.width(), auth_h);

	rect.setHeight(rect.height() - auth_h);

	f.setPointSize(text_pt(f, rect, title));
	p.setFont(f);
	p.setPen(QColor::fromRgb(255, 179, 44));
	p.drawText(rect,  Qt::TextWordWrap | Qt::AlignTop | Qt::AlignHCenter, title);
	f.setPointSize(8);
	p.setFont(f);
	p.drawText(rect2, author);

	emit complete(pixmap, size);
}
