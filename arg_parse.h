#include <iostream>
#include <stdexcept>

#include <deque>
#include <string>
#include <vector>


struct ArgParse
{
    bool print_help = false;
    bool run_once = true;
    bool run_continuous = true;
    bool print_windows = false;
    bool print_all_windows = false;
    std::vector<std::string> files;

    bool explicit_run_once = false;
    bool explicit_run_continuous = false;

    struct error : public std::runtime_error
    {
        error(const char * msg) : std::runtime_error{msg} {}
    };

    void add_option(const std::string & long_opt, char short_opt)
    {
        if (long_opt == "--help" || short_opt == 'h') {
            print_help = true;
        } else if (long_opt == "--once" || short_opt == 'o') {
            run_continuous = false;
            explicit_run_once = true;
        } else if (long_opt == "--continuous" || short_opt == 'c') {
            run_once = false;
            explicit_run_continuous = true;
        } else if (long_opt == "--print" || short_opt == 'p') {
            print_windows = true;
            run_continuous = false;
        } else if (long_opt == "--print-all" || short_opt == 'P') {
            print_all_windows = true;
            run_continuous = false;
        } else {
            throw error{"Unknown option"};
        }
    }

    void add_arg(const std::string & arg)
    {
        files.push_back(arg);
    }

    void finalize()
    {
        if (!print_help && !print_windows && !print_all_windows && files.empty()) {
            throw error{"No files specified"};
        }

        if (explicit_run_continuous && explicit_run_once) {
            run_once = true;
            run_continuous = true;
        } else if (explicit_run_once) {
            run_once = true;
            run_continuous = false;
        } else if (explicit_run_continuous) {
            run_once = false;
            run_continuous = true;
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

        finalize();
    }

    static void print_usage_and_exit(int status = EXIT_FAILURE)
    {
        std::cout <<"\twinctl - Window matching utility (like devilspie2) which uses relative window positions (percentages) \n"
            << "\n"
            << "SYNOPSIS\n"
            << "\twinctl [options]... [script_file]...\n"
            << "\n"
            << "OPTIONS:\n"
            << "\t--once|-o\n"
            << "\t\tRun once ande exit, unless --continous is specified\n"
            << "\n"
            << "\t--continuous|-c\n"
            << "\t\tRun continously and match new windows. Do not match existing windows at startup unless --once is also specified\n"
            << "\n"
            << "\t--print|-p\n"
            << "\t\tPrint existing regular windows. Implies --once unless --continous is also specified\n"
            << "\n"
            << "\t--print-all|-P\n"
            << "\t\tPrint all existing windows. Implies --once unless --continous is also specified\n"
            << "\n"
            << "\t--help|-h\n"
            << "\t\tPrint this help text\n"
            << "\n"
            << "\n";

        std::exit(status);
    };
};
