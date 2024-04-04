#pragma once

#include "logs/basis.hpp"

#include "logs/directory.hpp"
#include "logs/time.hpp"
#include "logs/file.hpp"
#include "logs/pack.hpp"
#include "logs/stringstream.hpp"
#include "logs/binary.hpp"
#include "logs/random.hpp"

#include "logs/debug_log.hpp"

namespace Logs {

    void clean() {
        File::remove_all_map_value();
        Directory::reset();
        Time::reset_time_string();
    }

}

// --------------------------------------------------------------------------------------


