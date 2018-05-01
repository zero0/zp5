#pragma once
#ifndef ZP_STACK_TRACE_H
#define ZP_STACK_TRACE_H

enum
{
    ZP_STACK_TRACE_MAX_TRACES = 32,
};

class zpStackTrace
{
public:
    zpStackTrace();
    ~zpStackTrace();

    void print() const;
    zp_uint size() const;
    zp_hash32 hash() const;

    static void Initialize();
    static void Shutdown();
    static void Dump();

private:
    void* m_stackTrace[ ZP_STACK_TRACE_MAX_TRACES ];
    zp_uint m_count;
    zp_hash32 m_hash;
};

#endif // !ZP_STACK_TRACE_H
