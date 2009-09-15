#pragma once

/// Automatically call a given void(void) member function when this leaves scope.
template<class T> class ScopedCaller
{
	public:
		typedef void (T::*Method)();

		ScopedCaller(T* instance, Method method)
		: m_instance(instance)
		, m_method(method)
		{
		}

		~ScopedCaller()
		{
			if(m_method)
			{
				(m_instance->*m_method)();
			}
		}
	private:
		T* m_instance;
		Method m_method;
};
