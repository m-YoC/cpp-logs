#pragma once

#include "basis.hpp"
#include "base_default_directory.hpp"

namespace Logs {
    namespace Directory {

        namespace {
            bool does_not_have_slash_at_the_end(std::string str){ return str.length() != 0 && str.back() != '/'; }

            const std::string base_default = base_default_directory;
            const std::string sub_default = "";
        }

        std::string base = base_default;
        std::string sub = sub_default;

        void set_basedir(std::string dir) {
            base = does_not_have_slash_at_the_end(dir) ? dir + "/" : dir;
        }

        void set_subdir(std::string dir) {
            sub = does_not_have_slash_at_the_end(dir) ? dir + "/" : dir;
        }

        std::string get_basedir() {
            assert(base != no_default_directory && "Please use Logs::Directory::set_basedir(dir_path)");
            return base;
        }

        std::string get_subdir() {
            return sub;
        }

        void create() {
            std::filesystem::create_directories(get_basedir() + get_subdir());
        }

        std::string get_fullpath() {
            return get_basedir() + get_subdir();
        }

        void reset() {
            base = base_default;
            sub = sub_default;
        }
    }
}