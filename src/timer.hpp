#pragma once

#include <chrono>


class Timer {

    public:
		Timer() {
			Reset();
		}

		void Reset() {
    			m_Start = std::chrono::high_resolution_clock::now();
		}

		float Elapsed() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		float ElapsedMillis() {
			return Elapsed() * 1000.0f;
		}

		uint64_t ElapsedSeconds() {
            auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(ns);
            
		    return std::chrono::duration_cast<std::chrono::seconds>(ms).count();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
