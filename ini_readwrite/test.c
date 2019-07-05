#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configfile.h"

/*
*/

/*
typedef struct INGooseConfig{
    int subscribeFlag;//是否订阅
    char goID[130];
    char gocbRef[130];
    char datSetName[130];
    char macAddress[20];
    unsigned int        vlan_AppID;
    unsigned int        vlan_VID;
    unsigned char       vlan_Priority;
    unsigned int   minTime;
    unsigned int   maxTime;
    unsigned char statusNum;
    unsigned char analogNum;

}INGooseConfig;
*/

/*
//将大写字母转换成小写字母
int tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + 'a' - 'A';
}
    else
    {
        return c;
    }
}

//将十六进制的字符串转换成整数  
long Fixed_key(char s[])
{
    int i;
    int n = 0;
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    {
        i = 2;
    }
    else
    {
        i = 0;
    }
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'); ++i)
    {
        if (tolower(s[i]) > '9')
        {
            n = 16 * n + (10 + tolower(s[i]) - 'a');
        }
        else
        {
            n = 16 * n + (tolower(s[i]) - '0');
        }
    }
    return n;
}
*/



int main(int argc, char **argv)
{

    const char* file = "gooseconfig.ini";

    //the num of IED to subscribe 
    int numIED = readIntValue("PUBDATA","inGooseNum",file);
    if(numIED < 0)
    {
        printf("line_%d error:the inGooseNum < 0,exit\n", __LINE__);
        exit(EXIT_FAILURE);
    }
    printf("[PUBDATA] inGooseNum = %d\n",numIED);

    //遍历获取订阅goose块信息
    //INGooseConfig inGooseConfigParam;
    //inGooseConfigParam = (INGooseConfig *)malloc(sizeof(INGooseConfig));
    //memset(inGooseConfigParam,0,sizeof(INGooseConfig));
   
    //char* section_in ="INGOOSE_";
    char section_in[10]={"INGOOSE_"}; 
    char index_char[2]={};
    char strValue[STRVALUE_MAX_LEN];
    unsigned char macAddress[6]={0};
    int statusNum=0;
    int analogNum=0;
    char statusIndex[20]={0};
    char analogIndex[20]={0};
    char statusMap[16]={"statusMapIndex_"};

    for(int i = 0; i < numIED; i++)
    {
        //printf("====== %d ======\n",(i+1));
        //add the index
        //strcat(section_in,itoa(i+1));
        strcpy(section_in,"INGOOSE_");    
        sprintf(index_char,"%d",(i+1));
        printf("====== index_%s ======\n",index_char);
        strcat(section_in,index_char);
        printf("section_in(%d) : (%s)\n",(i+1),section_in);        
 
        //是否订阅
        int subscribeFlag = readIntValue(section_in,"isSubscribe",file);
        printf("inGooseConfigParam.subscribeFlag(%d): %d\n",(i+1),subscribeFlag);
        //mac地址
        memset(strValue, '\0', STRVALUE_MAX_LEN);
        if(readStringValue(section_in, "macAddress", strValue, file) != READ_STR_OK)
        {   
            printf("%d: error", __LINE__);
            //return 0;
            strcpy(strValue,"00-00-00-00-00-00");
        }   
        printf("value from section[%s] key[%s] is %s", section_in, "macAddress", strValue);
        char *ptr;
        ptr = strtok(strValue,"-");
        int j=0;
        while(ptr != NULL){
            //printf("ptr=%s\n", ptr);
            //macAddress[j++]=atoi(ptr);
            macAddress[j++] = (unsigned char)Fixed_key(ptr);
            ptr = strtok(NULL, "-");
        }
        
        printf("mac address:0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",macAddress[0],macAddress[1],macAddress[2],macAddress[0],macAddress[1],macAddress[2]);

        //goID
        memset(strValue, '\0', STRVALUE_MAX_LEN);
        if(readStringValue(section_in, "goID", strValue, file) != READ_STR_OK)
        {
            printf("%d: error", __LINE__);
            //return 0;
            strcpy(strValue,"000000");
        }
        printf("value from section[%s] key[%s] is %s", section_in, "goID", strValue);

        //gocbRef
        memset(strValue, '\0', STRVALUE_MAX_LEN);
        if(readStringValue(section_in, "gocbRef", strValue, file) != READ_STR_OK)
        {   
            printf("%d: error", __LINE__);
            //return 0;
            strcpy(strValue,"000000");
        }
        printf("value from section[%s] key[%s] is %s", section_in, "gocbRef", strValue);

        //APPID
        memset(strValue, '\0', STRVALUE_MAX_LEN);
        if(readStringValue(section_in, "APPID", strValue, file) != READ_STR_OK)
        {   
            printf("%d: error", __LINE__);
            //return 0;
            strcpy(strValue,"000000");
        }
        printf("value from section[%s] key[%s] is %s", section_in, "APPID", strValue);

        //index map of status
        statusNum = readIntValue(section_in,"statusNum",file);
        if(statusNum > 0)
        {
            printf("statusMapIndex in config:");
            for(int j=0;j<statusNum;j++)
            {
                strcpy(statusMap,"statusMapIndex_");
                sprintf(index_char,"%d",(j+1));
                strcat(statusMap,index_char);

                statusIndex[j]=readIntValue(section_in,statusMap,file);
                printf("%d ",statusIndex[j]);
            }
            printf("\n");
        }
        //index map of analog
        
    }

    printf("======= ====== ====== ======\n");
    numIED = readIntValue("PUBDATA","outGooseNum",file);
    if(numIED <= 0)
    {
        printf("line_%d error:the outGooseNum < 0,exit", __LINE__);
        exit(EXIT_FAILURE);
    }
    printf("[PUBDATA] outGooseNum = %d\n",numIED);
    //获取本设备信息
   
    //遍历获取发布goose块信息
    for(int i = 0; i < numIED; i++)
    {



        //mac地址

        //goID

        //gocbRef

        //APPID

        //minTime

        //maxTime

    }     








/*
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
*/

    return EXIT_SUCCESS;
}





