#include <iostream>
#include <stdexcept>

#include <deque>
#include <string>
#include <vector>


struct ArgParse
{
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

    void add_option(const std::string & long_opt, char short_opt, std::deque<std::string> & args, const char *& short_opts)
    {
        if (long_opt == "--help" || short_opt == 'h') {
            print_help = true;
        } else if (long_opt == "--once" || short_opt == 'o') {
            run_continuous = false;
            explicit_run_once = true;
        } else if (long_opt == "--continuous" || short_opt == 'c') {
            run_once = false;
            explicit_run_continuous = true;
        } else if (long_opt == "--print-all-windows" || (short_opt == 'p' && *short_opts == 'a')) {
            print_all_windows = print = true;
            run_continuous = false;
            ++short_opts;
        } else if (long_opt == "--print-window-functions" || (short_opt == 'p' && *short_opts == 'f')) {
            print_functions = print = true;
            run_continuous = false;
            ++short_opts;
        } else if (long_opt == "--print-windows" || short_opt == 'p') {
            print_windows = print = true;
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
    {
        parse(argc, argv);
    }

    void parse(int argc, char * argv[])
    {
        std::deque<std::string> args{argv + 1, argv + argc};

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
                    const char * empty = "";
                    add_option(arg, 0, args, empty);
                } else {
                    const char * short_opts = arg.c_str() + 1;
                    std::string empty;
                    while (*short_opts != 0) {
                        char short_opt = *short_opts;
                        ++short_opts;
                        add_option(empty, short_opt, args, short_opts);
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
            << "\t--print-windows|-p\n"
            << "\t\tPrint normal and dialog windows. Implies --once unless --continous is also specified\n"
            << "\n"
            << "\t--print-all-windows|-pa\n"
            << "\t\tPrint all windows. Implies --once unless --continous is also specified\n"
            << "\n"
            << "\t--print-window-functions|-pf\n"
            << "\t\tPrint all Lua functions available for each window and exit.\n"
            << "\n"
            << "\t--help|-h\n"
            << "\t\tPrint this help text and exit\n"
            << "\n"
            << "\n";

        std::exit(status);
    };
};
