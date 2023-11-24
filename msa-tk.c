/*****************************************************************************

  msa-tk: command line interface.  

  (c) 2023-2023 - LEI ZHANG
  Logic Informatics Co.,Ltd.
  zhanglei@logicinformatics.com
  
  Licenced under The MIT License.
******************************************************************************/

#include <stdio.h>
#include <string.h>

int  ident_matrix_main(int argc, char *argv[]);
int  neighbor_main(int argc, char *argv[]);

static int usage(){
    
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage:  msa-tk <command> <arguments>\n");
    fprintf(stderr, "Version: 0.0.1\n\n");
    fprintf(stderr, "Command:\n");
    fprintf(stderr, "     neighbor       find the nearest neighbor in msa file using indetity percentage.\n");
    fprintf(stderr, "     ident_matrix   calculator identity matrix.\n");

    fprintf(stderr, "\n");

    fprintf(stderr, "\nLicenced:\n");
    fprintf(stderr, "(c) 2023-2023 - LEI ZHANG\n");
    fprintf(stderr, "Logic Informatics Co.,Ltd.\n");
    fprintf(stderr, "zhanglei@logicinformatics.com\n");
    fprintf(stderr, "\n");

    return 1;

}

int main(int argc, char *argv[]){

    if (argc < 2) return usage();
    
    if (strcmp(argv[1], "neighbor") == 0)    neighbor_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "ident_matrix") == 0)  ident_matrix_main(argc - 1, argv + 1);
    else {
        fprintf(stderr, "[main] unrecognized command '%s'. Abort!\n", argv[1]);
        return 1;
    }
    
    return 0;

}