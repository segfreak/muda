#include "align.h"

size_t
muda_detail_align_up (size_t value, size_t alignment)
{
  return (value + alignment - 1) & ~(alignment - 1);
}