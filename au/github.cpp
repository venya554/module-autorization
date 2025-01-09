#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
#include <iostream>
#include <string>

class GitHubOAuth {
public:
    GitHubOAuth(const std::string& client_id, const std::string& client_secret)
        : client_id(client_id), client_secret(client_secret) {}

    void authorize() {
        std::string auth_url = "https://github.com/login/oauth/authorize?client_id=" + client_id;
        std::cout << "Перейдите по следующему URL для авторизации: " << auth_url << std::endl;
        std::cout << "Введите код авторизации: ";
        std::string code;
        std::cin >> code;

        getAccessToken(code);
    }

private:
    std::string client_id;
    std::string client_secret;

    void getAccessToken(const std::string& code) {
        web::http::client::http_client client(U("https://github.com"));

        web::http::http_request request(web::http::methods::POST);
        request.set_request_uri(U("/login/oauth/access_token"));
        request.headers().set_content_type(U("application/x-www-form-urlencoded"));

        // Формируем тело запроса в формате x-www-form-urlencoded
        std::string body = "client_id=" + client_id +
                           "&client_secret=" + client_secret +
                           "&code=" + code;
        request.set_body(body);

        client.request(request).then([](web::http::http_response response) {
            if (response.status_code() == web::http::status_codes::OK) {
                return response.extract_string();
            }
            throw std::runtime_error("Ошибка при получении токена доступа: " + std::to_string(response.status_code()));
        }).then([](const utility::string_t& response_string) {
            // Обработка ответа
            std::wcout << L"Ответ от сервера: " << response_string << std::endl;
        }).wait();
    }
};

int main() {
    std::string client_id = "Ov23lipM6scFzPXEutdA"; // Замените на ваш Client ID
    std::string client_secret = "Y78c48c3395c0b7446b3a720d225703db397b2ae1"; // Замените на ваш Client Secret

    GitHubOAuth github(client_id, client_secret);
    github.authorize();

    return 0;
}