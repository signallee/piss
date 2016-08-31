#ifndef GLOBALTIME_H
#define GLOBALTIME_H

class GlobalTime
{
public:
    long currentTime();

private:
    long m_frequency;

public:
    GlobalTime();
};

#endif // GLOBALTIME_H
