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
	if (recid==0) {
		int len = atoi(FIELDS[idx]->params[0]);
		if (len>0) {
			char *head = FIELDS[idx]->params[1];
			FIELDS[idx]->ret = malloc((len+1)*sizeof(char));
			sprintf(FIELDS[idx]->ret, "%s%0*d\0", head, len-strlen(head), 0);
		} else
			FIELDS[idx]->ret = malloc((atoi(getvalue("fldlen"))+1)*sizeof(char));
	}
	
	if (strcmp(FIELDS[idx]->params[0],"0")==0)
		sprintf(FIELDS[idx]->ret, "%d\0", recid);
	else
		sprintf(FIELDS[idx]->ret+atoi(FIELDS[idx]->params[0])-fndchrcnt(recid), "%d\0", recid);

	return FIELDS[idx]->ret;
}

char *file(int idx, int recid) {
	static int filelen = 0;

	if (recid==0) {
		FIELDS[idx]->fp = fopen(FIELDS[idx]->params[0], "r");
		filelen = atoi(getvalue("filelen"));
		FIELDS[idx]->ret = malloc((filelen+1)*sizeof(char));
	} else {
		fgets(FIELDS[idx]->ret, filelen, FIELDS[idx]->fp);
		if ((recid>1&&(recid-1)%atoi(FIELDS[idx]->params[2])==0)||(FIELDS[idx]->ret[0]=='\n')) {
			rewind(FIELDS[idx]->fp);
			fgets(FIELDS[idx]->ret, filelen, FIELDS[idx]->fp);
		}
		if (atoi(FIELDS[idx]->params[1])>1) {
			for (int i=0;i<atoi(FIELDS[idx]->params[1])-1;i++)
				if (i==0)
					strtok(FIELDS[idx]->ret, ",");
				else
					strtok(NULL, ",\n");
			sprintf(FIELDS[idx]->ret, "%s\0", strtok(NULL, ",\n"));
		} else
			sprintf(FIELDS[idx]->ret, "%s\0", strtok(FIELDS[idx]->ret, ",\n"));
	}

	if (strcmp(FIELDS[idx]->ret, getvalue("nullsign"))==0)
		return "";
	else
    	return FIELDS[idx]->ret;
}

char *array(int idx, int recid) {
	char *ret = FIELDS[idx]->params[(recid-1)%atoi(FIELDS[idx]->params[0])+1];

	if (strcmp(ret, getvalue("nullsign"))==0)
		return "";
	else
    	return ret;
}

