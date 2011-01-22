#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "getopts.h"

GetOpts::GetOpts(struct options opts[], bool &help, int optslen, int argc, char *argv[])
{
    int i,j;
	help = false;
    if(argc == 1)
        return;
    for(i=1; i<argc; i++)
    {
	if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            usage(argv[0], optslen, opts);
			help = true;
            return;
        }
    }

    for(i=1; i<argc; i++)
    {
        for(j=0;j<optslen;j++)
        {
            if( (strstr(argv[i], "--") == argv[i] && strstr(argv[i], opts[j].name) == argv[i]+2)
				|| (strstr(argv[i], "--") == NULL && strstr(argv[i], opts[j].sname) == argv[i]+1) )
            {
                if(opts[j].value > 0)
                {
                    i++;
                    *opts[j].vartoset = new char[strlen(argv[i])];
                    strcpy(*opts[j].vartoset, argv[i]);
                }
                else
                {
                    *opts[j].vartoset = new char[1];
                    strcpy(*opts[j].vartoset, "1\0");
                }
                continue;
            }
        }
    }
}

void GetOpts::usage(char *progn, int optslen, struct options opts[])
{
    printf("Usage: %s [options]\n\n", progn);
    printf("  --help,\t-h\t\t\tDisplays this message\n");
    for(int i = 0; i<optslen; i++)
    {
        //char *line = new char[strlen(opts[i].name)+strlen(opts[i].sname)+10];
		char *line = new char[1024];
		if(opts[i].value > 0) {
			sprintf(line, "  --%s,\t-%s <args>\t\t", opts[i].name, opts[i].sname);
		} else { 
			sprintf(line, "  --%s,\t-%s \t\t\t", opts[i].name, opts[i].sname);
		}
		printf("%s%s\n", line, opts[i].descp);
    }
}
