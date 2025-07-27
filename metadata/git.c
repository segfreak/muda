#include "git.h"
#include "../include/muda/muda.h"

const char *
muda_git_commit ()
{
  return MUDA_GIT_COMMIT;
}

const char *
muda_git_branch ()
{
  return MUDA_GIT_BRANCH;
}

int
muda_git_dirty ()
{
  return MUDA_GIT_DIRTY;
}

const char *
muda_build_time ()
{
  return MUDA_BUILD_TIME;
}
