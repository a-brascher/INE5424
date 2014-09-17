// EPOS Thread Abstraction Initialization

#include <system/kmalloc.h>
#include <system.h>
#include <thread.h>
#include <alarm.h>

extern "C" { void __epos_app_entry(); }

__BEGIN_SYS

Thread * Thread::_idle;

void Thread::init()
{
    int (* entry)() = reinterpret_cast<int (*)()>(__epos_app_entry);

    db<Init, Thread>(TRC) << "Thread::init(entry=" << reinterpret_cast<void *>(entry) << ")" << endl;

    db<Init, Thread>(TRC) << "Creating idle thread" << endl;
    _idle = new (kmalloc(sizeof(Thread))) Thread(Thread::idle, State::READY, MINIMUM);

    _running = new (kmalloc(sizeof(Thread))) Thread(entry, RUNNING, NORMAL);

    if(preemptive)
        _timer = new (kmalloc(sizeof(Scheduler_Timer))) Scheduler_Timer(QUANTUM, reschedule);

    db<Init, Thread>(INF) << "Dispatching the first thread: " << _running << endl;

    This_Thread::not_booting();

    _running->_context->load();
}

__END_SYS
