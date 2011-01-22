#ifndef GETOPTS_H
#define GETOPTS_H

struct options
{
    char **vartoset;	//Address to the variable that will hold the value
    char *name;		//Full name of the parameter
    char *descp;	//Text description of the parameter
    char *sname;	//Short name of the parameter
    int value;		//1or0 0 if has no value (true/false) 1 if is going to have value
};

class GetOpts
{
    public:
        GetOpts(struct options [], bool&, int, int, char* []);
    private:
        void usage(char *, int, struct options []);
};

#endif	/*GETOPTS_H*/
