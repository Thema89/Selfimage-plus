#ifndef COPYDATA_H_
#define COPYDATA_H_

#include <gtk/gtk.h>

#define ERROR_READTHREAD_NONE           10
#define ERROR_READTHREAD_FAIL           11
#define ERROR_READTHREAD_COMPRESS       12
#define ERROR_READTHREAD_DECOMPRESS     13

#define ERROR_WRITETHREAD_NONE          20
#define ERROR_WRITETHREAD_FAIL          21
#define ERROR_WRITETHREAD_COMPRESS      22
#define ERROR_WRITETHREAD_DECOMPRESS    23



extern bool handle_copy (void);
extern void *CopyData(void *args);

#endif //COPYDATA_H_
