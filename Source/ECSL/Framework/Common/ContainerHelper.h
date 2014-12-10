#ifndef CONTAINERHELPER_H
#define CONTAINERHELPER_H

namespace ECSL
{
	namespace ContainerHelper
	{
		template<typename T>
		bool AddUniqueElement(T _element, std::vector<T>& _vector)
		{
			for (auto vecElement : _vector)
				if (_element == vecElement)
					return false;
				_vector.push_back(_element);
			return true;
		}

		template<typename T>
		bool RemoveUniqueElement(T _element, std::vector<T>& _vector)
		{
			for (auto it = _vector.begin(); it != _vector.end(); ++it)
			{
				if (_element == *it)
				{
					_vector.erase(it);
					return true;
				}
			}
			return false;
		}

		template<typename KeyType, typename ValueType>
		bool TryAddKey(KeyType _key, std::map<KeyType, ValueType>& _map)
		{
			auto it = _map.find(_key);
			if (it == _map.end())
			{
				_map[_key] = ValueType();
				return true;
			}
			return false;
		}
	}
}

#endif