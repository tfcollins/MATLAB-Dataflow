// Utilities

#ifndef UTILS_H
#define UTILS_H

// Blank data, used for sink return calls
void* none;

template<class T>
void* consumeOut(T in)
{
        return (void*) in;
}

// Consume Outputs to create void* output vector
template<typename U>
void Push_back(voidvec &items, U item)
{
        items.push_back((void*)item);
}

template<typename U, typename ...Args>
void Push_back(voidvec &items, U item, Args ...args)
{
        items.push_back((void*)item);
        Push_back( items, args... );
}

template<typename... Args>
voidvec SendOut(Args... Fargs)
{
        voidvec vec;
        Push_back(vec, Fargs...);
        return vec;
}

// Shutdown blocks
template<typename U>
void Shutdown(U block)
{
    (*block).m_StopThread = true;
    int usec = 100;
    boost::this_thread::sleep(boost::posix_time::microseconds(usec));
    (*block).m_BlockThread.join();
}

template<typename U, typename ...Args>
void Shutdown(U block, Args ...args)
{
    (*block).m_StopThread = true;
    (*block).m_BlockThread.join();
    int usec = 100;
    boost::this_thread::sleep(boost::posix_time::microseconds(usec));
    Shutdown( args... );
}
template<typename ...Args>
void StopSim(Args ...args)
{
    // std::cout<<"INFO>> ------SHUTTING DOWN------\n";
    // std::cout<<"INFO>> Waiting for threads to quit\n";
    // std::cout<<"INFO>> Ignore errors that may follow\n";

    try
        {Shutdown(args...);}
    catch(...)
    {}
}


#endif
// [EOF]
