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
		typename std::map<std::string, T*>::const_iterator cbegin();
		typename std::map<std::string, T*>::const_iterator cend();
		~Storage();
	private:
		std::map<std::string, T*> map_;
		Storage();
	};

	template<class T>
	inline Storage<T>& Storage<T>::instance()
	{
		static Storage<T>* instance = new Storage<T>();
		return *instance;
	}

	template<class T>
	inline Storage<T>& Storage<T>::add(std::string name, T* item)
	{
		remove(name);
		map_[name] = item;
		return *this;
	}

	template<class T>
	inline Storage<T>& Storage<T>::remove(std::string name)
	{
		if (map_.find(name) != map_.end()) {
			delete map_[name];
		}
		return *this;
	}

	template<class T>
	inline T * Storage<T>::get(std::string name)
	{
		return map_[name];
	}

	template<class T>
	inline typename std::map<std::string, T*>::const_iterator Storage<T>::cbegin()
	{
		return map_.cbegin();
	}

	template<class T>
	inline typename std::map<std::string, T*>::const_iterator Storage<T>::cend()
	{
		return map_.cend();
	}

	template<class T>
	inline Storage<T>::~Storage()
	{
		for (auto item : map_) {
			delete item.second;
		}
	}

	template<class T>
	inline Storage<T>::Storage()
	{
		this->map_ = std::map<std::string, T*>();
	}

}