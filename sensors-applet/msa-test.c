#include "msa-test.h"

#include <syslog.h>

#define MSA_TEST_C

void printmsamsg (void) {
syslog(LOG_ERR, "msa-test.c");
}
