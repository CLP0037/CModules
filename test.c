#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configfile.h"

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





