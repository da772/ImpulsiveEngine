#pragma once

namespace GEngine {

	class ThreadPool {
	public:
		static void AddJob(std::function<void()> func);

		static void Setup();
		static void Shutdown();
		
		static std::thread::id GetThreadId();

		static bool OnMainThread();

		static void AddMainThreadFunction(std::function<void()> func);

		static void AddEndFrameFunction(std::function<void()> func);

		static std::function<void()> GetMainThreadFunction();
		static std::queue<std::function<void()>>&  GetEndThreadFunction();
		static std::queue<std::function<void()>>& GetMainThreadFunctions();

		static std::mutex& GetMainFunctionsMutex();
		static std::mutex& GetEndThreadFunctionsMutex();

		static std::vector < std::thread> threads;
		static std::queue <std::function<void()>> jobQueue;
		static std::mutex queueMutex;
		static std::condition_variable condition;
		static bool terminateThreads;
		static std::mutex threadMutex;
		static std::queue<std::function<void()>> m_MainthreadQueue;
		static std::queue<std::function<void()>> m_EndthreadQueue;
		static std::thread::id MAIN_THREAD_ID;
		static std::mutex m_MainthreadMutex;
		static std::mutex m_EndthreadMutex;
		

	};









}