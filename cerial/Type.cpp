#include "Type.h"
using namespace cerial;

TypeInfo::TypeInfo(const TypeBase& _type) : type(_type) {
	type.refer();
}

TypeInfo::~TypeInfo() {
	type.unrefer();
}

//	番号でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
std::optional<size_t> TypeInfo::operator [](size_t index) const noexcept {
	return type[index];
}

//	名前でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
std::optional<size_t> TypeInfo::operator [](TypeName name) const noexcept {
	return type[name];
}

TypeName TypeInfo::name() const noexcept {
	return type.name;
}

size_t TypeInfo::size() const noexcept {
	return type.size;
}



TypeBase::TypeBase(TypeName name) noexcept
	: name(name) {}

TypeBase::TypeBase(TypeName name, size_t size) noexcept
	: name(name), size(size) {}

TypeBase::operator TypeInfo() const noexcept {
	return {*this};
}


//	番号でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
std::optional<size_t> TypeBase::operator [](size_t index) const noexcept {
	return false;
}

//	名前でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
std::optional<size_t> TypeBase::operator [](TypeName name) const noexcept {
	return false;
}


//	番号でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
std::optional<size_t> ContainerType::operator [](size_t index) const noexcept {
	if(index >= count) return false; 
	return index * base_type.size();
}


ContainerType::ContainerType(TypeName name, const TypeInfo& base_type, size_t count) noexcept
	: TypeBase(name, base_type.size() * count), base_type(base_type), count(count) {}



//	名前でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
std::optional<size_t> StructureType::operator [](TypeName name) const noexcept {
	auto itr = members.find(name);
	if(itr == members.cend()) return false;
	return itr->second.offset;
}


StructureType::StructureType(TypeName name) noexcept
	: TypeBase(name) {}

void StructureType::add(const TypeInfo& type) noexcept {
	members.insert(std::pair<TypeName, TypeIndexInfo>(type.name(), {size, type}));
	size += type.size();
}


ReferenceType::ReferenceType(TypeName name, const TypeInfo& base_type) noexcept
	: TypeBase(name, Reference_Size), base_type(base_type) {}

