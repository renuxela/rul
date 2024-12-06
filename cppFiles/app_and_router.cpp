#include "crow_all.h"
#include <fstream>
#include <nlohmann/json.hpp>

void startServer() {
    crow::SimpleApp app;

    // Маршруты API
    app.route_dynamic("/api/firstPageOnLoad").methods("GET"_method)([](const crow::request& req) {
        return crow::response(404); // Заглушка
    });

    app.route_dynamic("/api/checkButtonClick").methods("POST"_method)([](const crow::request& req) {
        return crow::response(404); // Заглушка
    });

    // Статические файлы
    app.mount("/layout", crow::mustache::load_directory("../layout"));
    app.mount("/client", crow::mustache::load_directory("../client"));

    // Запуск сервера
    app.port(5500).multithreaded().run();
}

int main() {
    startServer();
    return 0;
}
