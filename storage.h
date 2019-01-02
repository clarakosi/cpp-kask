#ifndef CPP_KASK2_STORAGE_H
#define CPP_KASK2_STORAGE_H

#include <string>
#include "cassandra.h"

class Storage
{
public:
    CassError get(CassSession* session, std::string key, const char* &output);
    CassError set(CassSession* session, std::string key, std::string value);
};

#endif //CPP_KASK2_STORAGE_H
