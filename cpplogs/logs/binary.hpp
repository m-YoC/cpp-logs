#pragma once

#include "basis.hpp"

#include "directory.hpp"
#include "time.hpp"
#include "file.hpp"

namespace Logs {
    namespace Binary {
        enum class Endian {
            little,
            big
        };

        Endian get_endian() {
            std::uint16_t n = 1;
            return *reinterpret_cast<unsigned char*>(&n) == 1 ? Endian::little : Endian::big; 
        }

        const unsigned char endian_char() {
            return get_endian() == Endian::big;
        }

        namespace Context {
            namespace Base {
                template<class T>
                T read(std::ifstream& ifs) {
                    T d;
                    ifs.read(reinterpret_cast<char*>(&d), sizeof(T));
                    return d;
                }

                template<class T>
                void write(std::ofstream& ofs, T d) {
                    ofs.write(reinterpret_cast<const char*>(&d), sizeof(T));
                }

                template<class T, class TARR = std::vector<T>>
                TARR read_arr(std::ifstream& ifs, std::size_t size) {
                    const std::uint32_t byte = sizeof(T);
                    TARR d; 
                    d.resize(size);
                    ifs.read(reinterpret_cast<char*>(d.data()), size * byte);
                    return d;
                }

                template<class T>
                void write_arr(std::ofstream& ofs, T* d, std::size_t size) {
                    const std::uint32_t byte = sizeof(T);
                    ofs.write(reinterpret_cast<const char*>(d), size * byte);
                }
            }

            namespace Version {
                unsigned char read(std::ifstream& ifs) {
                    return Base::read<unsigned char>(ifs);
                }

                void write(std::ofstream& ofs, unsigned char ver) {
                    Base::write<unsigned char>(ofs, ver);
                }
            }

            namespace Endian {
                unsigned char read(std::ifstream& ifs) {
                    return Base::read<unsigned char>(ifs);
                }

                void write(std::ofstream& ofs) {
                    Base::write<unsigned char>(ofs, endian_char());
                }
            }

            namespace String {
                std::string read(std::ifstream& ifs) {
                    auto hsize = Context::Base::read<std::uint32_t>(ifs);
                    return Base::read_arr<char, std::string>(ifs, hsize);
                }

                void write(std::ofstream& ofs, const char* hdata, std::uint32_t hsize) {
                    Base::write<std::uint32_t>(ofs, hsize);
                    Base::write_arr<const char>(ofs, hdata, hsize);
                }
            }
            namespace StructArray {
                template<class T>
                std::optional<std::vector<T>> read(std::ifstream& ifs) {
                    auto byte = Context::Base::read<std::uint32_t>(ifs);
                    auto size = Context::Base::read<std::uint32_t>(ifs);

                    if(sizeof(T) != byte) return std::nullopt;

                    return Base::read_arr<T>(ifs, size);
                }

                template<class T>
                void write(std::ofstream& ofs, T* data, std::uint32_t size) {
                    const std::uint32_t byte = sizeof(T);
                    Base::write<std::uint32_t>(ofs, byte);
                    Base::write<std::uint32_t>(ofs, size);
                    Base::write_arr<T>(ofs, data, size);
                }
            }
        }

        // --------------------------------------------------------------------------------------

        // First 1 byte is 1.
        namespace v1 {

            template<class T>
            bool write(std::string filename, T* data, std::uint32_t size,  const char* hdata, std::uint32_t hsize) {
                std::ofstream ofs(Directory::get_fullpath() + filename, std::ios::binary);
                if (!ofs) return false;

                Context::Version::write(ofs, 1);
                Context::Endian::write(ofs);
                Context::String::write(ofs, hdata, hsize);
                Context::StructArray::write(ofs, data, size);
    
                ofs.close();

                return true;
            }

            template<class T>
            bool write(std::string filename, const std::vector<T>& data, const std::string& hstr = "") {
                return write(filename, data.data(), data.size(), hstr.data(), hstr.size());
            }

            template<class T, unsigned int N>
            bool write(std::string filename, const std::array<T, N>& data, const std::string& hstr = "") {
                return write(filename, data.data(), data.size(), hstr.data(), hstr.size());
            }

            template<class T>
            struct ReadData {
                std::string head;
                std::vector<T> data;
            };

            template<class T>
            std::optional<ReadData<T>> read(std::string path) {
                std::ifstream ifs(path, std::ios::binary);
                if (!ifs) return std::nullopt;

                if(Context::Version::read(ifs) != 2) return std::nullopt;
                auto endian = Context::Endian::read(ifs);
                auto head = Context::String::read(ifs);
                auto data = Context::StructArray::read<T>(ifs);
                if(ifs.fail() || !data) return std::nullopt;
                ifs.close();
                
                return ReadData<T>{.head = head, .data = data.value()};
            }
        }

        // --------------------------------------------------------------------------------------

        template<class T>
        bool write(std::string filename, const std::vector<T>& data, const std::string& hstr = "") {
            return v1::write(filename, data.data(), data.size(), hstr.data(), hstr.size());
        }

        template<class T, unsigned int N>
        bool write(std::string filename, const std::array<T, N>& data, const std::string& hstr = "") {
            return v1::write(filename, data.data(), data.size(), hstr.data(), hstr.size());
        }

        template<class T>
        struct ReadData {
            unsigned int version = 0;
            std::vector<T> data;
            std::string head = "";
        };

        template<class T>
        std::optional<ReadData<T>> read(std::string path) {
            std::ifstream ifs(path, std::ios::binary);
            if (!ifs) return std::nullopt;
            auto version = Context::Version::read(ifs);
            ifs.close();

            if(version == 1) {
                auto res = v1::read<T>(path);
                if(!res) return std::nullopt;
                return ReadData<T>{.version = 1, .data = res.value().data, .head = res.value().head};
            }

            return std::nullopt;
        }
    }
}
