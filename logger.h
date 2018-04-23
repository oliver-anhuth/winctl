#ifndef INCLUDED_B7DC7162_0B1E_4B62_8874_C86283D2CD4F
#define INCLUDED_B7DC7162_0B1E_4B62_8874_C86283D2CD4F

class Logger
{
public:
    Logger() = default;
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger & operator=(const Logger &) = delete;
    Logger & operator=(Logger &&) = delete;

    static constexpr const int Nothing = -1;
    static constexpr const int Error = 0;
    static constexpr const int Warning = 2;
    static constexpr const int Info = 3;
    static constexpr const int Debug = 4;

    void set_log_level(int level)
    {
        log_level_ = level;
    }

    int log_level() const
    {
        return log_level_;
    }

    bool is_active(int log_level) const
    {
        return log_level <= log_level_;
    }

    static Logger & instance()
    {
        if (instance_ == nullptr) {
            initialize();
        }
        return *instance_;
    }

private:
    int log_level_ = Error;

    static void initialize();
    static Logger * instance_;
};

#define LOG_ERROR(x) if (Logger::instance().is_active(Logger::Error)) { std::cerr << x << "\n"; }
#define LOG_WARNING(x) if (Logger::instance().is_active(Logger::Warning)) { std::cerr << x << "\n"; }
#define LOG_INFO(x) if (Logger::instance().is_active(Logger::Info)) { std::cerr << x << "\n"; }
#define LOG_DEBUG(x) if (Logger::instance().is_active(Logger::Debug)) { std::cerr << x << "\n"; }

#endif // INCLUDED_B7DC7162_0B1E_4B62_8874_C86283D2CD4F
