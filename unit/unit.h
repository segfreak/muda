#ifndef __UNIT_IMP_H
#define __UNIT_IMP_H

typedef void (*unit_test_fn) (void);

void ut_register (unit_test_fn fn, const char *name);
int  ut_run_all (void);
void ut_basic_assert (int cond, const char *cond_str, const char *msg);
#define ut_assert(cond, msg) ut_basic_assert (cond, #cond, msg)

#endif
