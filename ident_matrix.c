#include "utils.h"
#include "kvec.h"

int ident_matrix_main(int argc, char *argv[]){

    if ( optind == argc || argc != optind + 1) {
        fprintf(stderr, "\nUsage: msa-tk ident_matrix <msa> \n\n");
        return 1;
    }

    kvec_t( const char* ) name_t;
    kvec_t( khint_t ) vs;
    kv_init(name_t);
    kv_init(vs);

    khash_t( reg ) *h;
    h   = kh_init(reg);

    khint_t k;
    int i, j, ret;

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
                kv_push(const char*, name_t, kh_key(h, k));

            }else{
                fprintf(stderr, "[ERR]: can't insert key/val: %s/%s\n", kseq->name.s, kseq->seq.s);
                exit(-1);
            }
        }
    
    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind ] );
        exit(1);
    }

    if(kv_size(name_t) == 0) exit(-1);

    for (i = 0; i < kv_size(name_t) - 1; ++i){
        k = kh_get(reg, h, kv_A(name_t, i));
        if(k == kh_end(h)){
            fprintf(stderr, "[ERR]: can't locate key:%s\n", kv_A(name_t, i) );
            exit(-1);
        }
        kv_push(khint_t, vs, k);
    }

    int len = strlen(kh_val(h, kv_A(vs, 0)));

    int num = kv_size(vs);
    float *mx = (float *) calloc ( num * num, sizeof(float));

    for (i = 0; i < kv_size(vs); ++i){
        for (j = i; j < kv_size(vs); ++j){
            float val = identity( kh_val(h, kv_A(vs, i)), kh_val(h, kv_A(vs, j)), len );
            mx[ i * num + j] = val;
            mx[ j * num + i] = val;
        }
    }

    printf("identity");
    for (i = 0; i < num; ++i) printf("\t%s", kh_key(h, kv_A(vs, i)));
    printf("\n");
    
    for (i = 0; i < num; ++i){
        printf("%s", kh_key(h, kv_A(vs, i)));
        for (j = 0; j < num; ++j) printf("\t%f", mx[ i * num + j]);
        printf("\n");
    }

    free(mx);
    kv_destroy(name_t);
    kv_destroy(vs);    
    kh_reg_destroy(h);

    return 0;
}