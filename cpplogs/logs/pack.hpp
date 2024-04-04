#pragma once

#include "basis.hpp"

#include "directory.hpp"
#include "time.hpp"
#include "file.hpp"

namespace Logs {
    inline namespace Pack {
        class Data {
            friend class Data;
            protected:
            std::vector<File::Data*> data;
            bool has_spacer = false;
            std::string spacer = "";
            bool write_spacer_at_last = true;

            template<class T>
            void output_with_spacer(File::Data* data, T&& arg) {
                if(has_spacer) {
                    data->stream() << arg << spacer;
                } else {
                    data->output_with_spacer(arg);
                }
            }

            template<class T>
            void output_after_spacer(File::Data* data, T&& arg) {
                if(has_spacer) {
                    data->stream() << spacer << arg;
                } else {
                    data->output_after_spacer(arg);
                }
            }

            void _void() { return; }

            public:

            File::Data& at(int idx) {
                return *(data[idx]);
            }

            File::Data& operator[](int idx) {
                return at(idx);
            }

            /* set temporary spacer */
            Data set_spacer(std::string spacer = "", bool _write_spacer_at_last = true) {
                auto p = Data(*this);
                p.has_spacer = true;
                p.spacer = spacer;
                p.write_spacer_at_last = _write_spacer_at_last;
                return p;
            }

            /* set temporary spacer */
            Data s(std::string spacer = "", bool _write_spacer_at_last = true) {
                return std::move(set_spacer(spacer, _write_spacer_at_last));
            }

            Data& n() {
                for(auto d: data) d->stream() << std::endl;
                return *this;
            }

            Data& flush() {
                for(auto d: data) d->stream() << std::flush;
                return *this;
            }

            Data& fixed() {
                for(auto d: data) d->stream() << std::fixed;
                return *this;
            }

            Data& set_precision(int n) {
                for(auto d: data) d->stream() << std::setprecision(n);
                return *this;
            }

            Data& default_float() {
                for(auto d: data) d->stream() << std::defaultfloat;
                return *this;
            }

            template<typename... ArgsType>
            Data& write(ArgsType&&... args) {
                if constexpr ( sizeof...(ArgsType) == 0 ) return *this;

                if(write_spacer_at_last) {
                    for(auto d: data) {
                        (output_with_spacer(d, args), ...);
                        d->status = File::Status::written;
                    }
                } else {
                    for(auto d: data) {
                        int count = 0;
                        ((count++ == 0 ? d->stream() << args, _void() :output_after_spacer(d, args)), ...);
                        d->status = File::Status::written;
                    }
                }

                return *this;
            }

            template<typename... ArgsType>
            Data& w(ArgsType&&... args) {
                return write(args...);
            }

            template<typename... ArgsType>
            Data& operator()(ArgsType&&... args) {
                return write(args...);
            }

            template<typename... ArgsType>
            Data& rep(size_t n, ArgsType&&... args) {
                for(size_t i = 0; i < n; ++i) write(args...);
                return *this;
            }

            template<typename ArgType>
            Data& write_v(const ArgType& args) {
                if(write_spacer_at_last) {
                    for(auto d: data) {
                        for(const auto& a: args) output_with_spacer(d, a);
                        d->status = File::Status::written;
                    }     
                } else {
                    for(auto d: data) {
                        int count = 0;
                        for(const auto& a: args) {
                            count++ == 0 ? (d->stream() << a, _void()) : output_after_spacer(d, a);
                        }
                        d->status = File::Status::written;
                    }      
                }
                return *this;
            }

            template<typename ArgType>
            Data& v(const ArgType& args) {
                return write_v(args);
            }

            void close() {
                for(auto d: data) d->close();
            }

            /* constructors */          
            Data() = delete;

            Data(const Data& pack_data) : 
                data(pack_data.data), 
                has_spacer(pack_data.has_spacer), 
                spacer(pack_data.spacer), 
                write_spacer_at_last(pack_data.write_spacer_at_last) 
            {}

            template<class ...ArgsType, 
            std::enable_if_t<std::conjunction<std::is_same<stdp::remove_cr_t<ArgsType>, File::Data>...>::value, std::nullptr_t> = nullptr>
            Data(ArgsType&&... args) {
                data.reserve(sizeof...(ArgsType));
                (data.push_back(&args), ...);
            }

            Data(const std::vector<File::Data*>& args) : data(args) {}

        };

        Data open_file(std::string arg) {
            File::Data* res = &File::open(arg);
            return Data({res});
        }

        Data open(const std::vector<std::string>& args) {
            std::vector<File::Data*> res(args.size());
            for(int i = 0; i < args.size(); ++i) res[i] = &File::open(args[i]);
            return Data(res);
        }

        void close_all_file() {
            File::remove_all_map_value();
        }

    }
}
