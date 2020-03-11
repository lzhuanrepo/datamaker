#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "model.h"
//#include "oracle.h"
#include "bus.h"

void unittest() {
	//char *tmp = padding("hello", 10, '0');
	//printf("'%s'\n", tmp);
	//free(tmp);

	FILE *fp = fopen("assetacc", "r");
	printf("line count:%d\n", fndlncnt(fp));
}

void init(int argc, char *argv[]) {
	readini("config.ini");
	parseparams(argc, argv);

	int cfglen = atoi(getvalue("cfglen")), fldcnt = 0, prmcnt = 0;
	char line[cfglen], *token, *tmp;

	FILE *fpcfg = fopen(getvalue("cfgpath"), "r");
	while (!feof(fpcfg)) {
		fgets(line, cfglen, fpcfg);
		if (line[0]!='\n'&&line[0]!='#')
			fldcnt++;
	}
	rewind(fpcfg);

	tmp = malloc((fndchrcnt(fldcnt)+1)*sizeof(char));
	sprintf(tmp, "%d\0", fldcnt);
	addvalue("fldcnt", tmp);
	free(tmp);

	FIELDS = malloc(fldcnt*sizeof(field_t *));
	for (int i=0;i<fldcnt;) {
		fgets(line, cfglen, fpcfg);
		if (line[0]=='\n'||line[0]=='#')
			continue;
		line[strlen(line)-1] = '\0';
		prmcnt = fndsptcnt(line, ",") + 1;

		strtok(line, ":");
		FIELDS[i] = malloc(sizeof(field_t));
        FIELDS[i]->mthd = getmthd(strtok(NULL, "|"));
        FIELDS[i]->params = malloc((prmcnt+1)*sizeof(char *));
        for (int j=0;j<prmcnt+1;j++) {
			if (j!=prmcnt) {
        		token = strtok(NULL, ",");
            	FIELDS[i]->params[j] = malloc((strlen(token)+1)*sizeof(char));
            	sprintf(FIELDS[i]->params[j], "%s\0", token);
			} else {
				FIELDS[i]->params[j] = malloc(2*sizeof(char));
                sprintf(FIELDS[i]->params[j], "%d\0", 0);
			}
        }
		FIELDS[i]->mthd(i, 0);
		i++;
	}
}

void parseparams(int argc, char *argv[]) {
	for (int i=1;i<argc;i++)
		parseline(argv[i], 1);
}

void execute() {
	if (strcmp(getvalue("action"), "output")==0) {	
		int fldcnt = atoi(getvalue("fldcnt")), reccnt = atoi(getvalue("reccnt"));
		char *separator = getvalue("separator"), record[atoi(getvalue("reclen"))];
		FILE *fp = fopen(getvalue("outname"), "w");

		for (int i=1;i<=reccnt;i++) {
			memset(record, '\0', sizeof(record));
			for (int j=0;j<fldcnt;j++) {
				if (j>0 && strcmp(separator, "NVL")!=0)
					strcat(record, separator);
				char *fld = FIELDS[j]->mthd(j, i);
				strcat(record, fld);
				free(fld);
			}
			fprintf(fp, "%s\n", record);
		}
	} else if (strcmp(getvalue("action"), "query")==0) {
		//oraclequery();
	}
}

