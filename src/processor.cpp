#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <chrono>
#include <thread>

using std::vector;
/*
 * Contents of cpu utilization vector
 * 0 - user
 * 1 - nice
 * 2 - system
 * 3 - idle
 * 4 - iowait
 * 5 - irq
 * 6 - softirq
 * 7 - steal
 * 8 - guest
 * 9 - guest_nice
*/
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    int sleep_interval = 100;

    // Read "previous" cpu utilization
    vector<unsigned long long int> previous_time = LinuxParser::CpuUtilization();

    // Wait for 100 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_interval));

    // Read "current" cpu utilization
    vector<unsigned long long int> current_time = LinuxParser::CpuUtilization();

    unsigned long long int prev_idle = previous_time[3] + previous_time[4];
    unsigned long long int current_idle = current_time[3] + current_time[4];

    unsigned long long int prev_non_idle = previous_time[0] + previous_time[1] + previous_time[2] + previous_time[5] + previous_time[6] + previous_time[7 + previous_time[8] + previous_time[9]];
    unsigned long long int current_non_idle = current_time[0] + current_time[1] + current_time[2] + current_time[5] + current_time[6] + current_time[7] + current_time[8] + current_time[9];

    unsigned long long int prev_total = prev_idle + prev_non_idle;
    unsigned long long int current_total = current_idle + current_non_idle;

    // differentiate: actual value minus the previous one
    unsigned long long int diff_total = current_total - prev_total;
    unsigned long long int diff_idle = current_idle - prev_idle;

    float cpu_percentage = (diff_total - diff_idle)*1.0/diff_total;

    return cpu_percentage;
}
