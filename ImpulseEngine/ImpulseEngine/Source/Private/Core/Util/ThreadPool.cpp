#include "gepch.h"

#include "Public/Core/Util/ThreadPool.h"

namespace GEngine {

	void ThreadPool::AddJob(std::function<void()> func)
	{
		{
			std::lock_guard<std::mutex> g(ThreadPool::queueMutex);
			ThreadPool::jobQueue.push(func);
		}
		ThreadPool::condition.notify_one();

	}

	void ThreadPool::Setup()
	{
		ThreadPool::MAIN_THREAD_ID = std::this_thread::get_id();
		int numThreads = std::thread::hardware_concurrency();
		if (numThreads == 0) numThreads = 1;
		GE_CORE_INFO("ThreadPool: Created {0} threads", numThreads);
		std::lock_guard<std::mutex> g(ThreadPool::threadMutex);
		for (int i = 0; i < numThreads; i++) {
			threads.push_back(std::thread([]() {
				std::function<void()> job;
				
				while (true) {
					{
						{
							std::unique_lock<std::mutex> lock(ThreadPool::queueMutex);
							condition.wait(lock, [] {return !ThreadPool::jobQueue.empty() || ThreadPool::terminateThreads; });
							if (ThreadPool::jobQueue.empty()) break;
							job = ThreadPool::jobQueue.front();
							ThreadPool::jobQueue.pop();
						}
						job();
					}
				}
				}));
		}
	}

	void ThreadPool::Shutdown()
	{
		
		std::lock_guard<std::mutex> g(ThreadPool::threadMutex);
		for (std::thread& t : ThreadPool::threads) {
			t.detach();
		}
		ThreadPool::terminateThreads = true;
		ThreadPool::condition.notify_all();
		ThreadPool::threads.clear();

	}

	std::thread::id ThreadPool::GetThreadId()
	{
		return std::this_thread::get_id();
	}

	

	bool ThreadPool::OnMainThread()
	{
		return ThreadPool::MAIN_THREAD_ID == GetThreadId();
	}

	void ThreadPool::AddMainThreadFunction(std::function<void()> func)
	{
		std::lock_guard<std::mutex> guard(m_MainthreadMutex);
		ThreadPool::m_MainthreadQueue.push(func);
	}

	void ThreadPool::AddEndFrameFunction(std::function<void()> func)
	{
		std::lock_guard<std::mutex> guard(m_EndthreadMutex);
		ThreadPool::m_EndthreadQueue.push(func);
	}



	std::function<void()> ThreadPool::GetMainThreadFunction()
	{
		std::lock_guard<std::mutex> guard(m_MainthreadMutex);
		if (ThreadPool::m_MainthreadQueue.size() == 0)
			return nullptr;
		std::function<void()> f = m_MainthreadQueue.front();
		m_MainthreadQueue.pop();
		return f;
	}

	
	std::queue<std::function<void()>>& ThreadPool::GetEndThreadFunction()
	{
		std::lock_guard<std::mutex> guard(m_EndthreadMutex);
		return ThreadPool::m_EndthreadQueue;
	}

	std::queue<std::function<void()>>& ThreadPool::GetMainThreadFunctions()
	{
		std::lock_guard<std::mutex> guard(m_MainthreadMutex);
		return m_MainthreadQueue;
	}

	std::mutex& ThreadPool::GetMainFunctionsMutex()
	{
		return m_MainthreadMutex;
	}

	std::mutex& ThreadPool::GetEndThreadFunctionsMutex()
	{
		return m_EndthreadMutex;
	}

	std::vector < std::thread > ThreadPool::threads;
	std::queue <std::function<void()>> ThreadPool::jobQueue;
	std::mutex ThreadPool::queueMutex;
	std::mutex ThreadPool::threadMutex;
	std::condition_variable ThreadPool::condition;
	bool ThreadPool::terminateThreads = false;
	std::thread::id ThreadPool::MAIN_THREAD_ID;
	std::queue<std::function<void()>> ThreadPool::m_MainthreadQueue;
	std::queue<std::function<void()>> ThreadPool::m_EndthreadQueue;
	std::mutex ThreadPool::m_MainthreadMutex;
	std::mutex ThreadPool::m_EndthreadMutex;
}