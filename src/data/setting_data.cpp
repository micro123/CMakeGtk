#include "setting_data.hpp"

#include <string>
#include <glib.h>

#define GROUP_NAME "generial"

class Settings {
public:
    static Settings& instance() {
        static Settings settings;
        return settings;
    }

    int  GetValue(const char *key, int default_value) {
        GError *err = nullptr;
        int ret = g_key_file_get_integer(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            g_error_free(err);
        }
        return ret;
    }

    void SetValue(const char *key, int value) {
        g_key_file_set_integer(keyfile_, GROUP_NAME, key, value);
    }

    double  GetValue(const char *key, double default_value) {
        GError *err = nullptr;
        double ret = g_key_file_get_double(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            g_error_free(err);
        }
        return ret;
    }

    void SetValue(const char *key, double value) {
        g_key_file_set_double(keyfile_, GROUP_NAME, key, value);
    }

    std::string  GetValue(const char *key, const char* default_value) {
        GError *err = nullptr;
        std::string ret = g_key_file_get_string(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            g_error_free(err);
        }
        return ret;
    }
    
    void SetValue(const char *key, const std::string& value) {
        g_key_file_set_string(keyfile_, GROUP_NAME, key, value.c_str());
    }


    void Load() {
        if (g_file_test(setting_file_path_, G_FILE_TEST_EXISTS))
        {
            GError *err = nullptr;
            if (!g_key_file_load_from_file(keyfile_, setting_file_path_, G_KEY_FILE_NONE, &err))
            {
                g_error_free(err);
            }
        }
    }

    void Save() {
        GError *err = nullptr;
        if (!g_key_file_save_to_file(keyfile_, setting_file_path_, &err))
        {
            g_error_free(err);
        }
    }
private:
    Settings() {
        keyfile_ = g_key_file_new();
        auto const cfg_dir = g_build_path("/", g_get_user_config_dir(), g_get_application_name(), nullptr);
        g_mkdir_with_parents(cfg_dir, 0644);
        setting_file_path_ = g_build_path("/", cfg_dir, "dashboard.ini", nullptr);
        g_free(cfg_dir);
        Load();
    }
    ~Settings() {
        Save();
        g_key_file_free(keyfile_);
        g_free(setting_file_path_);
    }

    GKeyFile   *keyfile_;
    gchar      *setting_file_path_;
};

int  settings_get_int(const char *key, int def) {
    return Settings::instance().GetValue(key, def);
}
void settings_set_int(const char *key, int value) {
    return Settings::instance().SetValue(key, value);
}

double  settings_get_double(const char *key, double def) {
    return Settings::instance().GetValue(key, def);
}
void    settings_set_double(const char *key, double value) {
    return Settings::instance().SetValue(key, value);
}

std::string settings_get_str(const char *key, const std::string def) {
    return Settings::instance().GetValue(key, def.c_str());
}
void settings_set_int(const char *key, const std::string &value) {
    return Settings::instance().SetValue(key, value);
}

void settings_reload() {
    Settings::instance().Load();
}
void settings_save() {
    Settings::instance().Save();
}
