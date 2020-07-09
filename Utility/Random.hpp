#pragma once
#include <random>

class Random
{
public:
    template<typename T>
    static T Real(T min, T max)
    {
        std::uniform_real_distribution<T> dist(min, max);
        auto& mt = getEngine();
        return dist(mt);
    }

    template<typename T>
    static T Int(T min, T max) 
    {
        std::uniform_int_distribution<T> dist(min, max);
        auto& mt = getEngine();
        return dist(mt);
    }

private:
    static std::mt19937& getEngine() 
    {
        static std::random_device seed_gen;
        static std::mt19937 engine(seed_gen());
        return engine;
    }

};
