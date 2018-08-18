#define PCRE2_CODE_UNIT_WIDTH 8

#include "pcre2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: [%s] [input path]\n", argv[0]);
		return 0;
	}
	pcre2_code *re = NULL;
	pcre2_match_data *match_data = NULL;
	PCRE2_SIZE erroffset, *ovector;
	int errorcode;
	int rc;
	int num;
	int cnt;
	char strTemp[255];

	FILE *fp = NULL;
	if (fopen_s(&fp, argv[1], "r") != 0)
	{
		printf("no file found\n");
		return 0;
	}


	for (cnt = 0; cnt < 3; cnt++) {

		PCRE2_SPTR pattern = (PCRE2_SPTR)"[0][2][-]\\d{3,4}[-]\\d{4}|[0][1][0|6|9][-]\\d{3,4}[-]\\d{4}|[0][3][1][-]\\d{3,4}[-]\\d{4}"; // write regular expression here
		switch (cnt)
		{
		case 0:
			pattern = (PCRE2_SPTR)"[0][2][-]\\d{3,4}[-]\\d{4}\n|[0][1][0|6|9][-]\\d{3,4}[-]\\d{4}\n|[0][3][1][-]\\d{3,4}[-]\\d{4}\n";
			break;
		case 1:
			pattern = (PCRE2_SPTR)"[\\w|\\W]*[[]\\d{1,2}[/]Apr[/]2018[]]\\s*[\"]GET\\s*[/]admin[\\w|\\W]*";
			break;
		case 2:
			pattern = (PCRE2_SPTR)"[424D][\\d|A-F]{32}5000000046000000[\\d|A-F]{44}";
			break;
		}

		fscanf_s(fp, "%d", &num);
		fgetc(fp);

		while (num--)
		{
			fgets(strTemp, sizeof(strTemp), fp);
			PCRE2_SPTR input = (PCRE2_SPTR)strTemp;

			re = pcre2_compile(pattern, -1, 0, &errorcode, &erroffset, NULL);

			if (re == NULL)
			{
				PCRE2_UCHAR8 buffer[120];
				(void)pcre2_get_error_message(errorcode, buffer, 120);
				/* Handle error */
				return 0;
			}

			match_data = pcre2_match_data_create(20, NULL);
			rc = pcre2_match(re, input, -1, 0, 0, match_data, NULL);

			if (rc <= 0)
				printf("N\n");
			else
			{
				printf("Y\n");
			}
		}
	}


	fclose(fp);

	pcre2_match_data_free(match_data);
	pcre2_code_free(re);

	return 0;

}