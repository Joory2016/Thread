// Thread.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <list>

using namespace std;

class TestThread {
public:
	void inMsgRecvQueue() {
		for (int i = 0; i < 100000; ++i) {
			cout << "inMsgRecvQueue()执行,插入一个元素 " << i << endl;
			unique_lock<mutex> sbguard1(myMutex);
			msgRecvQueue.push_back(i);
			//...
		}
	}


	void outMsgRecvQueue() {
		int command = 0;
		for (int i = 0; i < 100000; ++i) {
			bool result = outMsgLULProc(command);
			if (result) {
				cout << "outMsgRecvQueue()执行，取出一个元素 " << command << endl;
				//....
			}
			else {
				cout << "outMsgRecvQueue()执行,但消息队列为空 " << i << endl;
			}
//			cout << endl;
		}
	}

private:
	bool outMsgLULProc(int &command) {
		unique_lock<mutex> sbguard1(myMutex);
		if (!msgRecvQueue.empty()) {
			command = msgRecvQueue.front();
			msgRecvQueue.pop_front();
			return true;
		}
		return false;
	}


private:
	list<int> msgRecvQueue;
	mutex myMutex;	//创建了一个互斥量

};

int main()
{
	TestThread myobj;
	thread myOutMsgObj(&TestThread::outMsgRecvQueue, &myobj);
	thread myInMsgObj(&TestThread::inMsgRecvQueue, &myobj);
	myInMsgObj.join();
	myOutMsgObj.join();

    std::cout << "Hello World!\n"; 
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
