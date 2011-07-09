#include "qmltestview.h"

#include <QtDeclarative/QtDeclarative>
#include <QVBoxLayout>
#include <QDebug>

QList<QObject*> model;
QDeclarativeContext *context;

QmlTestView::QmlTestView(QWidget *parent) :
	QWidget(parent) {

	qmlRegisterType<QmlType>("app.bookslibrary", 1, 0, "Test");

	QVBoxLayout *main_layout = new QVBoxLayout(this);

	view_ = new QDeclarativeView;
	QDeclarativeContext *context = view_->engine()->rootContext();


	//QList<QObject*> model;
	model.append(new QmlType("test1", this));
	model.append(new QmlType("test2", this));
	model.append(new QmlType("test3", this));

	context->setContextProperty("testModel", QVariant::fromValue(model));
	view_->setSource(QUrl("/home/alrock/devel/qtcreator/BooksLibrary/test.qml"));

	main_layout->addWidget(view_);

	QTimer::singleShot(2000, this, SLOT(timer_shot()));
}

void QmlTestView::timer_shot() {
	qDebug() << "timer";

	model.last()->setProperty("name", QString("Hello"));

	qDebug() << qobject_cast<QmlType*>(model.last())->name();

	//context->setContextProperty("testModel", QVariant::fromValue(model));
}
