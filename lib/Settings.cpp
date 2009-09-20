#include "Settings.h"

#include <QCoreApplication>
#include <QStringList>

namespace FastCgiQt
{
	Settings::Settings()
	: QSettings(
		QCoreApplication::applicationDirPath() + "/." + QCoreApplication::applicationFilePath().split("/").last(),
		QSettings::IniFormat
	)
	{
	};
};
