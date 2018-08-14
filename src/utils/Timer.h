#ifndef MPC_TIMER_H
#define MPC_TIMER_H
#include<memory>
#include <chrono>

const long MILLISECONDS_PER_SECOND = 1000;
const long MICROSECONDS_PER_MILLISECOND = 1000;
const long NANOSECONDS_PER_MICROSECOND = 1000;

const long MICROSECONDS_PER_SECOND = MICROSECONDS_PER_MILLISECOND * MILLISECONDS_PER_SECOND;
const long NANOSECONDS_PER_MILLISECOND = NANOSECONDS_PER_MICROSECOND * MICROSECONDS_PER_MILLISECOND;
const long NANOSECONDS_PER_SECOND = NANOSECONDS_PER_MILLISECOND * MILLISECONDS_PER_SECOND;

const int  SECONDS_PER_MINUTE = 60;
const int  MINUTES_PER_HOUR = 60;
const int  HOURS_PER_DAY = 24;
const int  SECONDS_PER_HOUR = SECONDS_PER_MINUTE * MINUTES_PER_HOUR;

const long MILLISECONDS_PER_MINUTE = MILLISECONDS_PER_SECOND * SECONDS_PER_MINUTE;
const long NANOSECONDS_PER_MINUTE = NANOSECONDS_PER_SECOND * SECONDS_PER_MINUTE;
const long NANOSECONDS_PER_HOUR = NANOSECONDS_PER_SECOND * SECONDS_PER_HOUR;
const long NANOSECONDS_PER_DAY = NANOSECONDS_PER_HOUR * HOURS_PER_DAY;

/**
 * timer for nanosecond, main class
 */
class NanoTimer
{
public:
    /** return current nano time: unix-timestamp * 1e9 + nano-part */
    long   getNano() const;

    static NanoTimer* getInstance();

private:
    NanoTimer(){};
    /** singleton */
    static std::shared_ptr<NanoTimer> m_ptr;
    /** object to be updated every time called */
};

/**
 * util function to utilize NanoTimer
 * @return current nano time in long (unix-timestamp * 1e9 + nano-part)
 */
inline long getNanoTime()
{
    return NanoTimer::getInstance()->getNano();
}




inline NanoTimer* NanoTimer::getInstance()
{
    if (m_ptr.get() == nullptr)
    {
        m_ptr = std::shared_ptr<NanoTimer>(new NanoTimer());
    }
    return m_ptr.get();
}

inline std::chrono::steady_clock::time_point get_time_now()
{
    timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return std::chrono::steady_clock::time_point(
            std::chrono::steady_clock::duration(
                    std::chrono::seconds(tp.tv_sec) + std::chrono::nanoseconds(tp.tv_nsec)
            )
    );
}



inline long NanoTimer::getNano() const
{
    long _nano = std::chrono::duration_cast<std::chrono::nanoseconds>(
            get_time_now().time_since_epoch()
    ).count();
    return _nano;
}


#endif //MPC_TIMER_H