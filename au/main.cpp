#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <string>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class BackendServer {
public:
    BackendServer(const utility::string_t& url) {
        listener_ = http_listener(url);
        listener_.support(methods::POST, std::bind(&BackendServer::handle_post, this, std::placeholders::_1));
    }

    void start() {
        listener_.open().wait();
        std::wcout << L"Сервер запущен на " << listener_.uri().to_string() << std::endl;
    }

    void stop() {
        listener_.close().wait();
        std::wcout << L"Сервер остановлен." << std::endl;
    }

private:
    void handle_post(http_request request) {
        request.extract_json().then([=](json::value body) {
            // Обработка запроса
            std::wcout << L"Получен запрос: " << body.serialize() << std::endl;

            // Пример обработки запроса
            if (body.has_field(U("action"))) {
                utility::string_t action = body[U("action")].as_string();

                if (action == U("github_auth")) {
                    // Обработка авторизации через GitHub
                    json::value response;
                    response[U("status")] = json::value::string(U("success"));
                    response[U("message")] = json::value::string(U("GitHub auth successful"));
                    request.reply(status_codes::OK, response);
                } else if (action == U("yandex_auth")) {
                    // Обработка авторизации через Yandex
                    json::value response;
                    response[U("status")] = json::value::string(U("success"));
                    response[U("message")] = json::value::string(U("Yandex auth successful"));
                    request.reply(status_codes::OK, response);
                } else if (action == U("verify_code")) {
                    // Обработка проверки кода подтверждения
                    json::value response;
                    response[U("status")] = json::value::string(U("success"));
                    response[U("message")] = json::value::string(U("Code verified"));
                    request.reply(status_codes::OK, response);
                } else {
                    request.reply(status_codes::BadRequest, U("Invalid action"));
                }
            } else {
                request.reply(status_codes::BadRequest, U("Missing action"));
            }
        }).wait();
    }

    http_listener listener_;
};

int main() {
    // Преобразуем std::string в utility::string_t
    utility::string_t url = U("http://localhost:8080");
    BackendServer server(url);
    server.start();

    std::wcout << L"Нажмите Enter для остановки сервера..." << std::endl;
    std::cin.ignore();
    server.stop();

    return 0;
}