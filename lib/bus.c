#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "model.h"
#include "bus.h"

void init(int argc, char *argv[]) {
	INI = readini("config.ini");
	parseparams(argc, argv);

	int cfglen = atoi(getvalue("cfglen")), fldcnt = 0, prmcnt = 0;
	char line[cfglen], *token, *tmp;

	FILE *fpcfg = fopen(getvalue("cfgpath"), "r");
	while (!feof(fpcfg)) {
		fgets(line, cfglen, fpcfg);
		if (line[0]!='\n')
			fldcnt++;
	}
	rewind(fpcfg);

	tmp = malloc((fndchrcnt(fldcnt)+1)*sizeof(char));
	sprintf(tmp, "%d\0", fldcnt);
	setvalue("fldcnt", tmp);

	FIELDS = malloc(fldcnt*sizeof(field_t *));
	for (int i=0;i<fldcnt;i++) {
		fgets(line, cfglen, fpcfg);
		if (line[0]=='\n')
			break;
		line[strlen(line)-1] = '\0';
		prmcnt = fndsptcnt(line, ',') + 1;

		strtok(line, ":");
		FIELDS[i] = malloc(sizeof(field_t));
		FIELDS[i]->mthd = getmthd(strtok(NULL, "|"));
		FIELDS[i]->params = malloc(prmcnt*sizeof(char *));
		for (int j=0;j<prmcnt;j++) {
			token = strtok(NULL, ",");
			FIELDS[i]->params[j] = malloc((strlen(token)+1)*sizeof(char));
			sprintf(FIELDS[i]->params[j], "%s\0", token);
		}
		FIELDS[i]->mthd(i, 0);
	}
}

void parseparams(int argc, char *argv[]) {
	char *key, *value;

	for (int i=1;i<argc;i++) {
		key = strtok(argv[i]+1, "=");
		value = strtok(NULL, "=");
		setvalue(key, value);
	}
}

void execute() {
	int fldcnt = atoi(getvalue("fldcnt")), reccnt = atoi(getvalue("reccnt"));
	char *separator = getvalue("separator"), record[atoi(getvalue("reclen"))];
	FILE *fp = fopen(getvalue("outname"), "w");

	for (int i=1;i<=reccnt;i++) {
		memset(record, '\0', sizeof(record));
		for (int j=0;j<fldcnt;j++) {
			if (j>0)
				strcat(record, separator);
			strcat(record, FIELDS[j]->mthd(j, i));
		}
		fprintf(fp, "%s\n", record);
	}
}

