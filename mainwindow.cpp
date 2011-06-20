#include "mainwindow.h"

#include <QtWebKit/QWebView>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValueIterator>
#include <QtGui>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	QWidget *main = new QWidget;
	QHBoxLayout *top_panel = new QHBoxLayout(main);

	QPushButton *login = new QPushButton("Login");
	search_edit = new QLineEdit;

	top_panel->addWidget(login);
	top_panel->addWidget(search_edit);

	setCentralWidget(main);

	connect(login, SIGNAL(clicked()), this, SLOT(authenticate()));
	connect(search_edit, SIGNAL(returnPressed()), this, SLOT(perform_search()));
}

void MainWindow::authenticate() {
	QWebView *auth_dialog = new QWebView;
	QUrl auth_url("https://accounts.google.com/o/oauth2/auth?"
				  "client_id=448961656953.apps.googleusercontent.com&"
				  "redirect_uri=urn:ietf:wg:oauth:2.0:oob&"
				  "scope=https://www.googleapis.com/auth/books&"
				  "response_type=code");
	auth_dialog->load(auth_url);

	connect(auth_dialog, SIGNAL(titleChanged(QString)), this, SLOT(auth_title_checker(QString)));
	connect(auth_dialog, SIGNAL(destroyed()), this, SLOT(get_auth_token()));

	auth_dialog->show();
}

void MainWindow::auth_title_checker(QString title) {
	if (title.startsWith("Success code=")) {
		auth_code_ = title.remove("Success code=");
		qDebug() << "Success code:" << auth_code_;
		sender()->deleteLater();
	} else if (title.startsWith("Denied error=")) {
		auth_code_.clear();
		qDebug() << "Denied code:" << title.remove("Denied error=");
		sender()->deleteLater();
	}
}

void MainWindow::get_auth_token() {
	if (!auth_code_.isEmpty()) {
		QUrl token_url("https://accounts.google.com/o/oauth2/token");
		QNetworkRequest token_request(token_url);
		token_request.setHeader(QNetworkRequest::ContentTypeHeader,
								QVariant("application/x-www-form-urlencoded"));
		QByteArray data(QString("client_id=448961656953.apps.googleusercontent.com&"
								"client_secret=vlnlMjq1hYT8o9Zuyi1-KMQ7&"
								"code=%1&"
								"redirect_uri=urn:ietf:wg:oauth:2.0:oob&"
								"grant_type=authorization_code").arg(auth_code_).toAscii());
		QNetworkAccessManager *manager = new QNetworkAccessManager(this);
		QNetworkReply *token_reply = manager->post(token_request, data);

		connect(token_reply, SIGNAL(finished()), this, SLOT(parse_token()));
		connect(token_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error_token()));
	}
}

void MainWindow::parse_token() {
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	if (reply) {
		QString data(reply->readAll());

		qDebug() << "Token reply:" << data;

		QScriptEngine engine;
		QScriptValue sv = engine.evaluate("("+data+")");

		if (sv.property("access_token").isString()) {
			access_token_ = sv.property("access_token").toString();
			qDebug() << "Access token:" << access_token_;
		}
		if (sv.property("expires_in").isNumber()) {
			token_expires_in_ = sv.property("expires_in").toInt32();
			qDebug() << "Token expires in:" << token_expires_in_;
		} else
			token_expires_in_ = 0;
		if (sv.property("refresh_token").isString()) {
			refresh_token_ = sv.property("refresh_token").toString();
			qDebug() << "Refresh token:" << refresh_token_;
		}

		reply->deleteLater();
	} else {
		qDebug() << "Oops. P. Reply is not reply?";
	}
	emit auth_complete();
}

void MainWindow::error_token() {
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	if (reply) {
		qDebug() << "Token reply error code:" << reply->errorString();
	} else {
		qDebug() << "Oops. E. Reply is not reply?";
		reply->deleteLater();
	}
}

bool MainWindow::is_auth_complete() const {
	return !(auth_code_.isEmpty() | access_token_.isEmpty() | token_expires_in_
			| refresh_token_.isEmpty());
}

#include "googlesearchengine.h"

void MainWindow::perform_search() {
	GoogleSearchEngine *engine = new GoogleSearchEngine(this);
	engine->search(search_edit->text());
}

MainWindow::~MainWindow()
{

}
