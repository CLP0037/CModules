#ifndef CONFIGFILEH
#define CONFIGFILEH

#define READ_STR_OK 1
#define READ_STR_ERR 0

#define SECTION_MAX_LEN 256
#define STRVALUE_MAX_LEN 256
#define LINE_CONTENT_MAX_LEN 256

int readStringValue(const char* section, char* key, char* val, const char* file);

int readIntValue(const char* section, char* key, const char* file);

int writeStringVlaue(const char* section, char* key, char* val, const char* file);

int writeIntValue(const char* section, char* key, int val, const char* file);




#endif
