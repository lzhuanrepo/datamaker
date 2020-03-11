#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "model.h"

field_t **FIELDS;

mthd_t getmthd(char *name) {
    for (int i=0;i<sizeof(name_mthds)/sizeof(struct name_mthd);i++)
        if (strcmp(name_mthds[i].name, name)==0)
            return name_mthds[i].mthd;
}

char *id(int idx, int recid) {
	static int start;
	int len = atoi(FIELDS[idx]->params[2]);

	if (recid==0) {
		start = atoi(getvalue("start"));
		FIELDS[idx]->ret = malloc(atoi(getvalue("fldlen"))*sizeof(char));
		return NULL;
	} else {
		recid = recid + start - 1;
		if (strcmp(FIELDS[idx]->params[0], "NVL")!=0) {
			sprintf(FIELDS[idx]->ret, "%0*d\0", atoi(FIELDS[idx]->params[1]), 0);
			memcpy(FIELDS[idx]->ret, FIELDS[idx]->params[0], strlen(FIELDS[idx]->params[0]));
			sprintf(FIELDS[idx]->ret+atoi(FIELDS[idx]->params[1])-fndchrcnt(recid), "%d\0", recid);
		} else if (strcmp(FIELDS[idx]->params[0], "NVL")==0) {
			if (strcmp(FIELDS[idx]->params[1], "0")==0)
				sprintf(FIELDS[idx]->ret, "%d\0", recid);
			else {
				sprintf(FIELDS[idx]->ret, "%0*d\0", atoi(FIELDS[idx]->params[1]), 0);
            	sprintf(FIELDS[idx]->ret+atoi(FIELDS[idx]->params[1])-fndchrcnt(recid), "%d\0", recid);
			}
		}

		if (strcmp(FIELDS[idx]->params[2],"0")==0)
			return padding(FIELDS[idx]->ret, 0, '0');
		else
			return padding(FIELDS[idx]->ret, len, FIELDS[idx]->params[3][0]);
	}
}

char *file(int idx, int recid) {
	if (recid==0) {
		FIELDS[idx]->fp = fopen(FIELDS[idx]->params[0], "r");
		FILE *fp = FIELDS[idx]->fp;
		int filelen = atoi(getvalue("filelen"));
		FIELDS[idx]->ret = malloc((filelen+1)*sizeof(char));
		char *ret = FIELDS[idx]->ret;

		if (FIELDS[idx]->params[6][0]=='r') {
        	int count = atoi(FIELDS[idx]->params[5]);
        	int pos = atoi(FIELDS[idx]->params[4]);
        	char **params = malloc((count+4)*sizeof(char *));
        	char *token;

			for (int i=0;i<atoi(FIELDS[idx]->params[3])-1;i++)
				fgets(ret, filelen, fp);
			for (int i=0;i<count;i++) {
				fgets(ret, filelen, fp);
				for (int i=0;i<pos-1;i++)
        			if (i==0)
            			strtok(ret, ",");
        			else
            			strtok(NULL, ",");
				if (pos>1)
                    token = strtok(NULL, ",\n");
                else
                    token = strtok(ret, ",\n");
				cpystr(&params[i+3], token, strlen(token));
			}
			FIELDS[idx]->mthd = getmthd("array");
			cpystr(&params[0], FIELDS[idx]->params[1], strlen(FIELDS[idx]->params[1]));
			cpystr(&params[1], FIELDS[idx]->params[2], strlen(FIELDS[idx]->params[2]));
			params[2] = malloc((fndchrcnt(count)+1)*sizeof(char));
			sprintf(params[2], "%d\0", count);
			cpystr(&params[count+3], "r", 1);

			freefldparams(FIELDS[idx]->params, "file");
			FIELDS[idx]->params = params;
		}

		return NULL;
	} else {
		readfile(FIELDS[idx]->ret, &(FIELDS[idx]->fp), recid, atoi(FIELDS[idx]->params[3]), atoi(FIELDS[idx]->params[5]), atoi(FIELDS[idx]->params[4]));

		if (strcmp(FIELDS[idx]->ret, getvalue("nullsign"))==0)
	        return padding("", atoi(FIELDS[idx]->params[1]), FIELDS[idx]->params[2][0]);
    	else
        	return padding(FIELDS[idx]->ret, atoi(FIELDS[idx]->params[1]), FIELDS[idx]->params[2][0]);
	}
}

char *array(int idx, int recid) {
	if (recid==0)
		return NULL;
	else {
		if (FIELDS[idx]->params[atoi(FIELDS[idx]->params[2])+3][0]=='r')
			recid = rand();
		FIELDS[idx]->ret = FIELDS[idx]->params[(recid-1)%atoi(FIELDS[idx]->params[2])+3];

		if (strcmp(FIELDS[idx]->ret, getvalue("nullsign"))==0)
			return padding("", atoi(FIELDS[idx]->params[0]), FIELDS[idx]->params[1][0]);
		else
			return padding(FIELDS[idx]->ret, atoi(FIELDS[idx]->params[0]), FIELDS[idx]->params[1][0]);
	}
}

void freefldparams(char **params, char *type) {
	int count = 0;

	if (strcmp(type, "id")==0)
		count = 4;
	else if (strcmp(type, "file")==0)
		count = 7;
	else if (strcmp(type, "array")==0)
		count = atoi(params[2]) + 4;

	for (int i=0;i<count;i++)
		free(params[i]);
	free(params);
}

