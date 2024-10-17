#include "setting_data.hpp"

#include <string>
#include <glib.h>

#include "utils/glib_utility.hpp"

#define GROUP_NAME   "generial"
#define G_LOG_DOMAIN "Settings"


class Settings {
public:
    static Settings& instance() {
        static Settings settings;
        return settings;
    }

    gint  GetValue(const char *key, gint default_value) {
        GError *err = nullptr;
        int ret = g_key_file_get_integer(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            ERR_LOG(err);
        }
        return ret;
    }

    void SetValue(const char *key, gint value) {
        g_key_file_set_integer(keyfile_, GROUP_NAME, key, value);
    }


    guint  GetValue(const char *key, guint default_value) {
        GError *err = nullptr;
        guint ret = (guint)g_key_file_get_uint64(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            ERR_LOG(err);
        }
        return ret;
    }

    void SetValue(const char *key, guint value) {
        g_key_file_set_uint64(keyfile_, GROUP_NAME, key, value);
    }

    gint64  GetValue(const char *key, gint64 default_value) {
        GError *err = nullptr;
        gint64 ret = g_key_file_get_int64(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            ERR_LOG(err);
        }
        return ret;
    }

    void SetValue(const char *key, gint64 value) {
        g_key_file_set_int64(keyfile_, GROUP_NAME, key, value);
    }

    guint64  GetValue(const char *key, guint64 default_value) {
        GError *err = nullptr;
        guint64 ret = g_key_file_get_uint64(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            ERR_LOG(err);
        }
        return ret;
    }

    void SetValue(const char *key, guint64 value) {
        g_key_file_set_uint64(keyfile_, GROUP_NAME, key, value);
    }

    double  GetValue(const char *key, double default_value) {
        GError *err = nullptr;
        double ret = g_key_file_get_double(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            ERR_LOG(err);
        }
        return ret;
    }

    void SetValue(const char *key, double value) {
        g_key_file_set_double(keyfile_, GROUP_NAME, key, value);
    }

    float  GetValue(const char *key, float default_value) {
        GError *err = nullptr;
        float ret = (float)g_key_file_get_double(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            ERR_LOG(err);
        }
        return ret;
    }

    void SetValue(const char *key, float value) {
        g_key_file_set_double(keyfile_, GROUP_NAME, key, value);
    }

    std::string  GetValue(const char *key, const char* default_value) {
        GError *err = nullptr;
        std::string ret;
        gchar *cstr = g_key_file_get_string(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            ERR_LOG(err);
        } else {
            ret.assign(cstr);
            g_free(cstr);
        }
        return ret;
    }
    
    void SetValue(const char *key, const std::string& value) {
        g_key_file_set_string(keyfile_, GROUP_NAME, key, value.c_str());
    }

    bool  GetValue(const char *key, bool default_value) {
        GError *err = nullptr;
        bool ret = g_key_file_get_boolean(keyfile_, GROUP_NAME, key, &err);
        if (err) {
            ret = default_value;
            ERR_LOG(err);
        }
        return ret;
    }
    
    void SetValue(const char *key, bool value) {
        g_key_file_set_boolean(keyfile_, GROUP_NAME, key, value);
    }


    void Load() {
        if (g_file_test(setting_file_path_, G_FILE_TEST_EXISTS))
        {
            GError *err = nullptr;
            if (!g_key_file_load_from_file(keyfile_, setting_file_path_, G_KEY_FILE_NONE, &err))
            {
                ERR_LOG(err);
            }
        }
        else
            g_warning("setting file not exits!");
    }

    void Save() {
        GError *err = nullptr;
        if (!g_key_file_save_to_file(keyfile_, setting_file_path_, &err))
        {
            ERR_LOG(err);
        }
    }
private:
    Settings() {
        keyfile_ = g_key_file_new();
        auto const cfg_dir = g_build_path("/", g_get_user_config_dir(), g_get_prgname(), nullptr);
        if (-1 == g_mkdir_with_parents(cfg_dir, 0755))
        {
            perror(cfg_dir);
        }
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

#define DEFINE_SETTING_R(type) \
type settings_get_##type(const char *key, type def)
#define DEFINE_SETTING_W(type) \
void settings_set_##type(const char *key, type val)


DEFINE_SETTING_R(gint) {
    return Settings::instance().GetValue(key, def);
}
DEFINE_SETTING_W(gint) {
    return Settings::instance().SetValue(key, val);
}

DEFINE_SETTING_R(guint) {
    return Settings::instance().GetValue(key, def);
}
DEFINE_SETTING_W(guint) {
    return Settings::instance().SetValue(key, val);
}

DEFINE_SETTING_R(gint64) {
    return Settings::instance().GetValue(key, def);
}
DEFINE_SETTING_W(gint64) {
    return Settings::instance().SetValue(key, val);
}

DEFINE_SETTING_R(guint64) {
    return Settings::instance().GetValue(key, def);
}
DEFINE_SETTING_W(guint64) {
    return Settings::instance().SetValue(key, val);
}

DEFINE_SETTING_R(float) {
    return Settings::instance().GetValue(key, def);
}
DEFINE_SETTING_W(float) {
    return Settings::instance().SetValue(key, val);
}

DEFINE_SETTING_R(double) {
    return Settings::instance().GetValue(key, def);
}
DEFINE_SETTING_W(double) {
    return Settings::instance().SetValue(key, val);
}

DEFINE_SETTING_R(str) {
    return Settings::instance().GetValue(key, def.c_str());
}
DEFINE_SETTING_W(str) {
    return Settings::instance().SetValue(key, val);
}

DEFINE_SETTING_R(boolean) {
    return Settings::instance().GetValue(key, def);
}
DEFINE_SETTING_W(boolean) {
    return Settings::instance().SetValue(key, val);
}

void settings_reload() {
    Settings::instance().Load();
}
void settings_save() {
    Settings::instance().Save();
}

#include "utils/custom_section.hpp"
AFTER_APP(ensure_settings_loaded, 0) {
    Settings::instance().Load();
}
