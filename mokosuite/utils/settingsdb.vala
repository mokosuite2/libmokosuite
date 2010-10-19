[CCode (has_target = false)]
public delegate void RemoteSettingsCallback (string key, string? value);

[DBus (name = "org.mokosuite.Settings")]
public class RemoteSettingsDatabase : GLib.Object {

    private const string SCHEMA = """
    CREATE TABLE IF NOT EXISTS settings (
    name VARCHAR(30) PRIMARY KEY NOT NULL,
    value TEXT(1024) NOT NULL
    );
    """;

    private Sqlite.Database _conn;
    private HashTable<string,string> _store;    // store in caso di mancata connessione
    private HashTable<string,void*> _callbacks;

    public Sqlite.Database connection { get { return _conn; } }

    public RemoteSettingsDatabase(DBus.Connection system_bus, string path, string db_file) {
        _callbacks = new HashTable<string,void*>(str_hash, str_equal);

        system_bus.register_object (path, this);

        int res = Sqlite.Database.open(db_file, out _conn);
        if (res != Sqlite.OK) {
            _conn = null;
            _store = new HashTable<string,string>(str_hash, str_equal);
            warning("Unable to open application database; will not be able to store settings");

        } else {
            // creazione database
            _conn.exec(SCHEMA);

        }
    }

    [DBus (visible=false)]
    public void callback_add(string key, RemoteSettingsCallback callback) {
        _callbacks.replace(key, (void*) callback);
    }

    [DBus (visible=false)]
    public void callback_remove(string key) {
        _callbacks.remove(key);
    }

    /* === D-BUS API === */

    public string GetSetting(string key, string? default_value)
    {
        string val = null;

        if (_conn != null) {

            string[] result;
            int nrow, ncolumn;
            string errmsg;

            if (_conn.get_table("SELECT value FROM settings WHERE name = '" + key + "'",
                out result, out nrow, out ncolumn, out errmsg) == Sqlite.OK) {

                if (nrow > 0) // prendi solo la prima riga
                    val = result[ncolumn].dup();

                //Sqlite.Database.free_table(result);
            }

            if (errmsg != null) warning("Error reading settings: %s", errmsg);
        } else {

            val = _store.lookup(key);
            if (val != null) val = val.dup();

        }
        return (val != null) ? val : default_value;
    }

    public HashTable<string,string>? GetAllSettings()
    {
        HashTable<string,string> val = null;

        if (_conn != null) {
            string[] result;
            int nrow, ncolumn;
            string errmsg;

            if (_conn.get_table("SELECT name, value FROM settings",
                out result, out nrow, out ncolumn, out errmsg) == Sqlite.OK) {

                for (int i = ncolumn /* parti dalla prima riga del set */; i < (nrow+1)*ncolumn; i += 2) {
                    if (val == null) val = new HashTable<string,string>(str_hash, str_equal);

                    val.insert(result[i].dup() ,result[i+1].dup());
                }

                //Sqlite.Database.free_table(result);
            }

            if (errmsg != null) warning("Error reading settings: %s", errmsg);
        } else {
            val = _store;
        }

        return val;
    }

    public void SetSetting(string key, string? str_value)
    {

        if (_conn != null) {
            if (str_value != null)
                _conn.exec("REPLACE INTO settings (name, value) values('"+key+"', '"+str_value+"')");
            else
                _conn.exec("DELETE FROM settings WHERE name = '"+key+"'");
        } else {
            _store.insert(key, str_value.dup());
        }

        RemoteSettingsCallback cb = (RemoteSettingsCallback) _callbacks.lookup(key);
        if (cb != null)
            cb(key, str_value);
    }

}
