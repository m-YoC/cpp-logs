#pragma once

#include "basis.hpp"

#include "directory.hpp"
#include "time.hpp"
#include "file.hpp"
#include "pack.hpp"

namespace Logs {
    namespace File {
        /* stringstream */
        struct StringStream : public Data {
            std::string* str;
            std::stringstream ss;

            void init(std::string* str_p = nullptr) {
                str = str_p;
                filename = "stringstream";
                os_ptr = &ss;
                status = Status::init;
            }

            StringStream() { init(); }
            StringStream(std::string& s) { init(&s); }
            
            StringStream(const StringStream& dss): str(dss.str) {
                filename = "stringstream";
                os_ptr = &ss;
                status = dss.status;
                ss << dss.ss.str();
            } 
            StringStream(StringStream&& dss): str(dss.str) {
                filename = "stringstream";
                os_ptr = &ss;
                status = dss.status;
                ss = std::move(dss.ss);
            }

            std::string s() const {
                return ss.str();
            }

            void erase() {
                ss.str("");
                ss.clear(std::stringstream::goodbit);
            }

            std::string pop() {
                std::string res = ss.str();
                erase();
                return res;
            }

            void writeback() {
                if(str != nullptr) (*str) += ss.str();
                erase();
            }

            ~StringStream() {
               Data::close();
               writeback();
            }

            // cast
            operator std::string() {
                return s();
            }
        };

        StringStream use_ss() {
            return StringStream();
        }

        StringStream use_ss(std::string& str) {
            return StringStream(str);
        }
    }

    inline namespace Pack {
        using String = File::StringStream;

        /* stringstream */
        Data open_ss(File::StringStream& dss) {
            return Data({&dss});
        }

    }
}

// stream
std::ostream& operator<<(std::ostream& stream, const Logs::File::StringStream& value) {
    stream << value.s();
    return stream;
}

