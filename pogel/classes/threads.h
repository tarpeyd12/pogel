#ifndef _THREADS_H
#define _THREADS_H

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define		THREADTYPE		void*
#define		THREADARGS		void*
namespace POGEL {
class THREAD {
	private:
		void* (*thread_function)(void* arg);
		void *data;
		
		pthread_t thread;
		pthread_attr_t attr;
		
	protected:
		
	public:
		
		THREAD() {
			thread_function = NULL;
			data = NULL;
		}
		
		THREAD(void* (*func)(void* arg)) {
			thread_function = func;
			data = NULL;
		}
		
		THREAD(void* (*func)(void* arg), void* dat) {
			thread_function = func;
			data = dat;
		}
		
		~THREAD() {
			pthread_attr_destroy(&attr);
			thread_function = NULL;
			//data = NULL;
		}
		
		void setThread(void* (*func)(void* arg)) {
			thread_function = func;
		}
		
		void setData(void * dat) {
			data = dat;
		}
		
		void startThread() {
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
			
			int ret = pthread_create( &thread, &attr, thread_function, data);
			if( ret ) {
				printf("*** THREAD START FAILURE: %d ***", ret);
				exit(-1);
			}
		}
		
		void joinThread() {
			int ret = pthread_join(thread, NULL);
			if( ret ) {
				printf("*** THREAD JOIN FAILURE: %d ***", ret);
				exit(-1);
			}
		}
		
		//void detachThread();
};
}

#endif /* _THREADS_H */

