#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#define INILEN 102400

key_value_t *INI = NULL;

void readini2(char *fnini)
{
	char *line = malloc(1024*sizeof(char));
	int inilen = 0;

	FILE *fpini = fopen(fnini, "r");
	while (!feof(fpini)) {
		if (inilen==0)
			fgets(line, 1024, fpini);
		else
			fgets(line, inilen+1, fpini);
		line[strlen(line)-1] = '\0';
		if (line[0]>='a'&&line[0]<='z') {
			if (inilen==0) {
				parseline(line, 0);
				inilen = atoi(getvalue("inilen"));
				free(line);
				line = malloc((inilen+1)*sizeof(char));
			} else {
				parseline(line, 0);
			}
		}
	}
}

void readini(char *fnini)
{
    char line[INILEN];

    FILE *fpini = fopen(fnini, "r");
    while (!feof(fpini)) {
        fgets(line, INILEN, fpini);
        line[strlen(line)-1] = '\0';
        if (line[0]>='a' && line[0]<='z')
            parseline(line, 0);
    }
}

void readfile(char *ret, FILE **fp, int id, int start, int count, int pos) {
    int filelen = atoi(getvalue("filelen")), index = (id-1)%count;
    fgets(ret, filelen, *fp);

	if (index==0) {
        rewind(*fp);
        for (int i=0;i<start-1;i++)
            fgets(ret, filelen, *fp);
        fgets(ret, filelen, *fp);
    }
    for (int i=0;i<pos-1;i++) {
        if (i==0)
            strtok(ret, ",");
        else
            strtok(NULL, ",");
    }
	if (pos>1)
        sprintf(ret, "%s\0", strtok(NULL, ",\n"));
    else
        sprintf(ret, "%s\0", strtok(ret, ",\n"));
}

void printstrarr(char **arr, int size) {
	printf("%s\nString array:", "----------");
	for (int i=0;i<size;i++) {
		if (i>0)
			printf("%s", ",");
		printf("%s", arr[i]);
	}
	printf("\n%s\n", "----------");
}

void cpystr(char **dst, char *src, int size) {
	*dst = malloc((size+1)*sizeof(char));
	memcpy(*dst, src, size);
	(*dst)[size] = '\0';
}

void parseline(char *line, int iscmdarg) {
	char *tmp, *key, *value;
	int i = 0;

	for (;i<strlen(line);i++)
		if (line[i]=='=')
			break;
	cpystr(&key, line, i);
	value = line + i + 1;

	if (!iscmdarg) {
		if (!getvalue(key))
			addvalue(key, value);
		else
			setvalue(key, value);
	} else {
		if (!getvalue(key+1))
            addvalue(key+1, value);
        else
            setvalue(key+1, value);
	}

	free(key);
}

void printvalues() {
	key_value_t *it = INI;

	printf("%s\n", "Key value pairs:\n----------");
	while (it!=NULL) {
		printf("key:%s,value:%s\n", it->key, it->value);
		it = it->next;
	}
	printf("%s\n", "----------");
}

void addvalue(char *key, char *value) {
	key_value_t *it = INI;
	key_value_t **tmp;

	if (it!=NULL) {
		while (it->next!=NULL)
			it = it->next;
		tmp = &(it->next);
	} else
		tmp = &INI;
	
	*tmp = malloc(sizeof(key_value_t));
    cpystr(&((*tmp)->key), key, strlen(key));
    cpystr(&((*tmp)->value), value, strlen(value));
}

void setvalue(char *key, char *value) {
	key_value_t *it = INI;

	while (it!=NULL) {
		if (strcmp(it->key, key)==0) {
			free(it->value);
			cpystr(&(it->value), value, strlen(value));
			return;
		}
		it = it->next;
	}
}

char *getvalue(char *key) {
    key_value_t *it = INI;

    while (it!=NULL) {
        if (strcmp(key, it->key)==0)
            return it->value;
        it = it->next;
    }

    return NULL;
}

char *padding(char *input, int size, char padchr) {
	char *ret;
	int len = strlen(input);

	if (size>0)
		ret = malloc((size+1)*sizeof(char));
	else if (size<0)
		ret = malloc(((-1)*size+1)*sizeof(char));
	else
		ret = malloc((len+1)*sizeof(char));

	if (size>0) {
		for (int i=0;i<size-len;i++)
			ret[i] = padchr;
		sprintf(ret+size-len, "%s\0", input);
	} else if (size<0) {
		sprintf(ret, "%s", input);
		for (int i=len;i<(-1)*size;i++)
            ret[i] = padchr;
		ret[(-1)*size] = '\0';
	} else
		sprintf(ret, "%s\0", input);

	return ret;
}

int fndlncnt(FILE *fp) {
	int ret = 0;

	while (!feof(fp))
		if (fgetc(fp) == '\n')
			ret++;

	return ret-1;
}

int fndchrcnt(int n) {
	int cnt = 0;
	
	if (n==0)
		return 1;
	else {
		while (n!=0) {
			n /= 10;
			cnt++;
		}
		return cnt;
	}
}

int fndsptcnt(char *str, char *srh) {
	int cnt = 0;

	for (int i=0;i<strlen(str);i++)
		for (int j=0;j<strlen(srh);j++) {
			if (str[i]==srh[j]) {
				cnt++;
				break;
			}
		}

	return cnt;
}

