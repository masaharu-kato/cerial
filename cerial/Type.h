#pragma once
#include <type_traits>
#include <any>
#include <memory>
#include <unordered_map>
#include <optional>
#include "Shared.h"

namespace cerial {

//	�^��
	using TypeName = const char*;


	class TypeBase;

	
//	�^���̎Q�ƌ^�iTypeBase�̋��L�|�C���^�j
	class TypeInfo {
	public:
		TypeInfo(const TypeBase& type) noexcept;
		TypeInfo(const TypeInfo&) = default;
		~TypeInfo() noexcept;

	//	�^�����擾
		TypeName name() const noexcept;
		
	//	�^�T�C�Y���擾
		size_t size() const noexcept;

	//	�ԍ��ŃA�N�Z�X�����Ƃ��̗v�f�I�t�Z�b�g���擾 (�擾�ł��Ȃ��ꍇ�͖��ݒ�l��Ԃ�)
		std::optional<size_t> operator [](size_t index) const noexcept;

	//	���O�ŃA�N�Z�X�����Ƃ��̗v�f�I�t�Z�b�g���擾 (�擾�ł��Ȃ��ꍇ�͖��ݒ�l��Ԃ�)
		std::optional<size_t> operator [](TypeName name) const noexcept;



	//	�V�����^�����
		template <class _Type, class... Args>
		static TypeInfo create(Args&&... args) noexcept {
			return {*new _Type(args...)};
		}

	private:
		const TypeBase& type;
	};


//	�^�����^
	class TypeBase : public Shared {
		friend TypeInfo;
	public:
		operator TypeInfo() const noexcept;

	//	�ԍ��ŃA�N�Z�X�����Ƃ��̗v�f�I�t�Z�b�g���擾 (�擾�ł��Ȃ��ꍇ�͖��ݒ�l��Ԃ�)
		virtual std::optional<size_t> operator [](size_t index) const noexcept;

	//	���O�ŃA�N�Z�X�����Ƃ��̗v�f�I�t�Z�b�g���擾 (�擾�ł��Ȃ��ꍇ�͖��ݒ�l��Ԃ�)
		virtual std::optional<size_t> operator [](TypeName name) const noexcept;

	protected:
		TypeBase(TypeName name) noexcept;
		TypeBase(TypeName name, size_t size) noexcept;
		TypeBase(const TypeBase&) = delete;

	//	�^�T�C�Y
		size_t size = 0;
		
	private:
	//	�^��
		TypeName name = nullptr;
	};


//	�P��l�^���
	class ScalarType : public TypeBase {
		friend TypeInfo;
	protected:
		ScalarType() = default;
	};


//	�\���^���
	class StructureType : public TypeBase {
		friend TypeInfo;
	public:
	//	�����o��ǉ�
		void add(const TypeInfo& type) noexcept;

	//	���O�ŃA�N�Z�X�����Ƃ��̗v�f�I�t�Z�b�g���擾 (�擾�ł��Ȃ��ꍇ�͖��ݒ�l��Ԃ�)
		virtual std::optional<size_t> operator [](TypeName name) const noexcept override;

	protected:
		StructureType(TypeName name) noexcept;

	private:
		struct TypeIndexInfo {
			size_t offset;	//	�I�t�Z�b�g
			TypeInfo info;	//	�����o�^���
		};
		
	//	���ꂼ��̃����o�^���
		std::unordered_map<TypeName, TypeIndexInfo> members;
	};


//	�Q�ƌ^���
	class ReferenceType : public TypeBase {
		friend TypeInfo;
	protected:
		constexpr static size_t Reference_Size = sizeof(void*);

		ReferenceType(TypeName name, const TypeInfo& base_type) noexcept;

	private:
		TypeInfo base_type;
	};


//	�z��^���
	class ContainerType : public TypeBase {
		friend TypeInfo;
	public:

	//	�ԍ��ŃA�N�Z�X�����Ƃ��̗v�f�I�t�Z�b�g���擾 (�擾�ł��Ȃ��ꍇ�͖��ݒ�l��Ԃ�)
		virtual std::optional<size_t> operator [](size_t index) const noexcept override;

	protected:
		ContainerType(TypeName name, const TypeInfo& base_type, size_t count) noexcept;

	private:
		TypeInfo base_type;
		size_t count;
	};

}