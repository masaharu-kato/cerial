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


//	�������̂ňقȂ�ʒu���w���f�[�^�Q�Ƃ��쐬����
DataReference DataReference::reference_at(const DataUnit* new_ptr) const noexcept {
	return {entity, new_ptr};
}

//	�������̂ňʒu�𓮂������f�[�^�Q�Ƃ��쐬����
DataReference DataReference::reference_moved(size_t offset) const noexcept {
	return {entity, ptr + offset};
}

//	�������̂ňʒu�𓮂������f�[�^�Q�Ƃ��쐬����i�l���Ȃ��ꍇ�� nullptr �̃f�[�^�Q�Ƃ��쐬����j
DataReference DataReference::reference_moved(std::optional<size_t> optional_offset) const noexcept {
	if(!optional_offset.has_value()) return reference_at(nullptr);
	return reference_moved(optional_offset.value());
}