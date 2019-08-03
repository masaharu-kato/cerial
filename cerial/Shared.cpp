#include "Shared.h"

using namespace cerial;

void Shared::refer() const noexcept {
	++ref_count;
}

void Shared::unrefer() const noexcept {
	if(--ref_count) delete this;
}
