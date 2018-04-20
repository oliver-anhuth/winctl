#include <iostream>
#include <stdexcept>

#include <deque>
#include <string>
#include <vector>


struct ArgParse
{
    std::deque<std::string> args;

    struct error : public std::runtime_error
    {
        error(const char * msg) : std::runtime_error{msg} {}
    };

    bool run_once = true;
    bool run_continuous = true;

    bool print = false;
    bool print_help = false;
    bool print_windows = false;
    bool print_all_windows = false;
    bool print_functions = false;

    std::vector<std::string> files;

    bool explicit_run_once = false;
    bool explicit_run_continuous = false;

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
            print_windows = print = true;
            run_continuous = false;
        } else if (long_opt == "--print-all" || short_opt == 'P') {
            print_all_windows = print = true;
            run_continuous = false;
        } else if (long_opt == "--print-functions") {
            print_functions = print = true;
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
        if (!print && files.empty()) {
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
        : args{argv + 1, argv + argc}
    {
        parse();
    }

    void parse()
    {
        while (!args.empty()) {
            std::string arg = std::move(args.front());
            args.pop_front();

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
            << "\t\tRun continously and match newly opening windows.\n"
            << "\t\tDo not match existing windows at startup unless --once is also specified\n"
            << "\n"
            << "\t--print|-p\n"
            << "\t\tPrint normal and dialog windows. Implies --once unless --continous is also specified\n"
            << "\n"
            << "\t--print-all|-P\n"
            << "\t\tPrint all windows. Implies --once unless --continous is also specified\n"
            << "\n"
            << "\t--print-functions\n"
            << "\t\tPrint all Lua functions available for each window w and exit.\n"
            << "\n"
            << "\t--help|-h\n"
            << "\t\tPrint this help text and exit\n"
            << "\n"
            << "\n";

        std::exit(status);
    };
};
