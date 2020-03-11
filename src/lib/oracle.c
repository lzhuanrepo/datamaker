#include <stdio.h>
#include "ocilib.h"
#include "helper.h"

int oraclequery() {
	char *ip, ips[1024];
    char cnstr[32], fname[128];
	int size;
    FILE *fp;
    OCI_Connection *cn;
    OCI_Statement *st;
    OCI_Resultset *rs;

    sprintf(ips, "%s\0", getvalue("ips"));
	size = fndsptcnt(ips, ",") + 1;
    for (int i=0;i<size;i++) {
        if (i==0)
            ip = strtok(ips, ",\n");
        else
            ip = strtok(NULL, ",\n");
        sprintf(fname, "%s_%s\0", getvalue("oracleoutname"), ip);
        sprintf(cnstr, "%s:%s/%s\0", ip, getvalue("port"), getvalue("db"));
        fp = fopen(fname, "w");
        OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT);
        cn = OCI_ConnectionCreate(cnstr, getvalue("name"), getvalue("pwd"), OCI_SESSION_DEFAULT);
        st = OCI_StatementCreate(cn);
        OCI_ExecuteStmt(st, getvalue("sql"));
        rs = OCI_GetResultset(st);
        while (OCI_FetchNext(rs)) {
            for (int i=1;i<=atoi(getvalue("oraclefldcnt"));i++) {
                if (i>1)
                    fprintf(fp, "%s", ",");
                fprintf(fp, "%s", OCI_GetString(rs, i));
            }
            fprintf(fp, "%s", "\n");
        }
    }

    OCI_ConnectionFree(cn);
    OCI_Cleanup();
    fclose(fp);
    return EXIT_SUCCESS;
}
