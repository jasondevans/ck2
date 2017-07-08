#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "sqlite3.h"

using namespace std;


struct site
{
	int id;
	int site_id;
	string name;
	string url;
	string user;
	string password;
	string notes;
	int version;
} current_site;

vector<string> search_results;
int max_site_id;


string GetInputDefault(const std::string &defaultin, std::ostream &out = std::cout);


// Process a search result.
static int callback_search_result(void *NotUsed, int argc, char **argv, char **azColName)
{
	string this_result(argv[0]);
	search_results.push_back(this_result);
	return 0;
}


// Process the max site id query.
static int callback_max_site_id(void *NotUsed, int argc, char **argv, char **azColName)
{
	if (argv[0]) max_site_id = stoi(argv[0]);
	else max_site_id = 0;
	return 0;
}


// Process a record retrieval.
static int callback_retrieve_record(void *NotUsed, int argc, char **argv, char **azColName)
{
	site this_site;
	for (int i = 0; i < argc; i++)
	{
		string this_col_name(azColName[i]);
		if (this_col_name.compare("id") == 0) this_site.id = stoi(argv[i]);
		else if (this_col_name.compare("site_id") == 0) this_site.site_id = stoi(argv[i]);
		else if (this_col_name.compare("name") == 0) this_site.name = argv[i];
		else if (this_col_name.compare("url") == 0) this_site.url = argv[i];
		else if (this_col_name.compare("user") == 0) this_site.user = argv[i];
		else if (this_col_name.compare("password") == 0) this_site.password = argv[i];
		else if (this_col_name.compare("notes") == 0) this_site.notes = argv[i];
		else if (this_col_name.compare("version") == 0) this_site.version = stoi(argv[i]);
	}
	current_site = this_site;
	return 0;
}



int _tmain(int argc, _TCHAR* argv[])
{
	// Turn off input echoing.
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

	// Open the database.
	sqlite3 *db;
	if (sqlite3_open("C:\\Users\\jason\\Documents\\Projects\\ProjectRake\\ProjectRake.db", &db) != SQLITE_OK)
	{
		printf("Failed to open database.\n");
		_getch();
		return -1;
	}
		
	// Set encryption key.
	string keystr;
	cout << "Welcome to Project Rake!\n\nPlease enter key string: ";
	getline(cin, keystr);
	string pragma_str = "PRAGMA key = '" + keystr + "'";
	if (sqlite3_exec(db, (const char*)pragma_str.c_str(), NULL, NULL, NULL) != SQLITE_OK)
	{
		printf("Failed to set encryption key.\n");
		_getch();
		sqlite3_close(db);  //close it up properly
		return -1;
	};

	// Turn echo back on.
	SetConsoleMode(hStdin, mode);

	// Keep looping until exit.
	string command_str("");
	cout << "\n\n[s = search | n = new | q = quit]: ";
	getline(cin, command_str);
	while (true)
	{
		if (command_str.compare("q") == 0) break;
		else if (command_str.compare("n") == 0)
		{
			site new_site;
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
		}
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
					search_results.clear();

					string query_str = "select s.name from sites s, "
						"(select site_id, max(version) as max_version from sites group by site_id) v "
						"where s.site_id = v.site_id and s.version = v.max_version "
						"and lower(s.name) like '%" + search_str + "%' "
						"order by lower(s.name);";
					// string query_str = "select distinct name from sites where lower(name) like '%" + search_str + "%';";
					if (sqlite3_exec(db, (const char*)query_str.c_str(), callback_search_result, NULL, NULL) != SQLITE_OK)
					{
						cout << "An error occurred.\n";
					}

					if (search_results.size() == 0)
					{
						cout << "\nNo records found.\n";
					}
					else
					{
						cout << "\nRecords:\n";
						for (std::vector<site>::size_type i = 0; i != search_results.size(); i++)
						{
							cout << i + 1 << ". " << search_results[i] << "\n";
						}

						if (search_results.size() == 1)
						{
							query_str = "select id, site_id, name, url, user, password, notes, version "
								"from sites where name = '" + search_results[0] + "' and "
								"version in "
								"(select max(version) from sites where name = '" + search_results[0] + "')";
							if (sqlite3_exec(db, (const char*)query_str.c_str(), callback_retrieve_record, NULL, NULL) != SQLITE_OK)
							{
								cout << "An error occurred.\n";
							}
							string detail_command_str("");
							cout << "\n[cu = copy user | cp = copy pw | v = view | e = edit]: ";
							getline(cin, detail_command_str);
							while (true)
							{
								if (detail_command_str.compare("") == 0) break;
								else if (detail_command_str.compare("cu") == 0)
								{
									// Copy the password to the clipboard.
									const char* output = current_site.user.c_str();
									const size_t len = strlen(output) + 1;
									HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
									memcpy(GlobalLock(hMem), output, len);
									GlobalUnlock(hMem);
									OpenClipboard(0);
									EmptyClipboard();
									SetClipboardData(CF_TEXT, hMem);
									CloseClipboard();
								}
								else if (detail_command_str.compare("cp") == 0)
								{
									// Copy the password to the clipboard.
									const char* output = current_site.password.c_str();
									const size_t len = strlen(output) + 1;
									HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
									memcpy(GlobalLock(hMem), output, len);
									GlobalUnlock(hMem);
									OpenClipboard(0);
									EmptyClipboard();
									SetClipboardData(CF_TEXT, hMem);
									CloseClipboard();
								}
								else if (detail_command_str.compare("v") == 0)
								{
									cout << "\nid = " << current_site.id;
									cout << "\nsite_id = " << current_site.site_id;
									cout << "\nname = " << current_site.name;
									cout << "\nurl = " << current_site.url;
									cout << "\nuser = " << current_site.user;
									cout << "\npassword = " << current_site.password;
									cout << "\nnotes = " << current_site.notes;
									cout << "\nversion = " << current_site.version;
									cout << "\n";
								}
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

	sqlite3_close(db);  //close it up properly

	return 0;

} // end _tmain



string GetInputDefault(const std::string &defaultin, std::ostream &out)
{
	std::vector<char> inputvec(defaultin.begin(), defaultin.end());
	out << defaultin;

	while (true)
	{
		int in = _getch(); // Get keycode
		switch (in)
		{
		case 13: // Enter
			return std::string(inputvec.begin(), inputvec.end());
		case 8:  // Backspace
			if (inputvec.size() != 0)
			{
				inputvec.pop_back(); // Remove last character entered
				out << "\b \b" << std::flush; // Remove from screen
			}
			break;
		default:
			inputvec.push_back(in); // Insert character
			out << static_cast<char>(in) << std::flush; // Print
		}
	}
}


