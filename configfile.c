#define _PARAM_GLOBALS_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "userlib.h"
//#include "paramConfig.h"
#include <stdbool.h>
#include  <ctype.h>

#include "configfile.h"

//#define READ_STR_OK 1
//#define READ_STR_ERR 0
 
//#define SECTION_MAX_LEN 256
//#define STRVALUE_MAX_LEN 256
//#define LINE_CONTENT_MAX_LEN 256



/*
 *去除字符串右端空格
 */
char *strtrimr(char *pstr)
{
    int i;
    i = strlen(pstr) - 1;
    while (isspace(pstr[i]) && (i >= 0))
        pstr[i--] = '\0';
    return pstr;
}
/*
 *去除字符串左端空格
 */
char *strtriml(char *pstr)
{
    int i = 0,j;
    j = strlen(pstr) - 1;
    while (isspace(pstr[i]) && (i <= j))
        i++;
    if (0<i)
        strcpy(pstr, &pstr[i]);
    return pstr;
}
/*
 *去除字符串两端空格
 */
char *strtrim(char *pstr)
{
    char *p;
    p = strtrimr(pstr);
    return strtriml(p);
}


//read value from .ini
void IniReadValue(char* section, char* key, char* val, const char* file)
{
    FILE* fp;
    int i = 0;
    int lineContentLen = 0;
    int position = 0;
    char lineContent[LINE_CONTENT_MAX_LEN];
    bool bFoundSection = false;
    bool bFoundKey = false;
    fp = fopen(file, "r");
    if(fp == NULL)
    {
        printf("%s: Opent file %s failed.\n", __FILE__, file);
        return;
    }
    while(feof(fp) == 0)
    {
        memset(lineContent, 0, LINE_CONTENT_MAX_LEN);
        fgets(lineContent, LINE_CONTENT_MAX_LEN, fp);
        if((lineContent[0] == ';') || (lineContent[0] == '\0') || (lineContent[0] == '\r') || (lineContent[0] == '\n'))
        {
            continue;
        }

        //check section
        if(strncmp(lineContent, section, strlen(section)) == 0)
        {
            bFoundSection = true;
            //printf("Found section = %s\n", lineContent);
            while(feof(fp) == 0)
            {
                memset(lineContent, 0, LINE_CONTENT_MAX_LEN);
                fgets(lineContent, LINE_CONTENT_MAX_LEN, fp);
                //check key
                if(strncmp(lineContent, key, strlen(key)) == 0)
                {
                    bFoundKey = true;
                    lineContentLen = strlen(lineContent);
                    //find value
                    for(i = strlen(key); i < lineContentLen; i++)
                    {
                        if(lineContent[i] == '=')
                        {
                            position = i + 1;
                            break;
                        }
                    }
                    if(i >= lineContentLen) break;
                    strncpy(val, lineContent + position, strlen(lineContent + position));
                    lineContentLen = strlen(val);
                    for(i = 0; i < lineContentLen; i++)
                    {
                        if((lineContent[i] == '\0') || (lineContent[i] == '\r') || (lineContent[i] == '\n'))
                        {
                            val[i] = '\0';
                            break;
                        }
                    }  
                }
                else if(lineContent[0] == '[') 
                {
                    break;
                }
            }
            break;
        }
    }
    if(!bFoundSection){printf("No section = %s\n", section);}
    else if(!bFoundKey){printf("No key = %s\n", key);}
    fclose(fp);
}

int readStringValue(const char* section, char* key, char* val, const char* file)
{
    char sect[SECTION_MAX_LEN];
    //printf("section = %s, key = %s, file = %s\n", section, key, file);
    if (section == NULL || key == NULL || val == NULL || file == NULL)
    {
        printf("%s: input parameter(s) is NULL!\n", __func__);
        return READ_STR_ERR;
    }
    
    memset(sect, 0, SECTION_MAX_LEN);
    sprintf(sect, "[%s]", section);
    //printf("reading value...\n");
    IniReadValue(sect, key, val, file);
    
    return READ_STR_OK;
}

int readIntValue(const char* section, char* key, const char* file)
{
    char strValue[STRVALUE_MAX_LEN];
    memset(strValue, '\0', STRVALUE_MAX_LEN);
    if(readStringValue(section, key, strValue, file) != READ_STR_OK)
    {
        printf("%s: error", __func__);
        return -1;//0
    }
    return(atoi(strValue));
}

void IniWriteValue(const char* section, char* key, char* val, const char* file)
{
    FILE* fp;
    int i = 0, n = 0, err = 0;
    int lineContentLen = 0;
    int position = 0;
    char lineContent[LINE_CONTENT_MAX_LEN];
    char strWrite[LINE_CONTENT_MAX_LEN];
    bool bFoundSection = false;
    bool bFoundKey = false;
    
    memset(lineContent, '\0', LINE_CONTENT_MAX_LEN);
    memset(strWrite, '\0', LINE_CONTENT_MAX_LEN);
    n = sprintf(strWrite, "%s=%s\n", key, val);
    fp = fopen(file, "r+");
    if(fp == NULL)
    {
        printf("%s: Opent file %s failed.\n", __FILE__, file);
        return;
    }
    while(feof(fp) == 0)
    {
        memset(lineContent, 0, LINE_CONTENT_MAX_LEN);
        fgets(lineContent, LINE_CONTENT_MAX_LEN, fp);
        if((lineContent[0] == ';') || (lineContent[0] == '\0') || (lineContent[0] == '\r') || (lineContent[0] == '\n'))
        {
            continue;
        }
        //check section
        if(strncmp(lineContent, section, strlen(section)) == 0)
        {
            bFoundSection = true;
            while(feof(fp) == 0)
            {
                memset(lineContent, 0, LINE_CONTENT_MAX_LEN);
                fgets(lineContent, LINE_CONTENT_MAX_LEN, fp);
                //check key
                if(strncmp(lineContent, key, strlen(key)) == 0)
                {
                    bFoundKey = true;
                    printf("%s: %s=%s\n", __func__, key, val);
                    fseek(fp, (0-strlen(lineContent)),SEEK_CUR);
                    err = fputs(strWrite, fp);
                    if(err < 0){printf("%s err.\n", __func__);}
                    break; 
                }
                else if(lineContent[0] == '[') 
                {
                    break;
                }
            }
            break;
        }
    }
    if(!bFoundSection){printf("No section = %s\n", section);}
    else if(!bFoundKey){printf("No key = %s\n", key);}
    fclose(fp);
}

int writeStringVlaue(const char* section, char* key, char* val, const char* file)
{
    char sect[SECTION_MAX_LEN];
    //printf("section = %s, key = %s, file = %s\n", section, key, file);
    if (section == NULL || key == NULL || val == NULL || file == NULL)
    {
        printf("%s: input parameter(s) is NULL!\n", __func__);
        return READ_STR_ERR;
    }
    memset(sect, '\0', SECTION_MAX_LEN);
    sprintf(sect, "[%s]", section);
    IniWriteValue(sect, key, val, file);
}

int writeIntValue(const char* section, char* key, int val, const char* file)
{
    char strValue[STRVALUE_MAX_LEN];
    memset(strValue, '\0', STRVALUE_MAX_LEN);
    sprintf(strValue, "%-4d", val);
    
    writeStringVlaue(section, key, strValue, file);
}

/*
int main(int argc, char **argv)
{
	//exit(EXIT_FAILURE);
	
	const char* section = "section1"; 
	char* key = "MACAddress"; 
	//char* val; 
	const char* file = "test.ini";
	
    //==========test for read string
	char strValue[STRVALUE_MAX_LEN];
    memset(strValue, '\0', STRVALUE_MAX_LEN);
    if(readStringValue(section, key, strValue, file) != READ_STR_OK)
    {
        printf("%d: error", __LINE__);
        return 0;
    }
	printf("value from section[%s] key[%s] is %s\n", section, key, strValue);
	
    //==========test for read int
    int intValue ;
    char* key_int = "datsetNum";
    intValue = readIntValue(section, key, file);
    if(intValue < 0)
    {
       printf("%d: error", __LINE__);
       exit(EXIT_FAILURE); 
    }
    printf("value from section[%s] key[%s] is %d\n", section, key_int, intValue);

	return EXIT_SUCCESS;
}
*/






