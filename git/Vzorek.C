#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

std::vector<int> generate_data(int n) {
  std::vector<int> data;
  for (int i = 0; i < n; ++i) {
    data.push_back(rand() % 100 +
                   1); // generates random number between 1 and 100
  }
  return data;
}

std::vector<int> process_data(const std::vector<int> &data) {
  std::vector<int> processed_data;
  for (int x : data) {
    processed_data.push_back(x * 2); //  multiplies each number by 2
  }
  return processed_data;
}

void analyze_data(const std::vector<int> &data) {
  int total = 0;
  for (int x : data) {
    total += x;
  }
  double avg = (data.empty()) ? 0 : static_cast<double>(total) / data.size();
  std::cout << "Total: " << total << ", Average: " << avg << std::endl;
}

void log_data(const std::vector<int> &data, const std::string &message) {
  std::ofstream log_file("log.txt", std::ios::app);
  time_t now = time(0);
  char *dt = ctime(&now);
  dt[strlen(dt) - 1] = '\0'; // remove newline character in the end
  log_file << dt << ": " << message << " - ";
  for (int x : data) {
    log_file << x << " ";
  }
  log_file << std::endl;
}

void simulate_operation() {
  std::vector<int> data = generate_data(50);
  log_data(data, "Generated data");

  std::vector<int> processed_data = process_data(data);
  log_data(processed_data, "Processed data");

  analyze_data(processed_data);
  log_data(processed_data, "Analyzed data");

  sleep(1); //   sleep for 1 second
}

int Vzorek() {
  for (int i = 0; i < 10; ++i) {
    std::cout << "Iteration " << i + 1 << " starting..." << std::endl;
    simulate_operation();
    std::cout << "Iteration " << i + 1 << " complete." << std::endl
              << std::endl;
  }
  return 0;
}
