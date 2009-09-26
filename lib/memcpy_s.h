#pragma once

// TODO: use the windows function on windows

inline void* memcpy_s(void* destination, size_t destinationSize, const void* source, size_t count)
{
	Q_ASSERT(destinationSize >= count);
	if(destinationSize < count)
	{
		return 0;
	}
	return ::memcpy(destination, source, count);
}
