#include "logger.h"


Logger * Logger::instance_ = nullptr;

void Logger::initialize()
{
    static Logger the_instance;
    instance_ = &the_instance;
}
