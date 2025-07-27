#ifndef MUDA_MUDA_H
#define MUDA_MUDA_H

const char *muda_git_commit ();
const char *muda_git_branch ();
int         muda_git_dirty ();
const char *muda_build_time ();

typedef struct muda_version
{
  unsigned major;
  unsigned minor;
  unsigned patch;
} muda_version_t;

const char *muda_ver_string ();
unsigned    muda_ver_major ();
unsigned    muda_ver_minor ();
unsigned    muda_ver_patch ();

#endif