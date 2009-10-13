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
#include "EmbedExample.h"

#include <FastCgiQt/Manager.h>
#include <FastCgiQt/Request.h>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextStream>
#include <QWebView>

EmbedExample::EmbedExample(QWidget* parent)
: QWidget(parent)
, m_manager(0)
, m_webView(new QWebView(this))
, m_statusLabel(new QLabel(this))
, m_requestCount(0)
, m_startButton(new QPushButton(tr("Start Server"), this))
, m_stopButton(new QPushButton(tr("Stop Server"), this))
, m_refreshButton(new QPushButton(tr("Refresh"), this))
{
	// Initial state
	m_stopButton->setEnabled(false);
	m_refreshButton->setEnabled(false);
	m_webView->setUrl(QUrl("about:blank"));
	updateStatus();

	// Signal/slot connections
	connect(m_startButton, SIGNAL(clicked()), SLOT(start()));
	connect(m_stopButton, SIGNAL(clicked()), SLOT(stop()));
	connect(m_refreshButton, SIGNAL(clicked()), SLOT(refresh()));

	// Layout

	QGridLayout* layout = new QGridLayout();
	setLayout(layout);

	layout->addWidget(m_startButton, 0, 0);
	layout->addWidget(m_stopButton, 0, 1);
	layout->addWidget(m_refreshButton, 0, 2);
	layout->addWidget(m_webView, 1, 0, 1, 3);
	layout->addWidget(m_statusLabel, 2, 0, 1, 3);
}

void EmbedExample::updateStatus()
{
	QString pattern;
	if(m_manager)
	{
		pattern = tr("Server is running on http://localhost:8080, %1 requests handled.");
	}
	else
	{
		pattern = tr("Server is not running, %1 requests handled.");
	}
	m_startButton->setDisabled(m_manager);
	m_stopButton->setEnabled(m_manager);
	m_refreshButton->setEnabled(m_manager);

	m_statusLabel->setText(pattern.arg(m_requestCount));
}

void EmbedExample::respond(FastCgiQt::Request* request)
{
	QTextStream out(request);
	out << "<p>You are lucky request number " << ++m_requestCount << "</p>";
	updateStatus();
}

void EmbedExample::start()
{
	Q_ASSERT(!m_manager);
	QMap<QString, QVariant> options;
	options["portNumber"] = 8080;
	m_manager = new FastCgiQt::Manager("HTTP", options, this);
	connect(
		m_manager,
		SIGNAL(newRequest(FastCgiQt::Request*)),
		SLOT(respond(FastCgiQt::Request*))
	);
	updateStatus();
}

void EmbedExample::stop()
{
	Q_ASSERT(m_manager);
	delete m_manager;
	m_manager = 0;
	updateStatus();
}

void EmbedExample::refresh()
{
	Q_ASSERT(m_manager);
	m_webView->setUrl(QUrl("http://localhost:8080"));
}
