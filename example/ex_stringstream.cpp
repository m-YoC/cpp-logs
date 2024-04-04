#include "../cpplogs/logs.hpp"

#include <string>

int main() {

    {
        // Create stringstream buffer.
        auto ss1 = Logs::String();

        // Open and write.
        auto wss = Logs::open_ss(ss1);
        wss("Hello Logs StringStream World!");

        // Check
        Debug::log("ss1: ", ss1).n();

        // Write to std::string.
        std::string str = ss1; // or ss1.s();
        Debug::log("str: ", str).n();

        // Erase
        ss1.erase();
        wss("2nd text.");

        // Check
        Debug::log("after erase, ss1: ", ss1).n();

        // Pop to std::string
        std::string str2 = ss1.pop();
        wss("3nd text.");

        // Check
        Debug::log("pop, str2: ", str2).n();
        Debug::log("pop, ss1 : ", ss1).n();
    }

    {
        // Create stringstream buffer with std::string.
        std::string str;
        auto ss1 = Logs::String(str);

        // Open and write.
        auto wss = Logs::open_ss(ss1);
        wss("Hello Logs StringStream World! (ss1)");

        // Reflected in str. (ss data is erased.)
        ss1.writeback();

        // Check
        Debug::log("str: ", str).n();

        str = "";
        {
            auto ss2 = Logs::String(str);
            auto wss2 = Logs::open_ss(ss2);
            wss2("Hello Logs StringStream World! (ss2)");

            // Automatically reflected in str when destructor of Logs::String is used.
        }

        // Check
        Debug::log("str: ", str).n();
    }
    
}
