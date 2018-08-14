#include "Timer.h"
#include <memory>

std::shared_ptr<NanoTimer> NanoTimer::m_ptr = std::shared_ptr<NanoTimer>(nullptr);
