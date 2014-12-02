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

		template<typename KeyType, typename ValueType>
		bool TryAddKey(KeyType _key, std::map<KeyType, ValueType>& _map)
		{
			auto it = _map.find(_key);
			if (it == _map.end())
			{
				printf("2");
				_map[_key] = ValueType();
				printf("2.5");
				return true;
			}
			return false;
		}
	}
}

#endif