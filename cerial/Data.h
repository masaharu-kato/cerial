#pragma once
#include "Type.h"
#include <stdexcept>

namespace cerial {

	using DataUnit = char;

	class DataReference;

//	�f�[�^���̌^
	class DataEntity : public Shared {
		friend DataReference;
	private:
		DataEntity(const TypeInfo& type) noexcept;
		DataEntity(const DataEntity&) = delete;

		const TypeInfo type;				//	�f�[�^�^���
		std::unique_ptr<DataUnit[]> data;	//	�f�[�^���i�[����z��
	};


//	�f�[�^�Q�ƌ^
	class DataReference {
	public:
		DataReference(const DataEntity& entity, const DataUnit* ptr) noexcept;
		~DataReference() noexcept;

		//	�^���擾
		const TypeInfo& type() const noexcept;

		//	�f�[�^�����邩�ǂ����擾
		bool is_defined() const noexcept;

		//	is_defined �̃G�C���A�X
		explicit operator bool() const noexcept;

		//	(�z��f�[�^�̏ꍇ) �w��C���f�b�N�X�v�f���擾����
		DataReference operator [](size_t index) const noexcept;

		//	(�\���̃f�[�^�̏ꍇ) �w�胁���o�v�f���擾����
		DataReference operator [](TypeName type_name) const noexcept;
		
		//	���ۂ̌^�ɕϊ�����
		template <class T> operator const T&() const noexcept(false);


		//	�V�����f�[�^�����
		template <class... Args>
		static DataReference create(Args&&... args) noexcept {
			return {*new DataEntity(args...)};
		}


	private:
		//	�������̂ňقȂ�ʒu���w���f�[�^�Q�Ƃ��쐬����
		DataReference reference_at(const DataUnit* new_ptr) const noexcept;
		
		//	�������̂ňʒu�𓮂������f�[�^�Q�Ƃ��쐬����
		DataReference reference_moved(size_t offset) const noexcept;
		
		
		//	�������̂ňʒu�𓮂������f�[�^�Q�Ƃ��쐬����i�l���Ȃ��ꍇ�� nullptr �̃f�[�^�Q�Ƃ��쐬����j
		DataReference reference_moved(std::optional<size_t> optional_offset) const noexcept;
		


		const DataEntity& entity;			//	�f�[�^���̂ւ̃|�C���^���
		const DataUnit* ptr = nullptr;		//	�f�[�^�̊J�n�|�C���^
	};


	template <class T>
	DataReference::operator const T &() const noexcept(false) {
		if(!ptr) throw std::runtime_error("Data is null.");
		if(sizeof(T) != type().size()) throw std::runtime_error("Data size does not match.");

		return *(T*)ptr;
	}




}