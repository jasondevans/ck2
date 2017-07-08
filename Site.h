#ifndef CK2_SITE_H
#define CK2_SITE_H

#include "ck_common_includes.h"

namespace CipherKick {

    class Site
    {

    public:

        int id;
        int siteId;
        std::string name;
        std::string url;
        std::string user;
        std::string password;
        std::string notes;
        int version;
        std::string canonicalUrl2;
        std::string canonicalUrl3;
        std::string canonicalName1;
        std::string canonicalName2;
        std::string canonicalName3;
        std::string canonicalName4;

        // Constructor.
        Site();

        // Destructor.
        virtual ~Site();

        // Clear site (reset all values to defaults.
        virtual void clear();

    };
}


#endif //CK2_SITE_H
