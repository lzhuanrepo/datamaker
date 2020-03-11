typedef struct key_value {
    char *key;
    char *value;
    struct key_value *next;
} key_value_t;

extern key_value_t *INI;

void readini();
void readini2();
void readfile(char *, FILE **, int, int, int, int);
void cpystr(char **, char *, int);
void printstrarr(char **, int);
void parseline(char *, int iscmdarg);
void printvalues();
void addvalue(char *, char *);
void setvalue(char *, char *);
char *getvalue(char *);
char *padding(char *, int, char);
int fndlncnt(FILE *);
int fndsptcnt(char *, char *);
int fndchrcnt(int);

