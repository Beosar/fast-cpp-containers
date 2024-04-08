#include "list.h"
#include "ordered_set.h"
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <set>
#include "queue.h"
#include <queue>
#include <list>

class XOrShift_RNG_64 {
public:
	using result_type = uint64_t;
	static inline constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	static inline constexpr result_type min() { return 0; }
	XOrShift_RNG_64(const uint64_t Seed) :State(Seed == 0 ? 1 : Seed) {}
	inline uint64_t operator()() {
		State ^= State << 13;
		State ^= State >> 7;
		State ^= State << 17;
		return State;
	}
	inline void discard(size_t Count) {
		for (size_t i = 0; i < Count; ++i) {
			operator()();
		}
	}
private:
	uint64_t State;
};

int main() {

	using namespace std::chrono;

	
	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		
		fast_ordered_set<uint64_t, 1<<20> Set;

		XOrShift_RNG_64 RNG(42);

		for (size_t i = 0; i < 10000000; ++i) {
			Set.emplace(RNG());
		}

		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		{
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

			std::cout << "fast_ordered_set: " << time_span.count() << " seconds.";
			std::cout << std::endl;
		}

		high_resolution_clock::time_point t3 = high_resolution_clock::now();


		while (!Set.empty()) Set.erase(Set.begin());

		high_resolution_clock::time_point t4 = high_resolution_clock::now();

		{
			duration<double> time_span = duration_cast<duration<double>>(t4 - t3);

			std::cout << "fast_ordered_set erase: " << time_span.count() << " seconds.";
			std::cout << std::endl;
		}

		high_resolution_clock::time_point t5 = high_resolution_clock::now();
		
		for (size_t i = 0; i < 10000000; ++i) {
			Set.emplace(RNG());
		}

		high_resolution_clock::time_point t6 = high_resolution_clock::now();

		{
			duration<double> time_span = duration_cast<duration<double>>(t6 - t5);

			std::cout << "fast_ordered_set (again): " << time_span.count() << " seconds.";
			std::cout << std::endl;
		}


	}





	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();

		std::set<uint64_t> Set;

		XOrShift_RNG_64 RNG(42);

		for (size_t i = 0; i < 10000000; ++i) {
			Set.emplace(RNG());
		}

		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		{
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

			std::cout << "std::set: " << time_span.count() << " seconds.";
			std::cout << std::endl;
		}

		high_resolution_clock::time_point t3 = high_resolution_clock::now();


		while (!Set.empty()) Set.erase(Set.begin());

		high_resolution_clock::time_point t4 = high_resolution_clock::now();

		{
			duration<double> time_span = duration_cast<duration<double>>(t4 - t3);

			std::cout << "std::set erase: " << time_span.count() << " seconds.";
			std::cout << std::endl;
		}

		high_resolution_clock::time_point t5 = high_resolution_clock::now();

		for (size_t i = 0; i < 10000000; ++i) {
			Set.emplace(RNG());
		}

		high_resolution_clock::time_point t6 = high_resolution_clock::now();

		{
			duration<double> time_span = duration_cast<duration<double>>(t6 - t5);

			std::cout << "std::set (again): " << time_span.count() << " seconds.";
			std::cout << std::endl;
		}
	}





	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();

		fast_queue<uint64_t, 1 << 20> Set;

		XOrShift_RNG_64 RNG(42);

		for (size_t i = 0; i < 10000000; ++i) {
			Set.emplace(RNG());
		}

		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

		std::cout << "fast_queue: " << time_span.count() << " seconds.";
		std::cout << std::endl;

	}

	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();

		std::queue<uint64_t> Set;

		XOrShift_RNG_64 RNG(42);

		for (size_t i = 0; i < 10000000; ++i) {
			Set.emplace(RNG());
		}

		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

		std::cout << "std::queue: " << time_span.count() << " seconds.";
		std::cout << std::endl;

	}


	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();

		fast_list<uint64_t, 1 << 20> Set;

		XOrShift_RNG_64 RNG(42);

		for (size_t i = 0; i < 10000000; ++i) {
			Set.emplace_back(RNG());
		}

		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

		std::cout << "fast_list: " << time_span.count() << " seconds.";
		std::cout << std::endl;

	}

	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();

		std::list<uint64_t> Set;

		XOrShift_RNG_64 RNG(42);

		for (size_t i = 0; i < 10000000; ++i) {
			Set.emplace_back(RNG());
		}

		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

		std::cout << "std::list: " << time_span.count() << " seconds.";
		std::cout << std::endl;

	}


	return 0;

}