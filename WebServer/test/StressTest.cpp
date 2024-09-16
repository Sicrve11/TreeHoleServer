/* @Author shigw    @Email sicrve@gmail.com */

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <time.h>
#include "../SkipList.h"

#define NUM_THREADS 1
#define TEST_COUNT 100000

using namespace std;

const string FILE_PATH = "dumpFile.txt";
SkipList skipList(FILE_PATH, 10, 10);       // 定义10层的跳表


// 随机插入测试
void *insertElement(void* threadid) {
    const string USERNAME = "Sicrve";
    const string MESSAGE = "Hello World!";
    const string DATATIME = "today";

    long tid; 
    tid = reinterpret_cast<long>(threadid);
    cout << "insert test in threadid = " << tid << endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; i++) {
        count++;
        unsigned long key = rand() % TEST_COUNT;
        skipList.insertNode(USERNAME, MESSAGE, DATATIME, key);
	}
    pthread_exit(NULL);
}


// 随机查询测试
void *getElement(void* threadid) {
    long tid; 
    tid = reinterpret_cast<long>(threadid);
    cout << "search test in threadid = " << tid << endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; i++) {
        count++;
        unsigned long key = rand() % TEST_COUNT;
		skipList.searchNode(key); 
	}
    pthread_exit(NULL);
}


int main() {
    srand (time(NULL));  
    {

        pthread_t threads[NUM_THREADS];
        int rc;
        int i;

        auto start = std::chrono::high_resolution_clock::now();

        for(i = 0; i < NUM_THREADS; i++) {
            cout << "main() : creating thread, " << i << endl;
            rc = pthread_create(&threads[i], NULL, insertElement, reinterpret_cast<void*>(i));

            if (rc) {
                std::cout << "Error:unable to create thread," << rc << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for( i = 0; i < NUM_THREADS; i++ ) {
            if (pthread_join(threads[i], &ret) !=0 )  {
                perror("pthread_create() error"); 
                exit(3);
            }
        }
        auto finish = std::chrono::high_resolution_clock::now(); 
        chrono::duration<double> elapsed = finish - start;
        cout << "insert " << TEST_COUNT << " data, total time is " << elapsed.count() << endl;
        cout << "insert QPS = " << (TEST_COUNT / elapsed.count()) << endl;
    }


    srand (time(NULL));  
    {

        pthread_t threads[NUM_THREADS];
        int rc;
        int i;

        auto start = std::chrono::high_resolution_clock::now();

        for(i = 0; i < NUM_THREADS; i++) {
            cout << "main() : creating thread, " << i << endl;
            rc = pthread_create(&threads[i], NULL, getElement, reinterpret_cast<void*>(i));

            if (rc) {
                std::cout << "Error:unable to create thread," << rc << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for( i = 0; i < NUM_THREADS; i++ ) {
            if (pthread_join(threads[i], &ret) !=0 )  {
                perror("pthread_create() error"); 
                exit(3);
            }
        }
        auto finish = std::chrono::high_resolution_clock::now(); 
        chrono::duration<double> elapsed = finish - start;
        cout << "search " << TEST_COUNT << " data, total time is " << elapsed.count() << endl;
        cout << "search QPS = " << (TEST_COUNT / elapsed.count()) << endl;
    }

    pthread_exit(NULL);
    return 0;
}
