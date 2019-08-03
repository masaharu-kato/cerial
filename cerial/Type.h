#pragma once
#include <type_traits>
#include <any>
#include <memory>
#include <unordered_map>
#include <optional>
#include "Shared.h"

namespace cerial {

//	型名
	using TypeName = const char*;


	class TypeBase;

	
//	型情報の参照型（TypeBaseの共有ポインタ）
	class TypeInfo {
	public:
		TypeInfo(const TypeBase& type) noexcept;
		TypeInfo(const TypeInfo&) = default;
		~TypeInfo() noexcept;

	//	型名を取得
		TypeName name() const noexcept;
		
	//	型サイズを取得
		size_t size() const noexcept;

	//	番号でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
		std::optional<size_t> operator [](size_t index) const noexcept;

	//	名前でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
		std::optional<size_t> operator [](TypeName name) const noexcept;



	//	新しい型を作る
		template <class _Type, class... Args>
		static TypeInfo create(Args&&... args) noexcept {
			return {*new _Type(args...)};
		}

	private:
		const TypeBase& type;
	};


//	型情報基底型
	class TypeBase : public Shared {
		friend TypeInfo;
	public:
		operator TypeInfo() const noexcept;

	//	番号でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
		virtual std::optional<size_t> operator [](size_t index) const noexcept;

	//	名前でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
		virtual std::optional<size_t> operator [](TypeName name) const noexcept;

	protected:
		TypeBase(TypeName name) noexcept;
		TypeBase(TypeName name, size_t size) noexcept;
		TypeBase(const TypeBase&) = delete;

	//	型サイズ
		size_t size = 0;
		
	private:
	//	型名
		TypeName name = nullptr;
	};


//	単一値型情報
	class ScalarType : public TypeBase {
		friend TypeInfo;
	protected:
		ScalarType() = default;
	};


//	構造型情報
	class StructureType : public TypeBase {
		friend TypeInfo;
	public:
	//	メンバを追加
		void add(const TypeInfo& type) noexcept;

	//	名前でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
		virtual std::optional<size_t> operator [](TypeName name) const noexcept override;

	protected:
		StructureType(TypeName name) noexcept;

	private:
		struct TypeIndexInfo {
			size_t offset;	//	オフセット
			TypeInfo info;	//	メンバ型情報
		};
		
	//	それぞれのメンバ型情報
		std::unordered_map<TypeName, TypeIndexInfo> members;
	};


//	参照型情報
	class ReferenceType : public TypeBase {
		friend TypeInfo;
	protected:
		constexpr static size_t Reference_Size = sizeof(void*);

		ReferenceType(TypeName name, const TypeInfo& base_type) noexcept;

	private:
		TypeInfo base_type;
	};


//	配列型情報
	class ContainerType : public TypeBase {
		friend TypeInfo;
	public:

	//	番号でアクセスしたときの要素オフセットを取得 (取得できない場合は未設定値を返す)
		virtual std::optional<size_t> operator [](size_t index) const noexcept override;

	protected:
		ContainerType(TypeName name, const TypeInfo& base_type, size_t count) noexcept;

	private:
		TypeInfo base_type;
		size_t count;
	};

}