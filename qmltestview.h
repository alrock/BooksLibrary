#ifndef QMLTESTVIEW_H
#define QMLTESTVIEW_H

#include <QWidget>

class QmlType: public QObject {
	Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE set_name NOTIFY name_changed)
public:
	QmlType(const QString &name = QString(), QObject *parent = 0): QObject(parent), name_(name) {}
	QString name() const { return name_; }
	void set_name(const QString &name) { name_ = name; }
	~QmlType() {}
signals:
	void name_changed();
private:
	QString name_;
};

class QDeclarativeView;

class QmlTestView : public QWidget {
    Q_OBJECT
public:
    explicit QmlTestView(QWidget *parent = 0);
private slots:
	void timer_shot();
private:
	QDeclarativeView *view_;
};

#endif // QMLTESTVIEW_H
