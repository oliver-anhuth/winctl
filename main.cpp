#include "arg_parse.h"
#include "logger.h"
#include "lua.h"
#include "winctl.h"

#include <gdk/gdk.h>


int main(int argc, char * argv[])
{
    try {
        // Process command line options
        gdk_init(&argc, &argv);
        ArgParse arg_parse{argc, argv};

        if (arg_parse.print_help) {
            ArgParse::print_usage_and_exit();
        } else if (arg_parse.print_functions) {
            WinCtl{}.run_script(LuaPrintAvailableFunctions);
            std::exit(EXIT_SUCCESS);
        }

        WinCtl winctl;
        if (arg_parse.print_all_windows) {
            winctl.add_script(LuaPrintAllWindows);
        } else if (arg_parse.print_windows) {
            winctl.add_script(LuaPrintNormalWindows);
        }
        if (!arg_parse.files.empty()) {
            winctl.add_files(arg_parse.files);
        }

        if (arg_parse.run_once) {
            winctl.run_once();
        }

        if (arg_parse.run_continuous) {
            winctl.run();
        }
    } catch (const ArgParse::error & e) {
        LOG_ERROR("winctl: " << e.what() << ".\n" << "Try 'winctl --help' for more information.");
        std::exit(EXIT_FAILURE);
    } catch (const std::runtime_error & e) {
        LOG_ERROR(e.what());
        std::exit(EXIT_FAILURE);
    }
}
