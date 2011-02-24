/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef XSPACE_XTRACOR_H_
#define XSPACE_XTRACOR_H_

#include <stdarg.h>
#include <syslog.h>

namespace xspace {
extern void xtracor_init(const char *ident);
extern void xtracor(int priority, const char *format, ...);
extern void xtracor_fini(void);
}

#endif  // XSPACE_XTRACOR_H_
