#pragma once

/*
* ���� : https://modoocode.com/285
* ���� thread�� ó���� �Լ��� main�� �ְ�
* ThreadPool�� �־��ָ� �˾Ƽ� �ش� �Լ��� thread�� ���۽�Ų��.
* main�� �ִ� �Լ��� thread�� �����ϹǷ� UI ����, �޽��� ���޵��� ����������.
*/

#include <afxwin.h>
#include <functional>
#include <future>
#include <thread>
#include <queue>
#include <vector>
#include <condition_variable>
#include <mutex>

static const UINT Message_ThreadPool = ::RegisterWindowMessage(_T("MessageString_ThreadPool"));

/*
class ThreadPool {
public:
    void Start();
    void QueueJob(const std::function<void()>& job);
    void Stop();
    bool busy();

private:
    void ThreadLoop();

    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
};
*/

#if 0
class ThreadPool {
public:
    ThreadPool();
    template<class F, class... Args>
    //auto Enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
    void EnqueueJob(std::function<void()> job);
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // the task queue
    std::queue< std::function<void()> > tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};
#endif

#if 0
class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    // job �߰�
    void EnqueueJob(std::function<void()> job);

    // C ++14
    template<class Func, class... Args>
    std::future<typename std::result_of<Func(Args...)>::type>
        EmplaceJobAndGetFuture(Func&& f, Args&&... args);


private:
    // �� Worker �������� ����
    size_t num_threads;
    // Worker �����带 �����ϴ� ����
    std::vector<std::thread> worker_threads;
    // �� �ϵ��� �����ϴ� job ť
    std::queue<std::function<void()>> jobQue;
    // ���� job ť���� ���� ���� ������ ���ؽ�
    std::condition_variable cv;
    std::mutex mtx;

    // ��� ������ ����
    bool stop_all;

    // �۾� ť�� �ִ� �۾� �ϳ��� �����Ѵ�.
    void WorkerThread();
};
#endif


#if 1
class ThreadPool {
public:
    ThreadPool();
    ThreadPool(size_t num_threads);
    ~ThreadPool();

    // job �� �߰��Ѵ�.
    template <class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(
        F&& f, Args&&... args);

private:
    // �� Worker �������� ����.
    size_t num_threads_;
    // Worker �����带 �����ϴ� ����.
    std::vector<std::thread> worker_threads_;
    // ���ϵ��� �����ϴ� job ť.
    std::queue<std::function<void()>> jobs_;
    // ���� job ť�� ���� cv �� m.
    std::condition_variable cv_job_q_;
    std::mutex m_job_q_;

    // ��� ������ ����
    bool stop_all;

    // Worker ������
    void WorkerThread();
};

template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> ThreadPool::EnqueueJob(
    F&& f, Args&&... args) {
    if (stop_all) {
        throw std::runtime_error("ThreadPool ��� ������");
    }

    using return_type = typename std::result_of<F(Args...)>::type;
    auto job = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> job_result_future = job->get_future();
    {
        std::lock_guard<std::mutex> lock(m_job_q_);
        jobs_.push([job]() { (*job)(); });
    }
    cv_job_q_.notify_one();

    //Sleep()�� �ɾ����� ���� ��� EnqueueJob()�� ª�� �ð� �������� ȣ��� ��� ���� �߻�.
    Sleep(10);

    return job_result_future;
}
#endif
