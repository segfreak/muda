#ifndef MUDA_MUDA_H
#define MUDA_MUDA_H

/**
 * @file muda.h
 * @brief MUDA library version and build metadata interface.
 */

/**
 * @brief Get the current Git commit hash (short form).
 *
 * @return A null-terminated string with the commit hash.
 */
const char *muda_git_commit ();

/**
 * @brief Get the current Git branch name.
 *
 * @return A null-terminated string with the branch name.
 */
const char *muda_git_branch ();

/**
 * @brief Check if the working directory has uncommitted changes.
 *
 * @return Non-zero if there are changes; 0 otherwise.
 */
int         muda_git_dirty ();

/**
 * @brief Get the timestamp when the project was built.
 *
 * @return A null-terminated string representing build time.
 */
const char *muda_build_time ();

/**
 * @struct muda_version
 * @brief Semantic version of the MUDA library.
 */
typedef struct muda_version
{
  unsigned maj; /**< Major version number. */
  unsigned min; /**< Minor version number. */
  unsigned pat; /**< Patch version number. */
} muda_version_t;

/**
 * @brief Get the full version string (e.g., "1.2.3").
 *
 * @return A null-terminated string representing the version.
 */
const char    *muda_ver_str ();

/**
 * @brief Get the major version number.
 *
 * @return Major version (e.g., 1 in 1.2.3).
 */
unsigned       muda_ver_maj ();

/**
 * @brief Get the minor version number.
 *
 * @return Minor version (e.g., 2 in 1.2.3).
 */
unsigned       muda_ver_min ();

/**
 * @brief Get the patch version number.
 *
 * @return Patch version (e.g., 3 in 1.2.3).
 */
unsigned       muda_ver_pat ();

/**
 * @brief Get the full structured version object.
 *
 * @return A muda_version_t struct with major, minor, and patch values.
 */
muda_version_t muda_ver ();

#endif /* MUDA_MUDA_H */
