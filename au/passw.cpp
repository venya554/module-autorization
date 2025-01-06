#include <iostream>
#include <string>
#include <bcrypt/BCrypt.hpp>

class PasswordManager {
    public:
        // Хэширование пароля
        std::string hashPassword(const std::string& password) {
            return BCrypt::generateHash(password);
        }
    
        // Проверка пароля
        bool checkPassword(const std::string& password, const std::string& hashedPassword) {
            return BCrypt::validatePassword(password, hashedPassword);
        }
    };
    
    int main() {
        PasswordManager pm;
    
        // Пример использования
        std::string password = "my_secure_password";
        
        // Хэширование пароля
        std::string hashedPassword = pm.hashPassword(password);
        std::cout << "Хэшированный пароль: " << hashedPassword << std::endl;
    
        // Проверка пароля
        std::string inputPassword;
        std::cout << "Введите пароль для проверки: ";
        std::cin >> inputPassword;
    
        if (pm.checkPassword(inputPassword, hashedPassword)) {
            std::cout << "Пароль верный!" << std::endl;
        } else {
            std::cout << "Пароль неверный!" << std::endl;
        }
    
        return 0;
    }
    