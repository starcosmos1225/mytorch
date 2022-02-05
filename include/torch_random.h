#pragma once
#include <random>
#define PI 3.14159265358979323846
template<typename REngine=std::mt19937>
class Random{
    public:
        //RandomSingleton& get();
        // Random(){
        //     m_randomEngine.seed(std::time(nullptr));
        //     // for (int i=0;i<5;i++)
        //     //     intInRange(i, i*5);
        // }
        // default 5489
        
        // static Random* random = new Random();
        static Random& singleton()
        {
            static Random rand;
            // static Random random = Random();
            return rand;
        }
        // Declare a class template
        template <bool is_integral, typename T> struct uniform_distribution_selector;

        // Specialize for true
        template <typename T> struct uniform_distribution_selector<true, T>
        {
        using type = typename std::uniform_int_distribution<T>;
        };

        // Specialize for false
        template <typename T> struct uniform_distribution_selector<false, T>
        {
        using type = typename std::uniform_real_distribution<T>;
        };

        template<typename T>
        T valueInRange(T low, T high){
            using uniform_distribution_type = typename uniform_distribution_selector<std::is_integral<T>::value, T>::type;
            uniform_distribution_type distribution(low, high);
            double ret = distribution(m_randomEngine);
            return ret;
        }
    private:
        REngine m_randomEngine;
        Random(int seed=5489){
            m_randomEngine.seed(seed);
            // for (int i=0;i<5;i++)
            //     intInRange(i, i*5);
        }

};




// template<typename T>
// std::vector<T> generate_vector(size_t N, T lower = T(0), T higher = T(99))
// {
//    // Select the appropriate distribution type.
//    using uniform_distribution_type = typename uniform_distribution_selector<std::is_integral<T>::value, T>::type;

//    uniform_distribution_type distribution(lower, higher);
//    std::mt19937 engine;
//    auto generator = std::bind(distribution, engine);
//    std::vector<T> vec(N);
//    std::generate(vec.begin(), vec.end(), generator);
//    return vec;
// }