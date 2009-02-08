#include <FastCgiQt/ServiceMapper.h>

#include <QObject>

class ServiceExample : public FastCgiQt::ServiceMapper
{
	Q_OBJECT;
	SERVICE_MAPPER(ServiceExample);
	protected:
		void loadServices();
};
