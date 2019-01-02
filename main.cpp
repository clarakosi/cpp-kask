#include "cassandra.h"
#include "storage.h"
#include "crow.h"
#include <string>


int main() {

    Storage store;

    CassFuture* connect_future = NULL;
    CassCluster* cluster = cass_cluster_new();
    CassSession* session = cass_session_new();
    const char* hosts = "127.0.0.1";

    /* Add contact points */
    cass_cluster_set_contact_points(cluster, hosts);

    /* Provide the cluster object as configuration to connect the session */
    connect_future = cass_session_connect(session, cluster);

    if (cass_future_error_code(connect_future) != CASS_OK) {
        /* Handle error */
        const char* message;
        size_t message_length;
        cass_future_error_message(connect_future, &message, &message_length);
        fprintf(stderr, "Unable to connect: '%.*s'\n", (int)message_length, message);
    }

    crow::SimpleApp app;

    CROW_ROUTE(app, "/sessions/v1/<path>")
    .methods("GET"_method, "POST"_method, "DELETE"_method)
    ([&session, &store](const crow::request& req, std::string key){

        if (req.method == "GET"_method) {
            const char* value;
            CassError rc = store.get(session, key, value);;

            // TODO: Error handling, 404
            if (rc == CASS_OK) {
                return crow::response(200, value);
            }

        } else if (req.method == "POST"_method) {
            auto body = crow::json::load(req.body);
            if (!body) {
                return crow::response(400);
            }

            std::string value = body["value"].s();
            CassError rc = store.set(session, key, value);

            if (rc == CASS_OK) {
                return crow::response(201);
            }

            // TODO: Handle Error

        } else {
            return crow::response(404);
        }

    });

    app.port(5000).multithreaded().run();

    cass_future_free(connect_future);
    cass_cluster_free(cluster);
    cass_session_free(session);
    return 0;
}