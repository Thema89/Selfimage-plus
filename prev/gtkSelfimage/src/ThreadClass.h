#ifndef THREAD_CLASS_H_
#define THREAD_CLASS_H_


#define PRI_LOW     1
#define PRI_HIGH    2


class ThreadClass
{
   private:
      GThread *ThreadId_;
      void * Arg_;
   protected:
        static void   *EntryPoint(void*);
        virtual void  Execute(void*);
        void          *Arg() const {return Arg_;}
        void          Arg(void*a){Arg_ = a;}
   public:
	ThreadClass();
	virtual ~ThreadClass();
	bool          Start(void * arg);
	void          setPriority ( unsigned char pri );
	void          yield ();
	void          join ();
};

#endif //#define THREAD_CLASS_H_
