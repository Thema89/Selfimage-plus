#ifndef BUFFER_H_
#define BUFFER_H_

#include <gtk/gtk.h>

#include "MutexClass.h"

//#define BUFFER_SIZE 1024*1024*10
//#define BUFFER_SIZE 2097153
//#define BUFFER_SIZE 62484
#define BUFFER_SIZE 4194304 //1048576


class TFifoBufferClass{
      private:
              char buffer[BUFFER_SIZE+10];
              int StartPointer, EndPointer;
                bool WriteFinished;
                TMutexClass BufferMutex;

      public:
            TFifoBufferClass ();
            ~TFifoBufferClass();
            void Empty(void);
            bool Done (void);
            bool GetWriteFinished (void);
            void SetWriteFinished (void);
            unsigned int GetSizeOfDataInBuffer (void);
            unsigned int GetFreeSpaceInBuffer (void);
            bool AddData (char *Data, unsigned int size);
            bool GetData (char *Data, unsigned int size);
      
};


#endif //#define BUFFER_H_
