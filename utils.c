#include "utils.h"

float identity(char * p, char *q, int l){
    
    int i, match, len;
    for (len=l,  match = 0, i = 0; i < l ; ++i){
        if(p[i] == '-' && q[i] == '-'){
            --len;
            continue;
        }
        if( p[i] == q[i] ) ++match;
    }
    return 1.0 * match/len;

}

void kh_reg_destroy(khash_t(reg) *h){
    
    khint_t k;
    if  (h == 0) return;
    for (k = 0; k < kh_end(h); ++k) {
        if (kh_exist(h, k)) {
            free((char*)kh_key(h, k));
            free((char*)kh_val(h, k));
        }
    }
    kh_destroy(reg, h);

}

void kh_kreg_destroy(khash_t(kreg) *h){
    
    khint_t k;
    if  (h == 0) return;
    for (k = 0; k < kh_end(h); ++k) {
        if (kh_exist(h, k)) {
            free((char*)kh_key(h, k));
            free((char*)kh_val(h, k).s);
        }
    }
    kh_destroy(kreg, h);

}

void kh_set_destroy(khash_t(set) *h){
    
    khint_t k;
    if  (h == 0) return;
    for (k = 0; k < kh_end(h); ++k) {
        if (kh_exist(h, k)) free((char*)kh_key(h, k));
    }
    kh_destroy(set, h);

}
