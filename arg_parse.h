#include <iostream>
#include <stdexcept>

#include <deque>
#include <string>
#include <vector>


struct ArgParse
{
    bool run_once = false;
    bool print_windows = false;
    bool print_all_windows = false;
    std::vector<std::string> files;

    struct error : public std::runtime_error
    {
        error(const char * msg) : std::runtime_error{msg} {}
    };

    void add_option(const std::string & long_opt, char short_opt)
    {
        if (long_opt == "--once" || short_opt == 'o') {
            run_once = true;
        } else if (long_opt == "--print" || short_opt == 'p') {
            print_windows = true;
        } else if (long_opt == "--print-all" || short_opt == 'P') {
            print_all_windows = true;
        } else {
            throw error{"Unknown option"};
        }
    }

    void add_arg(const std::string & arg)
    {
        files.push_back(arg);
    }

    void check()
    {
        if (!print_windows && !print_all_windows && files.empty()) {
            throw error{"No files specified"};
        }
    }

    ArgParse(int argc, char * argv[])
    {
        std::deque<std::string> args{argv + 1, argv + argc};
        while (!args.empty()) {
            const std::string & arg = args.front();

            if (arg.empty()) {
                throw error{"Empty argument"};
            }

            if (arg[0] != '-') {
                add_arg(arg);
            } else {
                if (arg.size() > 1 && arg[1] == '-') {
                    add_option(arg, 0);
                } else {
                    for (auto iter = arg.begin() + 1; iter != arg.end(); ++iter) {
                        add_option(std::string{}, *iter);
                    }
                }
            }

            args.pop_front();
        }

        check();
    }

    static void print_usage_and_exit(int status = EXIT_FAILURE)
    {
        std::cout << "print_usage_and_exit" << "\n";
        std::exit(status);
    };
};
