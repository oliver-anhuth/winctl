#include "arg_parse.h"
#include "lua.h"
#include "winctl.h"


void print_error_and_exit(const char * msg, int status = EXIT_FAILURE)
{
    std::cerr << msg << std::endl;
    std::exit(status);
};


int main(int argc, char * argv[])
{
    try {
        // Process command line options
        gdk_init(&argc, &argv);
        ArgParse arg_parse{argc, argv};

        WinCtl winctl;
        if (arg_parse.print_all_windows) {
            winctl.add_script(LuaPrintAllWindows);
        } else if (arg_parse.print_windows) {
            winctl.add_script(LuaPrintNormalWindows);
        } else {
            winctl.add_files(arg_parse.files);
        }

        winctl.run_once();
        if (!arg_parse.run_once) {
            winctl.run();
        }
    } catch (const ArgParse::error &) {
        ArgParse::print_usage_and_exit();
    } catch (const std::runtime_error & e) {
        print_error_and_exit(e.what());
    }
}
