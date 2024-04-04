#pragma once

#include "basis.hpp"

#include "directory.hpp"
#include "time.hpp"
#include "file.hpp"
#include "pack.hpp"
#include "stringstream.hpp"
#include "binary.hpp"

namespace Debug {

    enum class output {
        none,
        all,
        only_set_keys,
    };

    output _output_log = output::all;
    std::vector<std::string> _keys = {};

    void set_debug_log(output b, const std::vector<std::string>& keys = {}) {
        _output_log = b;
        _keys = keys;
    }

    std::string log_out() {
        auto name = Logs::cout_s;
        if(_output_log == output::none) name = Logs::null_s;
        return name;
    }

    std::string log_out_key(std::string key){
        auto name = Logs::cout_s;
        if(_output_log == output::only_set_keys && std::find(_keys.begin(), _keys.end(), key) == _keys.end()) name = Logs::null_s;
        if(_output_log == output::none) name = Logs::null_s;
        return name;
    }

    Logs::Data key(std::string k) {
        return Logs::open_file(log_out_key(k));
    }

    Logs::Data n() {
        return Logs::open_file(log_out()).n();
    }

    Logs::Data s(std::string spacer = "", bool write_spacer_at_last = true) {
        return Logs::open_file(log_out()).set_spacer(spacer, write_spacer_at_last);
    }

    Logs::Data flush() {
        return Logs::open_file(log_out()).flush();
    }

    Logs::Data fixed() {
        return Logs::open_file(log_out()).fixed();
    }

    Logs::Data set_precision(int n) {
        return Logs::open_file(log_out()).set_precision(n);
    }

    Logs::Data default_float() {
        return Logs::open_file(log_out()).default_float();
    }

    template<typename... ArgsType>
    Logs::Data log(ArgsType&&... args) {
        return Logs::open_file(log_out()).write(args...);
    }

    
}

