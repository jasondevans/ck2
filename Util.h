#ifndef CK2_UTIL_H
#define CK2_UTIL_H

#include "ck_common_includes.h"
#include "sqlite3.h"
#include "Site.h"
// #include "Timer.h"
// #include "Site.h"
// #include "Metainfo.h"


namespace CipherKick {

    class Util
    {

    private:

        Util();

    public:

        // Get our singleton instance
        static Util& getInstance() {
            static Util instance;
            return instance;
        }

        // Don't allow copy construction or assignment, as this is a singleton
        Util(Util const&) = delete;
        void operator=(Util const&) = delete;

        // Base64 characters.
        // std::string base64_chars;

        // A reference to our SQLite database.
        sqlite3 *db;

        // A timer.
        // Timer* timer;

        // Clean up.
        void cleanUp();

        // Attempt to open a database file.
        void openDbFile(const std::string& filePath, const std::string& password);

        // Set up a new database, creating tables and populating initial values.
        // void setupNewDb(String^ friendlyName);

        // Get metadata.
        // Metainfo^ getMetadata();

        // Get a list of all sites.
        // System::Collections::Generic::List<Site^>^ getSiteList();

        // Search for records with names like search string.
        std::shared_ptr<std::vector<CipherKick::Site>> search(const std::string& searchTerm);

        // Get a specific site.
        std::shared_ptr<CipherKick::Site> getSite(int siteId);

        // Save a site.
        // int saveSite(Site^ site);

        // Delete a site.
        // void deleteSite(int id);

        // Get export data as a string.
        // String^ getExportData(System::Security::SecureString^ encryptPassword);

        // Export to a file.
        // void exportData(String^ filePath, System::Security::SecureString^ encryptPassword);

        // Import a CSV.
        // void importLastPassCSV(String^ csvText);

        // Change master password.
        // void changeMasterPassword(System::Security::SecureString^ newPassword);

        // Update last modified date/time.
        // void updateLastModifiedTime();

        // Encode into base64.
        // std::string base64_encode(unsigned char const* buf, unsigned int bufLen);

        // Decode from base64 into bytes.
        // std::vector<unsigned char> base64_decode(std::string const& encoded_string);

        ///Returns -1 if string is valid. Invalid character is put to ch.
        // int getInvalidUtf8SymbolPosition(const unsigned char *input, unsigned char &ch);

        // Determine whether a given byte is a base64 character.
        /*static inline bool is_base64(unsigned char c) {
            return (isalnum(c) || (c == '+') || (c == '/'));
        }*/
    };

}


#endif //CK2_UTIL_H
