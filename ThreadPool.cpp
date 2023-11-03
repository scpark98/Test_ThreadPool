#include "ThreadPool.h"
#if 0
void ThreadPool::Start()
{
    const uint32_t num_threads = std::thread::hardware_concurrency(); // Max # of threads the system supports
    for (uint32_t ii = 0; ii < num_threads; ++ii) {
        threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
    }
}

void ThreadPool::ThreadLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
                });
            if (should_terminate) {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}

void ThreadPool::QueueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::busy() {
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = !jobs.empty();
    }
    return poolbusy;
}

void ThreadPool::Stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}
#endif

#if 0
// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool()
    : stop(false)
{
    for (size_t i = 0; i < 5; ++i)
        workers.emplace_back(
            [this]
            {
                for (;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            }
            );
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}

void ThreadPool::EnqueueJob(std::function<void()> job)
{
    if (stop) {
        throw std::runtime_error("ThreadPool ��� ������");
    }
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        tasks.push(std::move(job));
    }
    condition.notify_one();
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers)
        worker.join();
}
#endif





#if 0
ThreadPool::ThreadPool()
    :
    num_threads(5),
    stop_all(false)
{
    worker_threads.reserve(num_threads);
    for (size_t i = 0; i < num_threads; ++i)
    {
        // ��Ŀ ������ �߰�
        worker_threads.emplace_back([this]() { this->WorkerThread(); });
    }
}

ThreadPool::~ThreadPool()
{
    stop_all = true;
    // �ڰ� �ִ� ��� �����带 �����.
    cv.notify_all();

    // �շ� �Ұ����ϰ� �����.
    for (auto& t : worker_threads)
    {
        t.join();
    }
}

void ThreadPool::EnqueueJob(std::function<void()> job)
{
    if (stop_all)
    {
        throw std::runtime_error("ThreadPool ��� ���� ��");
    }
    {
        std::lock_guard<std::mutex> lock(mtx);

        // �۾� ť ��ȣ�ϰ� �۾��� �߰��Ѵ�.
        jobQue.push(std::move(job));
    }
    cv.notify_one();
}

void ThreadPool::WorkerThread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);

        // ������� �ʰų� stop_all�� ��� ���� ������ �����Ѵ�.
        cv.wait(lock, [this]() { return !this->jobQue.empty() || stop_all; });

        // ������ ����� ��� �۾��� ������ ��� return
        if (stop_all && this->jobQue.empty())
        {
            return;
        }

        // �� ���� �۾��� ����.
        auto nextJob = std::move(jobQue.front());
        jobQue.pop();
        lock.unlock();

        // �ش� job�� �����Ѵ�.
        nextJob();
    }
}

// job�� �߰��ϰ� ����� ���� �� �ִ� �̷� ��ü�� ��ȯ�Ѵ�.
template<class Func, class... Args>
std::future<typename std::result_of<Func(Args...)>::type>
EmplaceJobAndGetFuture(Func&& f, Args&&... args)
{
    // C++ 14
    using return_type = typename std::result_of<Func(Args...)>::type;
    // �Ϻ� ���޷� �Լ��� �μ��� �����Ѵ�.
    auto callable = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);

    using job_type = std::packaged_task<return_type()>;
    auto job = std::make_shared<job_type>(std::move(callable));
    // �۾��� ������ �̷� ��ü�� ���´�.
    auto result_future = job->get_future();
    {
        std::lock_guard<std::mutex> lock(mtx);

        jobQue.push([job]()
            {
                (*job)();
            });
    }
    cv.notify_one();

    return result_future;
}
#endif

#if 1
ThreadPool::ThreadPool()
    : num_threads_(10), stop_all(false) {
    worker_threads_.reserve(num_threads_);
    for (size_t i = 0; i < num_threads_; ++i) {
        worker_threads_.emplace_back([this]() { this->WorkerThread(); });
    }
}

ThreadPool::ThreadPool(size_t num_threads)
    : num_threads_(num_threads), stop_all(false) {
    worker_threads_.reserve(num_threads_);
    for (size_t i = 0; i < num_threads_; ++i) {
        worker_threads_.emplace_back([this]() { this->WorkerThread(); });
    }
}

void ThreadPool::WorkerThread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(m_job_q_);
        cv_job_q_.wait(lock, [this]() { return !this->jobs_.empty() || stop_all; });
        if (stop_all && this->jobs_.empty()) {
            return;
        }

        // �� ���� job �� ����.
        // �Լ� �������̹Ƿ� ���� std::move�� �� �ʿ�� ����.
        //std::function<void()> job = std::move(jobs_.front());
        std::function<void()> job = jobs_.front();
        jobs_.pop();
        lock.unlock();

        // �ش� job �� �����Ѵ� :)
        job();
    }
}

ThreadPool::~ThreadPool() {
    stop_all = true;
    cv_job_q_.notify_all();

    for (auto& t : worker_threads_) {
        t.join();
    }
}
#endif
