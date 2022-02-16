#include <stdio.h>
#include "curl/curl.h"
#pragma comment(lib, "libcurl.a")
#pragma comment(lib, "libcurl.dll.a")

#define SIZE 50

long StatusHTTP(CURL* curl, char* word);
FILE* CreatehFile(const char* path, const char* mode);
int FileReadLine(char* buffer, FILE* File);
int FileWriteLine(char* buffer1, char* buffer2, FILE* hFile);

FILE* hFile; // RESULT.txt
int word_count = 1;

int main()
{
	CURL* curl;
	
	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (!curl)
	{
		fprintf(stderr, "Error curl init!\n");
		exit(EXIT_FAILURE);
	}

	char* wordlist;
	char* buffer = (char*)calloc(SIZE, sizeof(*buffer));
	if (!buffer)
		return(0);

	printf("Ex: Wordlist: wordlist.txt\n\n");

	printf("Wordlist: ");
	if (fgets(buffer, sizeof(*buffer) * SIZE, stdin) != NULL)
	{
		wordlist = (char*)calloc(strlen(buffer) + 1, sizeof(wordlist));
		if (!wordlist)
			return(0);

		if (*(buffer + strlen(buffer) - 1) == '\n')
			*(buffer + strlen(buffer) - 1) = '\0';
		strcpy_s(wordlist, sizeof(*wordlist) * (strlen(buffer) + 1), buffer);
	}
	else
	{
		printf("Error fgets to wordlist!\n");
		return(0);
	}
	free(buffer);

	FILE* Wordlist = CreatehFile(wordlist, "r");
	char* word = (char*)calloc(SIZE * 2, sizeof(*word));
	if (!word)
	{
		printf("calloc error!\n");
		return(0);
	}
	else
	{
		memset(word, 0, SIZE * 2);
	}

	hFile = CreatehFile("RESULT.txt", "w+");

	
	int loop = 1;
	int word_count = 0;
	while (loop)
	{
		FileReadLine(word, Wordlist);

		if (*(word + strlen(word) - 1) == '\n')
			*(word + strlen(word) - 1) = '\0';
		else
		{
			loop = 0;
			break;
		}
		fprintf(stdout, "Status[%ld] URL[%s] [%d] \n", StatusHTTP(curl, word), word, ++word_count);
		fprintf(hFile, "Status[%ld] URL[%s] [%d] \n", StatusHTTP(curl, word), word, ++word_count);
		//Sleep(10);
	}

	fclose(hFile);
	fclose(Wordlist);
	curl_easy_cleanup(curl);
	return(0);
}

long StatusHTTP(CURL* curl, char* word)
{
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_URL, word);
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);

	CURLcode res;
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform(curl) failed: %s\n", curl_easy_strerror(res));
		return(0);
	}
	
	long http_code;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	return(http_code);
}
int FileWriteLine(char* buffer1, char* buffer2, FILE* hFile)
{
	if (fprintf(hFile, "%s %s\n", buffer1, buffer2))
		return 1;
	else
		return 0;
}

int FileReadLine(char* buffer, FILE* File)
{
	fgets(buffer, SIZE * 2, File);
	return 0;
}


FILE* CreatehFile(const char* path, const char* mode)
{
	FILE* hFile;
	fopen_s(&hFile, path, mode);
	if (!hFile)
		return NULL;
	else
		return hFile;
}
