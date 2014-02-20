#ifndef ReleaseReferenceCounted
#define ReleaseReferenceCounted(x) { if(x){ x->drop(); x = nullptr; } }
#endif

#ifndef AddReferenceCounted
#define AddReferenceCounted(x) { if(x){ x->grab();} }
#endif

#if defined(DEBUG) || defined(_DEBUG) 
#define GF_PRINT_CONSOLE_INFO(format, ...) fprintf(stderr, format, ## __VA_ARGS__)

#else

#define GF_PRINT_CONSOLE_INFO(format, ...)

#endif

#ifndef _DECLARE_RESOURCE_DESTROY_METHODS
#define _DECLARE_RESOURCE_DESTROY_METHODS(T)			\
	virtual bool destroy(const std::string& name) = 0;	\
	virtual bool destroy(T* object) = 0;				\

#endif

#ifndef _DEFINE_RESOURCE_DESTROY_METHODS
#define _DEFINE_RESOURCE_DESTROY_METHODS(Class, map, T)	\
	bool destroy(const std::string& name)			\
	{												\
		auto it = map.find(name);					\
		if (it == map.end()) return false;			\
			T* object = it->second;					\
		if (object->getReferenceCount() == 1)		\
		{											\
			object->drop();							\
			map.erase(it);							\
			return true;							\
		}											\
		return false;								\
	}												\
													\
	bool destroy(T* object)							\
	{												\
		if (!object)								\
			return false;							\
													\
		auto it = map.find(object->getName());		\
		if (it == map.end())						\
			return false;							\
		if (object->getReferenceCount() == 1)		\
		{											\
		    object->drop();							\
			map.erase(it);							\
			return true;							\
		}											\
		return false;								\
	}												\
													\
	void destroyAll()								\
	{													\
	for (auto it = map.begin(); it != map.end(); it++)	\
		ReleaseReferenceCounted(it->second);				\
	}													\
	~Class()											\
	{													\
		destroyAll();									\
	}													\

#endif

/*
#ifndef _DEFINE_RESOURCE_DESTORY_FUNCTIONS
#define _DEFINE_RESOURCE_DESTORY_FUNCTIONS(map, T) \
	bool destroy(const std::string& name)			\
	{											\
		auto it = map.find(name);				\	
		if (it == map.end())					\
			return false;						\
		T* object = it->second;					\
		if (object->getReferenceCount() == 1)	\
		{										\
			object->drop();						\
			map.erase(it);						\
			return true;						\
		}										\
		return false;							\
	}											\
#endif
	*/