#include <iostream>

#ifdef WIN32
  #include <windows.h>
#else
  #include <unistd.h>
  #include <termios.h>
#endif

#include "sqlite3.h"
#include "Util.h"
#include "UtilException.h"
#include "Site.h"


// Turn stdin echo on or off
void setEcho(bool enable);


int main(int argc, char* argv[])
{
    // Use std here
    using namespace std;

    // Use CipherKick namespace
    using namespace CipherKick;

    // Our search results
    auto search_results = make_shared<vector<Site>>();

    // Our max site id
    int max_site_id;

    // Check arguments
    if (argc != 2)
    {
        // Invalid number of arguments, print usage and exit
        cout << "Usage: ck2 vault_file" << endl;
        return 1;
    }

    cout << endl << "Welcome to Cipher Kick!\n\nPlease enter key: ";

    // Turn off echo to read password
    setEcho(false);

    // TODO: Handle in-memory password more mindfully

    // Read password
    string keystr;
    getline(cin, keystr);

    // Turn echo back on
    setEcho(true);

    // Open and decrypt the database
    CipherKick::Util& util = CipherKick::Util::getInstance();
    try {
        util.openDbFile(argv[1], keystr);
    }
    catch (UtilException ue) {
        cout << endl << "Failed to open database:" << endl;
        cout << ue.what() << endl;
        return 1;
    }

    // TODO: Clear password string from memory

    cout << endl << endl << "Database decrypted -- you're in!";

    // Keep looping until exit.
    string command_str { "" };
    cout << endl << endl << "[s = search | n = new | q = quit]: ";
    getline(cin, command_str);
    while (true)
    {
        if (command_str.compare("q") == 0)
        {
            cout << endl << "Later skater!" << endl << endl;
            break;
        }
        /*else if (command_str.compare("n") == 0)
        {
            Site new_site;
            cout << "\n[name]: ";
            getline(cin, new_site.name);
            cout << "\n[url]: ";
            getline(cin, new_site.url);
            cout << "\n[user]: ";
            getline(cin, new_site.user);
            cout << "\n[password]: ";
            getline(cin, new_site.password);
            cout << "\n[notes]: ";
            getline(cin, new_site.notes);
            new_site.version = 1;

            // Get the current max site id.
            max_site_id = 0;
            string site_id_str = "select max(site_id) from sites;";
            if (sqlite3_exec(db, (const char*)site_id_str.c_str(), callback_max_site_id, NULL, NULL) != SQLITE_OK)
                cout << "\nError finding max site id.\n";
            max_site_id = max(0, max_site_id);

            string insert_str = "insert into sites (site_id, name, url, user, password, notes, version) "
                                        "values (" + to_string(max_site_id + 1) + ", '" + new_site.name + "', '" +
                                "" + new_site.url + "', '" + new_site.user + "', '" + new_site.password + "', '" +
                                "" + new_site.notes + "', " + to_string(new_site.version) + ");";
            if (sqlite3_exec(db, (const char *) insert_str.c_str(), NULL, NULL, NULL) != SQLITE_OK)
                cout << "\nError inserting site.\n";
            else
                cout << "\nSite added successfully.\n";
        }*/
        else if (command_str.compare("s") == 0)
        {
            string search_str("");
            cout << "\n[search string]: ";
            getline(cin, search_str);
            while (true)
            {
                if (search_str.compare("") == 0) break;
                else
                {
                    // Clear list of search results.
                    search_results->clear();

                    // Create a variable for the current site
                    std::shared_ptr<CipherKick::Site> current_site { nullptr };

                    try
                    {
                        search_results = util.search(search_str);
                    }
                    catch (UtilException ue)
                    {
                        cout << "An error occurred.\n";
                    }

                    if (search_results->size() == 0)
                    {
                        cout << "\nNo records found.\n";
                    }
                    else
                    {
                        if (search_results->size() == 1)
                        {
                            current_site = util.getSite(search_results->at(0).siteId);
                        }
                        else // search_results->size() > 1
                        {
                            cout << "\nRecords:\n";
                            for (std::vector<Site>::size_type i = 0; i != search_results->size(); i++)
                            {
                                cout << i + 1 << ". " << search_results->at(i).name << "\n";
                            }
                            
                            string number_command_str { "" };
                            int record_num { -1 };
                            cout << endl << "[record number to select]: ";
                            getline(cin, number_command_str);
                            while (true)
                            {
                                if (number_command_str.compare("") == 0) break;
                                try { record_num = std::stoi(number_command_str) - 1; }
                                catch (const std::invalid_argument& e) { }
                                catch (const std::out_of_range& e) { }
                                if (record_num >= 0 && record_num < search_results->size())
                                {
                                    current_site = util.getSite(search_results->at(record_num).siteId);
                                    break;
                                }
                                cout << endl << "[record number to select]: ";
                                getline(cin, number_command_str);
                            }
                        }

                        if (current_site) // a current site has been selected successfully
                        {
                            string detail_command_str("");
                            cout << endl << "[" << current_site->name << "]";
                            cout << endl << "[cu = copy user | cp = copy pw | v = view | e = edit]: ";
                            getline(cin, detail_command_str);
                            while (true)
                            {
                                if (detail_command_str.compare("") == 0) break;
                                else if (detail_command_str.compare("cu") == 0)
                                {
                                    // Copy the user to the clipboard.
                                    util.copyToClipboard(current_site->user);
                                    
                                    cout << endl << "User copied to the clipboard." << endl;
                                }
                                else if (detail_command_str.compare("cp") == 0)
                                {
                                    // Copy the password to the clipboard.
                                    util.copyToClipboard(current_site->password);

                                    cout << endl << "Password copied to the clipboard." << endl;
                                }
                                else if (detail_command_str.compare("v") == 0)
                                {
                                    cout << endl << "site_id = " << current_site->siteId;
                                    cout << endl << "name = " << current_site->name;
                                    cout << endl << "url = " << current_site->url;
                                    cout << endl << "user = " << current_site->user;
                                    cout << endl << "password = " << "**********";
                                    cout << endl << "notes = " << current_site->notes;
                                    cout << endl << "version = " << current_site->version;
                                    cout << endl;
                                }
                                /*
                                else if (detail_command_str.compare("e") == 0)
                                {
                                    site edited_site;
                                    cout << "\n[name]: ";
                                    edited_site.name = GetInputDefault(current_site.name, cout);
                                    cout << "\n[url]: ";
                                    edited_site.url = GetInputDefault(current_site.url, cout);
                                    cout << "\n[user]: ";
                                    edited_site.user = GetInputDefault(current_site.user, cout);
                                    cout << "\n[password]: ";
                                    edited_site.password = GetInputDefault(current_site.password, cout);
                                    cout << "\n[notes]: ";
                                    edited_site.notes = GetInputDefault(current_site.notes, cout);
                                    edited_site.site_id = current_site.site_id;
                                    edited_site.version = current_site.version + 1;
                                    cout << "\n";

                                    string insert_str = "insert into sites (site_id, name, url, user, password, notes, version) "
                                                                "values (" + to_string(edited_site.site_id) + ", '" + edited_site.name + "', '" +
                                                        "" + edited_site.url + "', '" + edited_site.user + "', '"
                                                                "" + edited_site.password + "', '" + edited_site.notes + "', " +
                                                        "" + to_string(edited_site.version) + ");";
                                    if (sqlite3_exec(db, (const char *) insert_str.c_str(), NULL, NULL, NULL) != SQLITE_OK)
                                    {
                                        cout << "\nError inserting site.\n";
                                    }
                                    else
                                    {
                                        cout << "\nSite edited successfully.\n";
                                    }
                                    break;
                                }
                                */
                                cout << endl << "[" << current_site->name << "]";
                                cout << "\n[cu = copy user | cp = copy pw | v = view | e = edit]: ";
                                getline(cin, detail_command_str);
                            }
                        }
                    }

                }
                cout << "\n[search string]: ";
                getline(cin, search_str);
            }
        }
        cout << "\n[s = search | n = new | q = quit]: ";
        getline(cin, command_str);
    }

    // Return success
    return 0;
}


// Turn stdin echo on or off
void setEcho(bool enable = true)
{
    // This section for windows
    #ifdef WIN32

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if (!enable) mode &= ~ENABLE_ECHO_INPUT;
    else mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode);

    // This section for unix / linux
    #else

    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);

    if (!enable) tty.c_lflag &= ~ECHO;
    else tty.c_lflag |= ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);

    #endif
}






