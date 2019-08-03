#pragma once
#include "Type.h"
#include <stdexcept>

namespace cerial {

	using DataUnit = char;

	class DataReference;

//	データ実体型
	class DataEntity : public Shared {
		friend DataReference;
	private:
		DataEntity(const TypeInfo& type) noexcept;
		DataEntity(const DataEntity&) = delete;

		const TypeInfo type;				//	データ型情報
		std::unique_ptr<DataUnit[]> data;	//	データを格納する配列
	};


//	データ参照型
	class DataReference {
	public:
		DataReference(const DataEntity& entity, const DataUnit* ptr) noexcept;
		~DataReference() noexcept;

		//	型を取得
		const TypeInfo& type() const noexcept;

		//	データがあるかどうか取得
		bool is_defined() const noexcept;

		//	is_defined のエイリアス
		explicit operator bool() const noexcept;

		//	(配列データの場合) 指定インデックス要素を取得する
		DataReference operator [](size_t index) const noexcept;

		//	(構造体データの場合) 指定メンバ要素を取得する
		DataReference operator [](TypeName type_name) const noexcept;
		
		//	実際の型に変換する
		template <class T> operator const T&() const noexcept(false);


		//	新しいデータを作る
		template <class... Args>
		static DataReference create(Args&&... args) noexcept {
			return {*new DataEntity(args...)};
		}


	private:
		//	同じ実体で異なる位置を指すデータ参照を作成する
		DataReference reference_at(const DataUnit* new_ptr) const noexcept;
		
		//	同じ実体で位置を動かしたデータ参照を作成する
		DataReference reference_moved(size_t offset) const noexcept;
		
		
		//	同じ実体で位置を動かしたデータ参照を作成する（値がない場合は nullptr のデータ参照を作成する）
		DataReference reference_moved(std::optional<size_t> optional_offset) const noexcept;
		


		const DataEntity& entity;			//	データ実体へのポインタ情報
		const DataUnit* ptr = nullptr;		//	データの開始ポインタ
	};


	template <class T>
	DataReference::operator const T &() const noexcept(false) {
		if(!ptr) throw std::runtime_error("Data is null.");
		if(sizeof(T) != type().size()) throw std::runtime_error("Data size does not match.");

		return *(T*)ptr;
	}




}