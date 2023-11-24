#include "utils.h"
#include "kvec.h"
#include "ksort.h"

kvec_t( const char* ) name_t;
kvec_t( int )    index_t;
kvec_t( float ) identity_t;

#define ident_lt(a, b) ( kv_A(identity_t, (a) ) > kv_A(identity_t, (b)) )
KSORT_INIT(int, int, ident_lt)

int neighbor_main(int argc, char *argv[]){
    
    int    num      = 10;
    float  id       = 0.3;

    int c;
    while ((c = getopt(argc, argv, "n:i:")) >= 0) {
        if(c == 'n')      num = atoi(optarg);
        else if(c == 'i') id  = atof(optarg);
    }
    
    if ( optind == argc || argc != optind + 2) {
        fprintf(stderr, "\nUsage: msa-tk neighbor <msa> <list> \n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -n INT     top %d nearest neighbor: [10];\n", num);
        fprintf(stderr, "  -i FLOAT   minimum identity percentage requirement: [0.3];\n\n");
        return 1;
    }


    kv_init(name_t);
    kv_init(index_t);
    kv_init(identity_t);

    khash_t( reg ) *h;
    khash_t( set ) *set;
    h   = kh_init(reg);
    set = kh_init(set);

    khint_t k, p;
    int ret;
    int pos = 0;

    gzFile fp;
    fp = strcmp(argv[ optind ], "-")? gzopen(argv[ optind ], "r") : gzdopen(fileno(stdin), "r");
    if( fp ){

        kseq_t *kseq;
        kseq = kseq_init(fp);
        int l;
        while ((l  = kseq_read(kseq)) >= 0){

            k = kh_put(reg, h, kseq->name.s, &ret);
            if(ret){
                
                kh_key(h, k)   = strdup( kseq->name.s );
                kh_val(h, k)   = strdup( kseq->seq.s );
            
            }else{
                fprintf(stderr, "[ERR]: can't insert key/val: %s/%s\n", kseq->name.s, kseq->seq.s);
                exit(-1);
            }
        }
    
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind ] );
        exit(1);
    }

    for (k = 0; k < kh_end(h); ++k) {
        if (kh_exist(h, k)) {
            kv_push(const char*, name_t, kh_key(h, k));
            kv_push(int, index_t, pos++);
            kv_push(float, identity_t, 0.0);
        }
    }

    fp = strcmp(argv[ optind + 1 ], "-")? gzopen(argv[ optind + 1 ], "r") : gzdopen(fileno(stdin), "r"); 

    kstream_t *ks;
    kstring_t kt   = {0, 0, 0};
    float ident   = 0.0;
    int len        = 0;
    int i;

    if(fp){
        
        ks  = ks_init(fp);
        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){

            p = kh_get(reg, h, kt.s);
            if(p == kh_end(h)){
                fprintf(stderr, "[ERR]: can't locate key: %s", kt.s);
                exit(-1);
            }

            len = strlen( (char *)kh_val(h, p) );
    
            for (pos = 0, k = 0; k < kh_end(h); ++k) {
                if (kh_exist(h, k)) {
                    ident = identity((char *)kh_val(h, p), (char *)kh_val(h, k), len);
                    kv_A(identity_t, pos++) = ident;
                }
            }

            ks_mergesort(int, kv_size(identity_t), index_t.a, 0);

            for (i = 0; i < kv_size(identity_t); ++i){

                if( i > num || kv_A(identity_t, kv_A(index_t, i)) < id ) break;

                k = kh_put(set, set, kv_A(name_t, kv_A(index_t, i)), &ret);
                if(ret) kh_key(set, k)   = strdup( kv_A(name_t, kv_A(index_t, i)) );
          
            }

            for (i = 0; i < kv_size(identity_t); ++i) kv_A(index_t, i) = i;

        }
        ks_destroy(ks);
        gzclose(fp);
    
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[optind + 1]);
        exit(-1);
    }

    for (k = 0; k < kh_end(set); ++k) {
        if (kh_exist(set, k)){
            p = kh_get(reg, h, kh_key(set, k));
            if(p != kh_end(h)) printf(">%s\n%s\n", kh_key(h, p), kh_val(h, p));
            else{
                fprintf(stderr, "[ERR]: can't locate key: %s", kh_key(set, k));
                exit(-1);
            }        
        }
    }

    kv_destroy(name_t);
    kv_destroy(identity_t);
    kv_destroy(index_t);
    
    free(kt.s);
    kh_reg_destroy(h);
    kh_set_destroy(set);

    return 0;
}