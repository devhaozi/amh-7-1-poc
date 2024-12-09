/************************************************
* AMH 7.1
* amh.sh 
* @param amh shell系统入口
* @copyright AMH & amh.sh
* @license APL (http://amh.sh/apl.htm) V1.0
* Update:2023-11-11
* 
************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
	int i, k, r = 0;
	FILE * fp;
	uid_t amh_uid, amh_euid;
	amh_uid = getuid();
	char amh_uid_str[10] = {0};
	char amh_cmd_all[2048] = "export amh_uid=";
	char amh_cmd[2048] = {0};
	struct passwd *amh_user = getpwuid(amh_uid);
	char amh_www[22] = "/usr/local/amh-7.1/web";
	char amh_pwd[30];
	getcwd(amh_pwd, 30);
	if(!(amh_uid == 0 || (strcmp(amh_user->pw_name, "www") == 0 && strcmp(amh_pwd, amh_www) == 0)))
		return 1;

	for (k = 1; k < argc; k++)
	{
		i = 0;
		char cmd_row[2048];
		strcpy(cmd_row, argv[k]);
		int cmd_row_len = (int)strlen(cmd_row);
		for (; i < cmd_row_len; i++)
			if (cmd_row[i] == ' ' || cmd_row[i] == '`' || cmd_row[i] == ';' || cmd_row[i] == '&' || cmd_row[i] == '|' || cmd_row[i] == '(') cmd_row[i] = '_';
		strcpy(argv[k], cmd_row);
	}

	strcat(amh_cmd, "/root/amh/conf/amh-base ");
	for (k = 1; k < argc; k++)
	{
		strcat(amh_cmd, argv[k]);
		strcat(amh_cmd, " ");
	}

	amh_euid = geteuid();
	sprintf(amh_uid_str, "%d", amh_uid);
	strcat(amh_cmd_all, amh_uid_str);
	strcat(amh_cmd_all, " && ");
	strcat(amh_cmd, "; echo $? >/tmp/amh.result");
	strcat(amh_cmd_all, amh_cmd);
	setreuid(amh_euid, amh_euid);
	system(amh_cmd_all);
	fp = fopen("/tmp/amh.result", "r");
	if (fp)
	{
		fscanf(fp, "%d", &r);
		fclose(fp);
	}
	return r;
}
