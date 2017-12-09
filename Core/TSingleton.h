
#ifndef _TSINGLETON_H_
#define _TSINGLETON_H_

#include <assert.h>

#ifdef WIN32
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#endif

namespace Core
{

template<typename T>
class TSingleton
{
private:

	static T*			m_spSingleton;
	
	public:
	
		TSingleton()
		{
			assert(!m_spSingleton && "Singleton already instanced!");
			int offset=(int)(T*)1 - (int)(TSingleton <T>*)(T*)1;
			m_spSingleton=(T*)((int)(this + offset));
		}
		
		~TSingleton()
		{
			assert(m_spSingleton);
			m_spSingleton=0;
		}
		
		static T&					getSingletonRef()
		{
			assert(m_spSingleton && "Singleton not instanced!");
			return *m_spSingleton;
		}
		
		static T*					getSingletonPtr()
		{
			assert(m_spSingleton && "Singleton not instanced!");
			return m_spSingleton;
		}

		static void					deleteSingleton()
		{
			if(m_spSingleton)
			{
				delete m_spSingleton;
				m_spSingleton=0;
			}
		}

		static bool					ssInstanced()				{ return m_spSingleton!=0; }
};

template<typename T> T* TSingleton<T>::m_spSingleton=0;

}

#ifdef WIN32
#pragma warning(default:4311)
#pragma warning(default:4312)
#endif

#endif
