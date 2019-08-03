#pragma once
#include <any>
#include <unordered_map>
#include <functional>

namespace cerial {

	class TypeList {
	public:
		using TypeName = const char*;

		template <class T> void insert();
		template <class T> void insert(TypeName type_name);

		template <class T> T cast(TypeName type_name, void* target) const;
		std::any cast(TypeName type_name, void* target) const;

	private:
	};

}
