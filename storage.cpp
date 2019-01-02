#include "storage.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include "cassandra.h"


CassError Storage::get(CassSession* session, std::string key, const char* &output)
{

    // TODO: Make keyspace + table dynamic.

    /* Build statement and execute query */
    const char* query = "SELECT * FROM mw.sessions WHERE key= ?";
    CassStatement* statement = cass_statement_new(query, 1);

    cass_statement_bind_string(statement, 0, key.c_str());

    CassFuture* result_future = cass_session_execute(session, statement);

    CassError rc = cass_future_error_code(result_future);

    if (rc == CASS_OK) {
        /* Retrieve result set and get the first row */
        const CassResult* result = cass_future_get_result(result_future);
        const CassRow* row = cass_result_first_row(result);

        if (row) {
            const CassValue* value = cass_row_get_column_by_name(row, "value");

            const char* result_value;
            size_t result_value_length;
            cass_value_get_string(value, &result_value, &result_value_length);

            char temp[100];
            snprintf(temp, 100, "%.*s", (int)result_value_length, result_value);
            output = temp;
        }

        cass_result_free(result);
    }

    cass_future_free(result_future);
    cass_statement_free(statement);
    
    return rc;
}

CassError Storage::set(CassSession *session, std::string key, std::string value)
{
    // TODO: Make keyspace + table dynamic.

    /* Build statement and execute query */
    const char* query = "INSERT INTO mw.sessions (key, value) VALUES (?, ?)";
    CassStatement* statement = cass_statement_new(query, 2);

    cass_statement_bind_string(statement, 0, key.c_str());
    cass_statement_bind_string(statement, 1, value.c_str());

    CassFuture* result_future = cass_session_execute(session, statement);

    CassError rc = cass_future_error_code(result_future);


    cass_future_free(result_future);
    cass_statement_free(statement);

    return rc;
}
