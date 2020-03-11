#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#define INILEN 1024

key_value_t *INI;

key_value_t *readini(char *fnini)
{
	char *key, *value, *token, separator[2];
	char line[INILEN];
	key_value_t *it = NULL, *ini;
	int init = 1;

	FILE *fpini = fopen(fnini, "r");
	fgets(line, INILEN, fpini);
	while (!feof(fpini)) {
		fgets(line, INILEN, fpini);
		line[strlen(line)-1] = '\0';

		if (init==1) {
			sprintf(separator, "%c\0", line[10]);
			init = 0;
			continue;
		}

		if (line[0]>='a' && line[0]<='z') {
			token = strtok(line, separator);
            key = malloc((strlen(token)+1)*sizeof(char));
            sprintf(key, "%s\0", token);
            token = strtok(NULL, separator);
            value = malloc((strlen(token)+1)*sizeof(char));
            sprintf(value, "%s\0", token);
			if (it==NULL) {
				createkeyvalue(&it, key, value);
				ini = it;
			} else {
				createkeyvalue(&(it->next), key, value);
				it = it->next;
			}
		}
	}
	return ini;
}

char *getvalue(char *key) {
	key_value_t *it = INI;
	while (it!=NULL) {
		if (strcmp(key, it->key)==0)
			return it->value;
		it = it->next;
	}
}

void createkeyvalue(key_value_t **it, char *key, char *value) {
	*it = malloc(sizeof(key_value_t));
	(*it)->key = key;
	(*it)->value = value;
}

void setvalue(char *key, char *value) {
	key_value_t *it = INI;
	while(1) {
		if (strcmp(it->key, key)==0) {
			free(it->value);
			it->value = malloc((strlen(value)+1)*sizeof(char));
			sprintf(it->value, "%s\0", value);
			return;
		}
		if (it->next==NULL)
			break;
		it = it->next;
	}

	it->next = malloc(sizeof(key_value_t));
	it->next->key = malloc((strlen(key)+1)*sizeof(char));
	sprintf(it->next->key, "%s\0", key);
	it->next->value = malloc((strlen(value)+1)*sizeof(char));
	sprintf(it->next->value, "%s\0", value);
}

int fndchrcnt(int n) {
	int cnt = 0;
	
	if (n==0)
		return 1;
	while (n!=0) {
		n /= 10;
		cnt++;
	}
	return cnt;
}

int fndsptcnt(char *str, char srh) {
	int cnt = 0;
	for (int i=0;i<strlen(str);i++)
		if (str[i]==srh)
			cnt++;
	return cnt;
}

