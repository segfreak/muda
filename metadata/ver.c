#include "ver.h"
#include "../include/muda/muda.h"

const char *
muda_version_string ()
{
  return MUDA_VERSION;
}

unsigned
muda_version_major ()
{
  return MUDA_VERSION_MAJOR;
}

unsigned
muda_version_minor ()
{
  return MUDA_VERSION_MINOR;
}

unsigned
muda_version_patch ()
{
  return MUDA_VERSION_PATCH;
}

muda_version_t
muda_version ()
{
  muda_version_t ver;
  ver.major = MUDA_VERSION_MAJOR;
  ver.major = MUDA_VERSION_MINOR;
  ver.patch = MUDA_VERSION_PATCH;
  return ver;
}