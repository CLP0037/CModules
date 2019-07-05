#ifndef CONFIGFILEH
#define CONFIGFILEH

#define READ_STR_OK 1
#define READ_STR_ERR 0

#define SECTION_MAX_LEN 256
#define STRVALUE_MAX_LEN 256
#define LINE_CONTENT_MAX_LEN 256

/*将大写字母转换成小写字母*/
int changetolower(int c);
//将十六进制的字符串转换成整数  
long Fixed_key(char s[]);

int readStringValue(const char* section, char* key, char* val, const char* file);

int readIntValue(const char* section, char* key, const char* file);

int writeStringVlaue(const char* section, char* key, char* val, const char* file);

int writeIntValue(const char* section, char* key, int val, const char* file);




#endif
