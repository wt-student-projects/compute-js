
#include "JsPlatform.h"

JsPlatform::JsPlatform() :
    disposeBackgroundThread(false)
{
    backgroundThread = std::thread([this]()
    {
        while (!queue.empty() || !disposeBackgroundThread)
        {
            v8::Task* task = nullptr;
            if (queue.try_pop(task))
            {
                task->Run();
                delete task;
            }
        }
    });
}

JsPlatform::~JsPlatform()
{
    disposeBackgroundThread = true;
    backgroundThread.join();
}

void JsPlatform::CallOnBackgroundThread(v8::Task* task, ExpectedRuntime expected_runtime)
{
    queue.push(task);
};

std::vector<SDL_Event>& JsPlatform::GetSystemEvents()
{
    return events;
}

void JsPlatform::CallOnForegroundThread(v8::Isolate* isolate, v8::Task* task)
{
    buffer.push_back({ task, false });
};

void JsPlatform::CallDelayedOnForegroundThread(v8::Isolate* isolate, v8::Task* task, double delay_in_seconds)
{
    buffer.push_back({ task, false });
};

double JsPlatform::MonotonicallyIncreasingTime()
{ 
    return SDL_GetTicks();
}

void JsPlatform::CallOnForegroundThread(std::pair<v8::Task*, bool> task)
{
    buffer.push_back(task);
}

bool JsPlatform::PumpMessageLoop(v8::Isolate* isolate)
{  
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) 
    {
        events.push_back(e);
    }

    auto tasks = move(buffer);

    for(auto& task : tasks)
    {
        task.first->Run();
        delete task.first;
    }
   
    events.clear();

    const auto& start = buffer.begin();
    const auto& end = buffer.end();
   
    return std::find_if(start, end, [](auto& t) { return t.second; }) != end;
}