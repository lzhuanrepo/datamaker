typedef char *(*mthd_t)(int, int);
typedef struct field {
    char **params;
	char *ret;
	FILE *fp;
    mthd_t mthd;
} field_t;

extern field_t **FIELDS;

mthd_t getmthd(char *);
char *id(int, int);
char *file(int, int);
char *array(int, int);
void freefldparams(char **, char *);

const static struct name_mthd {
    const char *name;
    mthd_t mthd;
} name_mthds[] = {
    {"id", id},
    {"file", file},
    {"array", array}
};

