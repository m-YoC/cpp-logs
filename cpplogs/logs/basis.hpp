#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <memory>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <iterator>

#include <filesystem>
#include <type_traits>
#include <algorithm>
#include <functional>
#include <chrono>
#include <cassert>
#include <variant>
#include <random>
#include <optional>

#include "type_traits_plus.hpp"
#include "nullstream.hpp"

namespace Logs {
    inline namespace Constant {
        const auto cout_s = "stdout";
        const auto null_s = "null";

        namespace {
            bool includes(std::string str) {
                return str == cout_s || str == null_s;
            }

            std::ostream* get_stream(std::string str) {
                return str == cout_s ? &std::cout : &Logs::nout;
            }
        }
    }

}

// stringstream.hpp
namespace Logs::File { struct StringStream; }
std::ostream& operator<<(std::ostream& stream, const Logs::File::StringStream& value);
