#include "Data.h"

using namespace cerial;

DataEntity::DataEntity(const TypeInfo& type)
	: type(type), data(new DataUnit[type.size()]) {}

DataReference::DataReference(const DataEntity& entity, const DataUnit* ptr) noexcept
	: entity(entity), ptr(ptr) 
{
	entity.refer();
}

DataReference::~DataReference() noexcept {
	entity.unrefer();
}

const TypeInfo& DataReference::type() const noexcept {
	return entity.type;
}

bool DataReference::is_defined() const noexcept {
	return ptr;
}

DataReference::operator bool() const noexcept {
	return is_defined();
}


DataReference DataReference::operator [](size_t index) const noexcept {
	return reference_moved(type()[index]);
}


DataReference DataReference::operator [](TypeName index) const noexcept {
	return reference_moved(type()[index]);
}


//	同じ実体で異なる位置を指すデータ参照を作成する
DataReference DataReference::reference_at(const DataUnit* new_ptr) const noexcept {
	return {entity, new_ptr};
}

//	同じ実体で位置を動かしたデータ参照を作成する
DataReference DataReference::reference_moved(size_t offset) const noexcept {
	return {entity, ptr + offset};
}

//	同じ実体で位置を動かしたデータ参照を作成する（値がない場合は nullptr のデータ参照を作成する）
DataReference DataReference::reference_moved(std::optional<size_t> optional_offset) const noexcept {
	if(!optional_offset.has_value()) return reference_at(nullptr);
	return reference_moved(optional_offset.value());
}
