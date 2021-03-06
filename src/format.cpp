#include <string>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hours = seconds / 3600;
    seconds = seconds - hours*3600;
    int minutes = seconds / 60;
    seconds = seconds - 60*minutes;

    string hour_string, minute_string, second_string;
    hour_string = hours < 10 ? "0" + to_string(hours) : to_string(hours);
    minute_string = minutes < 10 ? "0" + to_string(minutes) : to_string(minutes);
    second_string = seconds < 10 ? "0" + to_string(seconds) : to_string(seconds);

    return hour_string +  ":" + minute_string + ":"  + second_string;
}
