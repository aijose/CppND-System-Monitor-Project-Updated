#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hours = seconds / 3600;
    seconds = seconds - hours*3600;
    int minutes = seconds / 60;
    seconds = seconds - 60*minutes;
    string hour_string, minute_string, second_string;
    if (hours < 10)
        hour_string = "0" + to_string(hours);
    else
        hour_string = to_string(hours);

    if (minutes < 10)
        minute_string = "0" + to_string(minutes);
    else
        minute_string = to_string(minutes);

    if (seconds < 10)
        second_string = "0" + to_string(seconds);
    else
        second_string = to_string(seconds);

    return hour_string +  ":" + minute_string + ":"  + second_string;
}
