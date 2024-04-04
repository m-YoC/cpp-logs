#pragma once

#include "basis.hpp"
#include <time.h>

namespace Logs {
    namespace Time {
        std::string time_string = "";

        std::string get_time_string_base(const char* format) {
            char s[100];
            time_t t = time(NULL);
            struct tm *t_ptr = localtime(&t);
            int rc = strftime(s, sizeof(s), format, t_ptr);
            return std::string(s);
        }

        /* HoursMinutesSeconds */
        std::string get_time_string() {
            return get_time_string_base("%H%M%S");
        }

        /* Year-Month-Day */
        std::string get_date_string() {
            return get_time_string_base("%Y-%m-%d");
        }

        void set_time_string() {
            if(time_string == "") time_string = get_date_string() + "-" + get_time_string();
        }

        void reset_time_string() {
            time_string = "";
        }
    }
}
