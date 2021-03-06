/* LICENSE NOTICE
	Copyright (c) 2009, Frederick Emmott <mail@fredemmott.co.uk>

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#ifndef _FASTCGIQT_EMBEDEXAMPLE
#define _FASTCGIQT_EMBEDEXAMPLE

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
	Q_OBJECT
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
#endif // _FASTCGIQT_EMBEDEXAMPLE
