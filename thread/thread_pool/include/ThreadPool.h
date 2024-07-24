#ifndef THREAD_POOL_H
#define THREAD_POOL_H
 
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
 
namespace std
{
#define  MAX_THREAD_NUM 16    //256
 
 
	//线程池,可以提交变参函数或拉姆达表达式的匿名函数执行,可以获取执行返回值
	//不支持类成员函数, 支持类静态成员函数或全局函数,Opteron()函数等
	class ThreadPool
	{
	protected:
		/* We need access to the mutex in AddWork, and the variable is only
		* visible in .cxx file, so this method returns it. */
		/* We need access to the mutex in AddWork, and the variable is only
		* visible in .cxx file, so this method returns it. */
		std::mutex &
			GetMutex();
 
		ThreadPool();
		~ThreadPool()  ;
	private:
	
		using Task = std::function<void()>;
	 
		std::mutex          m_Mutex;
		//空闲线程数量
		std::atomic<int>  idlThrNum;
 
		/** This is a list of jobs submitted to the thread pool.
		* This is the only place where the jobs are submitted.
		* Filled by AddWork, emptied by ThreadExecute. */
		std::deque<std::function<void()>> m_WorkQueue;
 
		/** When a thread is idle, it is waiting on m_Condition.
		* AddWork signals it to resume a (random) thread. */
		std::condition_variable m_Condition;
 
		/** Vector to hold all thread handles.
		* Thread handles are used to delete (join) the threads. */
		std::vector<std::thread> m_Threads;
 
		/* Has destruction started? */
		std::atomic<bool> m_Stopping = false;
 
		/** The continuously running thread function */
		static void
			ThreadExecute();
		/** To lock on the internal variables */
		//static ThreadPool  m_ThreadPoolInstance;
	public:
		//void ThreadPool::RemoveInstance();
		static ThreadPool*
			GetInstance();
			void AddThreads(int count);
 
			int GetNumberOfCurrentlyIdleThreads();
		// 提交一个任务
		// 调用.get()获取返回值会等待任务执行完,获取返回值
		// 有两种方法可以实现调用类成员，
		// 一种是使用   bind： .commit(std::bind(&Dog::sayHello, &dog));
		// 一种是用 mem_fn： .commit(std::mem_fn(&Dog::sayHello), &dog)
 
		template <class Function, class... Arguments>
		auto
			AddWork(Function && function, Arguments &&... arguments)
			-> std::future<typename std::result_of<Function(Arguments...)>::type>
		{
			if (m_Stopping.load())    // stop == true ??
				throw std::runtime_error("commit on ThreadPool is stopped.");
			using return_type = typename std::result_of<Function(Arguments...)>::type;
 
			auto task = std::make_shared<std::packaged_task<return_type()>>(
				std::bind(std::forward<Function>(function), std::forward<Arguments>(arguments)...));
 
			std::future<return_type> res = task->get_future();
			{
				std::unique_lock<std::mutex> lock(this->GetMutex());
				m_WorkQueue.emplace_back([task]() { (*task)(); });
			}
			m_Condition.notify_one();
			return res;
		}
		//空闲线程数量
		int idlCount() { return idlThrNum; }
 
	};
 
}
 
#endif