// ThreadPool.cpp : 定义控制台应用程序的入口点。
//
 
// #include "stdafx.h"  //不使用预编译头
#include "threadpool.h"
 
#define DEFAULT_THREAD_COUNT 4
namespace std {
	ThreadPool *
		ThreadPool::GetInstance()
	{
		static ThreadPool g_ThreadPoolInstance;
		return   &g_ThreadPoolInstance;
	}
 
	ThreadPool::ThreadPool()
	{
		for (unsigned int i = 0; i < DEFAULT_THREAD_COUNT; ++i)
		{
			m_Threads.emplace_back(&ThreadExecute);
		}
	}
 
	void ThreadPool::AddThreads(int count)
	{
		std::unique_lock<std::mutex> mutexHolder(m_Mutex);
		m_Threads.reserve(m_Threads.size() + count);
		for (unsigned int i = 0; i < count; ++i)
		{
			m_Threads.emplace_back(&ThreadExecute);
		}
	}
 
	std::mutex &
		ThreadPool::GetMutex()
	{
		return m_Mutex;
	}
 
	void
		ThreadPool::ThreadExecute()
	{
		// plain pointer does not increase reference count
		ThreadPool * threadPool = GetInstance();
 
		while (!threadPool->m_Stopping.load())
		{
			std::function<void()> task;
 
			{
				std::unique_lock<std::mutex> mutexHolder(threadPool->m_Mutex);
 
				threadPool->m_Condition.wait(mutexHolder,
					[threadPool] { return threadPool->m_Stopping.load() || !threadPool->m_WorkQueue.empty(); });
				if (threadPool->m_Stopping.load()&& threadPool->m_WorkQueue.empty())
				{
					printf("qq---------------------------ThreadExecute ----------------- exit\n");
					return;
				}
				task = std::move(threadPool->m_WorkQueue.front());
				threadPool->m_WorkQueue.pop_front();
			}
 
			task(); // execute the task
		}
		printf("ww-------------------------------ThreadExecute ----------------- exit\n");
 
	}
 
	int
		ThreadPool::GetNumberOfCurrentlyIdleThreads()  
	{
		std::unique_lock<std::mutex> mutexHolder(m_Mutex);
	 
		return int(m_Threads.size()) - int(m_WorkQueue.size()); // lousy approximation
	}
	ThreadPool:: ~ThreadPool()
	{
		m_Stopping.store(true);
		m_Condition.notify_all(); // 唤醒所有线程执行
		for (auto & m_Thread : m_Threads)
		{
			if (m_Thread.joinable())
				m_Thread.join();
		}
		printf("~ThreadPool()---------------------ThreadExecute ----------------- exit\n");
	}
 
}