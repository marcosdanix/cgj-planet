#pragma once

#include <string>
#include <map>

namespace cgj {

	template <class T>
	class Storage {
	public:
		static Storage<T>& instance();
		Storage<T>& add(std::string name, T* item);
		Storage<T>& remove(std::string name);
		T* get(std::string name);
		~Storage();
	private:
		std::map<std::string, T*> map_;
		Storage();
	};

	template<class T>
	inline Storage<T>& Storage<T>::instance()
	{
		static Storage<T> instance = new Storage<T>();
		return instance_;
	}

	template<class T>
	inline Storage<T>& Storage<T>::add(std::string name, T* item)
	{
		remove(name);
		map_[name] = item;
	}

	template<class T>
	inline Storage<T>& Storage<T>::remove(std::string name)
	{
		if (map_.find(name) != map_.end()) {
			delete map_[name];
		}
	}

	template<class T>
	inline T * Storage<T>::get(std::string name)
	{
		return map_[name];
	}

	template<class T>
	inline Storage<T>::~Storage()
	{
		for (auto item : map_.begin()) {
			delete item;
		}
	}

	template<class T>
	inline Storage<T>::Storage()
	{
		this->map_ = std::map<std::string, T*>();
	}

}