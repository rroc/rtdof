#ifndef TEXT_FILE_HANDLER_H
#define TEXT_FILE_HANDLER_H

class TTextFileHandler
	{
	public:
		static char* TextFileRead(char *aFileName);
		static int   TextFileWrite(char *aFileName, char *aDataString);
	};

#endif
