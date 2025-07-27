#ifndef MUDA_MUDA_H
#define MUDA_MUDA_H

const char *muda_git_commit ();
const char *muda_git_branch ();
int         muda_git_dirty ();
const char *muda_build_time ();

#endif