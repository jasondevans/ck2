
#ifndef CK2_METAINFO_H
#define CK2_METAINFO_H


#include "ck_common_includes.h"

namespace CipherKick {
    public ref class Metainfo
    {

    public:

        System::String^ guid;
        System::String^ friendlyName;
        System::String^ lastModifiedUtc;

        property System::String^ Guid {
            System::String^ get() { return guid; }
            void set(System::String^ _guid) { guid = _guid; }
        }
        property System::String^ FriendlyName {
            System::String^ get() { return friendlyName; }
            void set(System::String^ _friendlyName) { friendlyName = _friendlyName; }
        }
        property System::String^ LastModifiedUtc {
            System::String^ get() { return lastModifiedUtc; }
            void set(System::String^ _lastModifiedUtc) { lastModifiedUtc = _lastModifiedUtc; }
        }

        // Constructor.
        Metainfo();

        // Destructor.
        virtual ~Metainfo();

    };
}




#endif //CK2_METAINFO_H
