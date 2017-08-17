/**
 * COPYRIGHT 2015 Flirc, Inc. All rights reserved.
 *
 * This copyright notice is Copyright Management Information under 17 USC 1202
 * and is included to protect this work and deter copyright infringement.
 * Removal or alteration of this Copyright Management Information without
 * the express written permission of Flirc, Inc. is prohibited, and any
 * such unauthorized removal or alteration will be a violation of federal law.
 *
 * @file
 * @brief   Flirc Driver non-public internal functions
 */

#ifndef I__FL_VERS_HISTORY_H__
#define I__FL_VERS_HISTORY_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cpluspuls */

#include <semver.h>

struct list_head *fl_get_ver_hist_v2(semver_t dev, semver_t comp);
struct list_head *fl_get_ver_hist_v1(semver_t dev, semver_t comp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* I__FL_VERS_HISTORY_H__ */
