#pragma once

#include "basis.hpp"

namespace Logs::Random {
    namespace {
        const std::string charset_num = "0123456789";
        const std::string charset_abc = "abcdefghijklmnopqrstuvwxyz";
        const std::string charset_ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const std::string charset_abc2 = "abcdefghijkmnpqrstuvwxyz"; // lとo抜き
    }

    int get_randnum(unsigned int max) {
        static std::random_device rd;
        static std::mt19937 mt(rd());

        std::uniform_int_distribution<> dist(0, max);
        return dist(mt);
    }

    namespace Set {
        using F = char(*)();

        char number() {
            return charset_num[get_randnum(charset_num.size() - 1)];
        }

        char abc() {
            return charset_abc[get_randnum(charset_abc.size() - 1)];
        }

        char ABC() {
            return charset_ABC[get_randnum(charset_ABC.size() - 1)];
        }

        char n_abc() {
            static std::string charset = charset_num + charset_abc;
            return charset[get_randnum(charset.size() - 1)];
        }

        char n_aA() {
            static std::string charset = charset_num + charset_abc + charset_ABC;
            return charset[get_randnum(charset.size() - 1)];
        }
    }

    std::string get_string(unsigned int size, Set::F set = Set::n_aA) {
        std::string res(size, '.');
        std::generate_n(res.begin(), size, set);
        return res;
    }

    std::string get_str_with_spacer(unsigned int blocksize, unsigned int repsize, std::string spacer, Set::F set = Set::n_aA) {
        std::string res = "";
        for(int i = 0; i < repsize; ++i) {
            if(i != 0) res += spacer;
            res += get_string(blocksize, set);
        }
        return res;
    }
}
