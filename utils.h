#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <stdlib.h>

#include "kstring.h"
#include "kseq.h"
#include "khash.h"


KSEQ_INIT(gzFile, gzread)

KHASH_MAP_INIT_STR(reg, char *)
void kh_reg_destroy(khash_t(reg) *h);

KHASH_MAP_INIT_STR(kreg, kstring_t)
void kh_kreg_destroy(khash_t(kreg) *h);

KHASH_SET_INIT_STR(set)
void kh_set_destroy(khash_t(set) *h);

float identity(char * p, char *q, int l);