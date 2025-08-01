#include "../detail/api.h"
#include "../unit/unit.h"

WEAK void
ut_register (unit_test_fn fn, const char *name)
{
  (void)fn;
  (void)name;
}

WEAK int
ut_run_all (void)
{
  return 0;
}

WEAK void
ut_basic_assert (int condition, const char *cond_str, const char *msg)
{
  (void)condition;
  (void)cond_str;
  (void)msg;
}