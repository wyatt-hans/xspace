/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#include <xtracor.h>
#include <stdio.h>

namespace xspace {
void xtracor_init(const char *ident) {
    openlog(ident, LOG_CONS|LOG_PID, LOG_USER);
}

void xtracor(int priority, const char *format, ...) {
    va_list ap;
    
    va_start(ap, format);
    vsyslog(priority, format, ap);

    va_start(ap, format);
    vprintf(format, ap);
}

void xtracor_fini(void) {
    closelog();
}
}
