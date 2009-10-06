#pragma once

namespace FastCgiQt
{
	class Manager;
	class Request;
};

class QLabel;
class QPushButton;
class QWebView;

#include <QWidget>

class EmbedExample : public QWidget
{
	Q_OBJECT;
	public:
		EmbedExample(QWidget* parent = 0);
	private slots:
		void respond(FastCgiQt::Request*);
		void start();
		void stop();
		void refresh();
	private:
		void updateStatus();

		FastCgiQt::Manager* m_manager;
		QWebView* m_webView;
		QLabel* m_statusLabel;
		int m_requestCount;

		QPushButton* m_startButton;
		QPushButton* m_stopButton;
		QPushButton* m_refreshButton;
};
