
#include "../cpplogs/logs.hpp"

#include <vector>

int main() {

    // First Simple Log
    Debug::log("Hello", "World!").n();

    // Set spacer between each arg.
    Debug::s(" ")("Hello", "World!").n();
    Debug::s("<sapce>", false /* Set spacer after last arg or not. */)("Hello", "World!").n();

    {
        // Output to stdout using variable.
        auto w = Logs::open_file(Logs::cout_s);
        w.write("Write using Variable.").n();

        // shorthand
        w.w("Write using Variable", " (shorthand type1).").n();
        w("Write using Variable", " (shorthand type2).").n();

        // Use number
        int a = 777;
        float pi = 3.14;
        w.s(" ")("Lucky number is", a, "and PI is", pi).n();

        // Can use w.fixed().set_precision(n).w("...").n();
        // or
        // auto w = Logs::open_file(Logs::cout_s).fixed().set_precision(n);

        // Use vector
        std::vector<int> v = {1, 2, 3, 4, 5};
        w.write_v(v).n();
        w.s(", ").v(v).n();

        // Repeat
        w.rep(5, "hello! ").n();
    }

    {
        // No output keyword.
        auto w = Logs::open_file(Logs::null_s);
        w("Write nothing.").n();
    }

    {
        // Output to file.
        Logs::Directory::set_basedir("./");
        auto w = Logs::open_file("out.txt");
        // create datetime_out.txt
        w("Write to out.txt.").n();
    }

    {
        // Output to 2 or more stream at once. 
        Logs::Directory::set_basedir("./");
        auto w = Logs::open({Logs::cout_s, "out.txt"});
        // If use same filename "out.txt", append to file already existed. 
        // (Until use clean() or finish program.)
        w("Write to out.txt. (2)").n();
    }
    

}
