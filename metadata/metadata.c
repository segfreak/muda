#include "../include/muda/muda.h"
#include "git.h"
#include "ver.h"

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

const char *
muda_ver_str ()
{
  return MUDA_VERSION;
}

unsigned
muda_ver_maj ()
{
  return MUDA_VERSION_MAJOR;
}

unsigned
muda_ver_min ()
{
  return MUDA_VERSION_MINOR;
}

unsigned
muda_ver_pat ()
{
  return MUDA_VERSION_PATCH;
}

muda_version_t
muda_ver ()
{
  muda_version_t ver;
  ver.maj = MUDA_VERSION_MAJOR;
  ver.min = MUDA_VERSION_MINOR;
  ver.pat = MUDA_VERSION_PATCH;
  return ver;
}