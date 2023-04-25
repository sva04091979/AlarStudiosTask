#pragma once

#include <chrono>
#include <iostream>

class TTime {
public:
	~TTime() {
		auto time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - m_start).count();
		std::cout << "Time: " << time / 1000.0 << " ms" << std::endl;
	}
private:
	std::chrono::steady_clock::time_point m_start = std::chrono::steady_clock::now();
};

