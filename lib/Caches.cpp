#include "Caches.h"

namespace FastCgiQt
{
	QAtomicInt Caches::m_referenceCount(0);
	QPointer<FileCache> Caches::m_fileCache;
	QPointer<FileDependentCache> Caches::m_requestCache;

	Caches::Caches()
	{
		m_referenceCount.ref();
		// Initialise caches
		fileCache();
		requestCache();
	}

	Caches::~Caches()
	{
		// cleanup caches
		if(!m_referenceCount.deref())
		{
			delete m_fileCache;
			delete m_requestCache;
		}
	}

	FileCache& Caches::fileCache()
	{
		if(!m_fileCache)
		{
			m_fileCache = new FileCache("FileCache");
		}
		return *m_fileCache;
	}

	FileDependentCache& Caches::requestCache()
	{
		if(!m_requestCache)
		{
			m_requestCache = new FileDependentCache("RequestCache");
		}
		return *m_requestCache;
	}
}
