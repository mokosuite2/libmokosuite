
[DBus (name = "org.mokosuite.Config")]
public class RemoteConfigService : Object {

    private bool _autosave = true;
    private string _file;
    private KeyFile _cfg;

    public KeyFile config { get { return _cfg; } }
    public string file { get { return _file; } }
    public bool autosave {
        get { return _autosave; }
        set { _autosave = value; }
    }

    public signal void Changed(string section, string key, Value value);

    public RemoteConfigService(DBus.Connection bus, string path, string cfg_file) {
        bus.register_object (path, this);
        _file = cfg_file;
        _cfg = new KeyFile();

        try {
            _cfg.load_from_file(_file, KeyFileFlags.NONE);
        } catch (Error e) {
            warning("Unable to read configuration; creating new if possible (%s)", e.message);
        }
    }

    private void invoke_callback(string section, string key, Value value) {
        if (this._autosave)
            this.save();
        this.Changed(section, key, value);
    }

    /* === D-BUS API === */

    public bool save() {
        try {
            DirUtils.create_with_parents(Path.get_dirname(_file), 0750);
            return FileUtils.set_contents(_file, _cfg.to_data());
        } catch (Error e) {
            warning("Unable to save configuration (%s)", e.message);
            return false;
        }
    }

    public bool reload() {
        try {
            return _cfg.load_from_file(_file, KeyFileFlags.NONE);
        } catch (Error e) {
            warning("Unable to reload configuration (%s)", e.message);
            return false;
        }
    }

    public bool get_int(string section, string key, out int value) {
        try {
            value = _cfg.get_integer(section, key);
            return true;
        } catch (Error e) {
            return false;
        }
    }

    public bool get_double(string section, string key, out double value) {
        try {
            value = _cfg.get_double(section, key);
            return true;
        } catch (Error e) {
            return false;
        }
    }

    public bool get_bool(string section, string key, out bool value) {
        try {
            value = _cfg.get_boolean(section, key);
            return true;
        } catch (Error e) {
            return false;
        }
    }

    public bool get_string(string section, string key, out string value) {
        try {
            value = _cfg.get_string(section, key);
            return true;
        } catch (Error e) {
            return false;
        }
    }

    public bool set_int(string section, string key, int value) {
        _cfg.set_integer(section, key, value);
        this.invoke_callback(section, key, value);
        return true;
    }

    public bool set_double(string section, string key, double value) {
        _cfg.set_double(section, key, value);
        this.invoke_callback(section, key, value);
        return true;
    }

    public bool set_bool(string section, string key, bool value) {
        _cfg.set_boolean(section, key, value);
        this.invoke_callback(section, key, value);
        return true;
    }

    public bool set_string(string section, string key, string value) {
        _cfg.set_string(section, key, value);
        this.invoke_callback(section, key, value);
        return true;
    }

/*
RemoteSettingsCallback cb = (RemoteSettingsCallback) _callbacks.lookup(key);
if (cb != null)
    cb(key, str_value);
*/

}
