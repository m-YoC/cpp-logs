#pragma once

#include <iostream>

namespace Logs {
    class nullstream : public std::streambuf, public std::ostream {
        public:
        nullstream() : std::ostream( this ) {}
    };

    nullstream nout; 
}


