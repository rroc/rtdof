#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TTextFileHandler.h"

char* TTextFileHandler::TextFileRead(char *aFileName) 
	{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (aFileName != NULL) 
		{
		fp = fopen(aFileName,"rt");
	
		if (fp != NULL) 
			{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) 
				{
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
				}
			fclose(fp);
			}
		}
		return content;
	}

int TTextFileHandler::TextFileWrite(char *aFileName, char *aDataString)
	{
	FILE *fp;
	int status = 0;
	if (aFileName != NULL) 
		{
		fp = fopen(aFileName,"w");
		if (fp != NULL) 
			{
			if (fwrite(aDataString,sizeof(char),strlen(aDataString),fp) == strlen(aDataString))
				{
				status = 1;
				}
			fclose(fp);
			}
		}
	return(status);
	}




