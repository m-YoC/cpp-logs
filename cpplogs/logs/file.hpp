#pragma once

#include "basis.hpp"

#include "directory.hpp"
#include "time.hpp"

#include <sstream>

namespace Logs {
    namespace File {
        enum class Status {
            not_open,
            init,
            written,
        };

        bool set_time_before_filename = true;

        struct Data;
        std::map<std::string, Data> file_map;

        struct Data {
            std::string filename = "";
            Status status = Status::not_open;
            std::string spacer = "";
            std::ofstream ofs;
            std::ostream* os_ptr;

            std::ostream& stream() { return *os_ptr; }

            template<class T>
            void output_with_spacer(T&& arg) {
                stream() << arg << spacer;
            }

            template<class T>
            void output_after_spacer(T&& arg) {
                stream() << spacer << arg;
            }

            void close() {
                ofs.close();
                status = Status::not_open;
            }

            ~Data() {
                close();
            }
        };

        Data& get_map_value(std::string basename) {
            Time::set_time_string();

            auto itr = file_map.find(basename);

            // map does not have key <basename>
            if( itr == file_map.end() ) {
                // create key and set filename
                auto& v = file_map[basename];
                if(Constant::includes(basename)) {
                    v.filename = basename;
                } else {
                    v.filename = (set_time_before_filename ? Time::time_string + "_" : "") + basename;
                }
                
                return v;
            }else {
                return itr->second;
            }
        }

        void remove_all_map_value() {
            file_map.erase(file_map.begin(), file_map.end());
        }

        Data& open(std::string basename) {
            Data& data = get_map_value(basename);

            if(data.status != Status::not_open) return data;

            if(Constant::includes(data.filename)) {
                data.os_ptr = Constant::get_stream(data.filename);
                data.status = Status::init;
            } else {
                data.ofs.open(Directory::get_basedir() + Directory::get_subdir() + data.filename, std::ios::app);
                data.os_ptr = &data.ofs;
                data.status = (int)data.ofs.tellp() == 0 ? Status::init : Status::written;
            }

            return data;
        }

        void close(std::string basename) {
            auto itr = file_map.find(basename);
            if( itr == file_map.end() ) return;

            file_map.erase(itr);
        }
    }

}