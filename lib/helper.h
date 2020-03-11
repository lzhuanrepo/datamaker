typedef struct key_value {
	char *key;
	char *value;
	struct key_value *next;
} key_value_t;

extern key_value_t *INI;

key_value_t *readini();
char *getvalue(char *);
void createkeyvalue(key_value_t **, char *, char *);
void setvalue(char *, char *);
int fndsptcnt(char *, char);
int fndchrcnt(int);

