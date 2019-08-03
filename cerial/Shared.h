#pragma once

namespace cerial {

	class Shared {
	protected:
		Shared() = default;
		virtual ~Shared() = default;

		void refer() const noexcept;
		void unrefer() const noexcept;

	private:
		mutable size_t ref_count = 0;
	};

}