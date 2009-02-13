#include <FastCgiQt/Service.h>

class IndexService : public FastCgiQt::Service
{
	Q_OBJECT
	SERVICE(IndexService)
	public:
		IndexService(const FastCgiQt::Request& request, QObject* parent = NULL);
	public slots:
		void index();
	protected:
		FastCgiQt::Service::UrlMap urlMap() const;
};
