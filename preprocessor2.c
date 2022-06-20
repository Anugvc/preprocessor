
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct macro
{
	char m_name[30];
	char m_body[100];
};

void main(int argc, char **argv)
{
	int i;
	if (argc != 2)
	{
		printf("Enter input correctly\n");
		return;
	}

	FILE *fp = fopen(argv[1], "r");

	if (!fp)
	{
		printf("File not fund\n");
		return;
	}
	FILE *ftemp = fopen("temp.c", "w");
	///////////////////////////////////////////////////////////
	char ch, ch1, ch2, ch3;

	char s[100], s1[100];
	char *p, *q;

	while ((ch = fgetc(fp)) != -1) //removing comments and writing in to temporaary file
	{
		if (ch == '/')
		{
			ch1 = fgetc(fp);
			if (ch1 == '/')
				while ((ch = fgetc(fp)) != '\n')
					;
			else if (ch1 == '*')
			{

				while ((ch2 = fgetc(fp)) != -1)
				{
					if (ch2 == '*')
					{
						ch3 = fgetc(fp);
						if (ch3 == '/')
							break;
					}
				}
			}
			else
			{
				fputc(ch, ftemp);
				fputc(ch1, ftemp);
			}
		}
		else
		{
			fputc(ch, ftemp);
		}
	}

	printf("after comment removal\n");

	rewind(ftemp);
	ftemp = fopen("temp.c", "r");

	if (!ftemp)
		printf("file not found\n");

	////////////////////////////////////////////////////////////

	struct macro m[5];
	int macro_no = 0, count = 0;

	FILE *ftemp2 = fopen("temp2", "w");
	FILE *fpnew = fopen("file.i", "w");
	//ftemp = fopen("temp","r");

	while ((ch = fgetc(ftemp)) != -1)
	{
		if (ch == '#')
		{
			ch1 = fgetc(ftemp);
			i = 0;
			if (ch1 == ' ') //for removing any spaces between # and define
			{
				//printf("inside if\n");
				while ((ch2 = fgetc(ftemp)) == ' ')
					;
				//printf("aftwer space:'%c'\n", ch2);
				s[i++] = ch2;
			}
			else
			{
				s[i++] = ch1;
				//printf("Inside else ch=%d\n", ch1);
			}
			while ((ch1 = fgetc(ftemp)) != ' ')
			{
				s[i++] = ch1;
			}

			s[i] = '\0';
			printf("# === '%s'\n", s);
			printf("%d\n", strcmp(s, "define"));
			if (strcmp(s, "define") == 0)
			{
				//printf("Inside define\n");
				i = 0;
				while (ch1 == ' ') // removing whilte spaces
					ch1 = fgetc(ftemp);
				s[i++] = ch1;
				while ((ch1 = fgetc(ftemp)) != ' ')
				{
					s[i++] = ch1;
				}
				s[i] = '\0';
				//printf("macro name = '%s'\n", s);
				strcpy(m[macro_no].m_name, s);
				i = 0;
				while ((ch1 = fgetc(ftemp)) != '\n')
					s[i++] = ch1;
				s[i] = '\0';
				//printf("macro body = '%s'\n", s);
				strcpy(m[macro_no++].m_body, s);
				//printf("struct name: %s\n body: %s\n", m[macro_no - 1].m_name, m[macro_no - 1].m_body);
			}
			else if (p = strstr(s, "include"))
			{
				//printf("Inside include\n");
				if ((p = strstr(s, "<")) && (q = strstr(s, ">")) && p > q)
					printf("p=%d\n", *p);
				else
				{
					while ((ch1 = fgetc(ftemp)) == ' ')
						;
					printf("ch1=%c\n", ch1);
				}

				if (ch1 == '<')
				{
					i = 0;
					ch1 = fgetc(ftemp);
					while (ch1 != '>')
					{
						s[i] = ch1;
						ch1 = fgetc(ftemp);
						i++;
					}
					s[i] = '\0';
					//////////////////////////////////////////////////////////////////////////////////////

					if (i)
					{
						FILE *fhead = fopen(s, "r");
						if (!fhead)
						{
							printf("Error: Header file not found\n");
							break;
						}
						int n = macro_no;
						if (macro_no > 0)
						{
							
							while ((fgets(s1, 100, fhead)) != 0)
							{
								macro_no = n;
								while(macro_no>=0)
								{
								if (p = (strstr(s1, m[macro_no].m_name)))
								{
									i = 0;
									while (s1 + i != p)
										fputc(s1[i++], fpnew);
									fputs(m[macro_no].m_body, fpnew);
									i = i + strlen(m[macro_no].m_name);
									while (s1[i])
										fputc(s1[i++], fpnew);
								}
								else
								{
									printf("Inside macro else\n macro no =%d\n", macro_no);
									fputs(s1, fpnew);
								}
								macro_no--;
								}
							}
						}
						else
						{
							while ((fgets(s1, 100, fhead)) != 0)
							{
								fputs(s1, fpnew);
							}
						}
					}

				} //end of if
			}
		} //end of main if
		else
		{
			fputc(ch, fpnew);
			//printf("%c ", ch);
		}
	}
}
