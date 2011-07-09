#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QLineEdit;
class ShelfView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
	void auth_complete();
private slots:
	void authenticate();
	void auth_title_checker(QString);
	void get_auth_token();
	void parse_token();
	void error_token();

	void perform_search();
	void search_complete();
private:
	bool is_auth_complete() const;
private:
	QString auth_code_;
	QString access_token_;
	QString refresh_token_;
	qint32  token_expires_in_;

	QLineEdit *search_edit;
	ShelfView *view_;
};

#endif // MAINWINDOW_H
