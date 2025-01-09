#include <iostream>
#include <string>
#include <bcrypt/BCrypt.hpp>
#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
#include <cstdlib> 
#include <ctime>   

class PasswordManager {
public:
    std::string hashPassword(const std::string& password) {
        return BCrypt::generateHash(password);
    }

    bool checkPassword(const std::string& password, const std::string& hashedPassword) {
        return BCrypt::validatePassword(password, hashedPassword);
    }

    void sendTelegramMessage(const std::string& token, const std::string& chat_id, const std::string& message) {
        web::http::client::http_client client(U("https://api.telegram.org"));

        // Формируем URI
        utility::string_t uri = U("/bot") + utility::conversions::to_string_t(token) + U("/sendMessage");

        web::http::http_request request(web::http::methods::POST);
        request.set_request_uri(uri);
        request.headers().set_content_type(U("application/json"));

        // Формируем тело запроса
        web::json::value json_body;
        json_body[U("chat_id")] = web::json::value::string(utility::conversions::to_string_t(chat_id));
        json_body[U("text")] = web::json::value::string(utility::conversions::to_string_t(message));
        request.set_body(json_body);

        client.request(request).then([](web::http::http_response response) {
            if (response.status_code() != web::http::status_codes::OK) {
                std::cerr << "Ошибка при отправке сообщения в Telegram: " << response.status_code() << std::endl;
            }
        }).wait();
    }

    int generateVerificationCode() {
        return rand() % 90000 + 10000; 
    }
};

int main() {
    PasswordManager pm;

    // Хэширование пароля
    std::string password = "my_secure_password";
    std::string hashedPassword = pm.hashPassword(password);
    std::cout << "Хэшированный пароль: " << hashedPassword << std::endl;

    // Проверка пароля
    std::string inputPassword;
    std::cout << "Введите пароль для проверки: ";
    std::cin >> inputPassword;

    if (pm.checkPassword(inputPassword, hashedPassword)) {
        std::cout << "Пароль верный!" << std::endl;

        // Генерация кода подтверждения
        srand(static_cast<unsigned int>(time(0))); 
        int verificationCode = pm.generateVerificationCode();
        std::cout << "Код подтверждения: " << verificationCode << std::endl;

        // Отправка кода подтверждения в Telegram
        std::string telegramToken = "YOUR_BOT_TOKEN"; // Замените на токен вашего бота
        std::string chatId = "YOUR_CHAT_ID"; // Замените на ваш chat_id
        pm.sendTelegramMessage(telegramToken, chatId, "Ваш код подтверждения: " + std::to_string(verificationCode));

        // Проверка кода подтверждения
        int inputCode;
        std::cout << "Введите код подтверждения: ";
        std::cin >> inputCode;

        if (inputCode == verificationCode) {
            std::cout << "Код подтверждения верный!" << std::endl;
        } else {
            std::cout << "Код подтверждения неверный!" << std::endl;
        }
    } else {
        std::cout << "Пароль неверный!" << std::endl;
    }

    return 0;
}