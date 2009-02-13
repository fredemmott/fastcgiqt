#include <FastCgiQt/Service.h>
class ArticleService : public FastCgiQt::Service
{
	Q_OBJECT
	SERVICE(ArticleService)
	public:
		ArticleService(const FastCgiQt::Request&, QObject* parent = NULL);
	public slots:
		void list();
		void list(const QString& year);
		void list(const QString& year, const QString& month);
		void list(const QString& year, const QString& month, const QString& day);
		void show(const QString& year, const QString& month, const QString& day, const QString& title);
	protected:
		FastCgiQt::Service::UrlMap urlMap() const;
};
