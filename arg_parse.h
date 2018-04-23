#ifndef INCLUDED_56D24949_9B62_4500_9230_AA56076D5B4B
#define INCLUDED_56D24949_9B62_4500_9230_AA56076D5B4B

#include "logger.h"

#include <iostream>
#include <sstream>

#include <stdexcept>

#include <deque>
#include <string>
#include <vector>


struct ArgParse
{
    bool run_once = true;
    bool run_continuous = true;

    bool print = false;
    bool print_help = false;
    bool print_version = false;
    bool print_functions = false;
    bool print_windows = false;
    bool print_all_windows = false;

    std::vector<std::string> files;

    bool explicit_run_once = false;
    bool explicit_run_continuous = false;

    void add_option(const std::string & long_opt, char short_opt, std::deque<std::string> & args, const char *& short_opts)
    {
        if (long_opt == "--once" || short_opt == 'o') {
            // Run once at startup
            run_continuous = false;
            explicit_run_once = true;
        } else if (long_opt == "--continuous" || short_opt == 'c') {
            // Run continously after startup
            run_once = false;
            explicit_run_continuous = true;
        }

        else if (long_opt == "--print-all-windows" || (short_opt == 'p' && *short_opts == 'a')) {
            // Print all windows (normal, dialog and others)
            print_all_windows = print = true;
            run_continuous = false;
            ++short_opts;
        } else if (long_opt == "--print-windows" || short_opt == 'p') {
            // Print normal and dialog windows
            print_windows = print = true;
            run_continuous = false;
        }

        else if (long_opt == "--log-level" || (short_opt == 'l' && *short_opts == 'l' && ++short_opts)) {
            // Set log level
            if (args.empty()) {
                throw_error("Missing log level");
            } else if (args.front() == "nothing") {
                Logger::instance().set_log_level(Logger::Nothing);
            } else if (args.front() == "error") {
                Logger::instance().set_log_level(Logger::Error);
            } else if (args.front() == "warning") {
                Logger::instance().set_log_level(Logger::Warning);
            } else if (args.front() == "info") {
                Logger::instance().set_log_level(Logger::Info);
            } else if (args.front() == "debug") {
                Logger::instance().set_log_level(Logger::Debug);
            } else {
                throw_error("Unknown log level ", args.front());
            }
            args.pop_front();
        }

        else if (long_opt == "--help-functions" || (short_opt == 'h' && *short_opts == 'f' && ++short_opts)) {
            // Print available Lua functions and exit
            print_functions = print = true;
            run_continuous = false;
        } else if (long_opt == "--help" || short_opt == 'h') {
            // Print help and exit
            print_help = print = true;
        } else if (long_opt == "--version" || short_opt == 'V') {
            // Print version and exit
            print_version = print = true;
        }

        else {
            if (!long_opt.empty()) {
                throw_error("Unknown option ", long_opt);
            } else {
                throw_error("Unknown option -", short_opt);
            }
        }
    }

    void add_arg(const std::string & arg)
    {
        files.push_back(arg);
    }

    void finalize()
    {
        if (!print && files.empty()) {
            throw error{"No Lua script files specified"};
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
                throw_error("Found empty command line argument");
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

    struct error : public std::runtime_error
    {
        explicit error(const char * msg) : std::runtime_error{msg} {}
        explicit error(const std::string & msg) : std::runtime_error{msg} {}
    };

    template <typename... ARGS>
    void throw_error(ARGS... args)
    {
        std::stringstream ss;
        throw_error(ss, args...);
    }

    template <typename ARG, typename... ARGS>
    void throw_error(std::stringstream & ss, const ARG & arg, ARGS... args)
    {
        ss << arg;
        throw_error(ss, args...);
    }

    template <typename ARG>
    void throw_error(std::stringstream & ss, const ARG & arg)
    {
        ss << arg;
        throw error{ss.str()};
    }

    static void print_usage_and_exit()
    {
        std::cout << "\twinctl - Window matching utility (like devilspie2) which uses relative window positions (percentages)\n"
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
            << "\t--help|-h\n"
            << "\t\tPrint this help text and exit\n"
            << "\n"
            << "\t--help-functions|-hf\n"
            << "\t\tPrint Lua functions available for each window and exit.\n"
            << "\n"
            << "\n";
        std::exit(EXIT_SUCCESS);
    }

    static void print_version_and_exit()
    {
        std::cout << "winctl " << VERSION_MAJOR << "." << VERSION_MINOR << "  Written by Oliver Anhuth\n";
        std::exit(EXIT_SUCCESS);
    };
};

#endif // INCLUDED_56D24949_9B62_4500_9230_AA56076D5B4B#include <iostream>
