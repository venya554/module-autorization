#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
#include <iostream>
#include <string>

class YandexOAuth {
public:
    YandexOAuth(const std::string& client_id, const std::string& client_secret)
        : client_id(client_id), client_secret(client_secret) {}

    void authorize() {
        std::string auth_url = "https://oauth.yandex.ru/authorize?response_type=code&client_id=" + client_id;
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
        try {
            web::http::client::http_client client(U("https://oauth.yandex.ru"));

            web::http::http_request request(web::http::methods::POST);
            request.set_request_uri(U("/token"));
            request.headers().set_content_type(U("application/x-www-form-urlencoded"));

            // Формируем тело запроса
            std::string body = "grant_type=authorization_code"
                               "&client_id=" + client_id +
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

                // Парсинг JSON-ответа
                auto json_response = web::json::value::parse(response_string);

                // Проверка наличия ключа "access_token"
                if (json_response.has_field(U("access_token"))) {
                    // Получение токена доступа
                    std::string access_token = utility::conversions::to_utf8string(json_response[U("access_token")].as_string());
                    std::cout << "Получен токен доступа: " << access_token << std::endl;
                } else {
                    std::cerr << "Ключ 'access_token' отсутствует в ответе." << std::endl;
                }
            }).wait();
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }
};

int main() {
    std::string client_id = "Ov23lipM6scFzPXEutdA"; // Замените на ваш Client ID
    std::string client_secret = "78c48c3395c0b7446b3a720d225703db397b2ae1"; // Замените на ваш Client Secret

    YandexOAuth yandex(client_id, client_secret);
    yandex.authorize();

    return 0;
}