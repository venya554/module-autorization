#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <iostream>
#include <string>

class MongoDB {
public:
    MongoDB(const std::string& uri, const std::string& db_name) {
        try {
            mongocxx::instance instance{}; // Инициализация драйвера
            client = mongocxx::client{mongocxx::uri{uri}};
            database = client[db_name];
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при подключении к MongoDB: " << e.what() << std::endl;
            throw;
        }
    }

    // Добавление пользователя
    bool addUser(const std::string& username, const std::string& password_hash) {
        try {
            auto collection = database["users"];
            bsoncxx::builder::stream::document document{};
            document << "username" << username
                     << "password_hash" << password_hash;
            auto result = collection.insert_one(document.view());
            return result ? true : false;
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при добавлении пользователя: " << e.what() << std::endl;
            return false;
        }
    }

    // Получение пользователя
    bsoncxx::document::value getUser(const std::string& username) {
        try {
            auto collection = database["users"];
            bsoncxx::builder::stream::document filter{};
            filter << "username" << username;
            auto result = collection.find_one(filter.view());
            return result ? *result : bsoncxx::document::value{};
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при получении пользователя: " << e.what() << std::endl;
            return bsoncxx::document::value{};
        }
    }

    // Удаление пользователя
    bool deleteUser(const std::string& username) {
        try {
            auto collection = database["users"];
            bsoncxx::builder::stream::document filter{};
            filter << "username" << username;
            auto result = collection.delete_one(filter.view());
            return result ? result->deleted_count() > 0 : false;
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при удалении пользователя: " << e.what() << std::endl;
            return false;
        }
    }

private:
    mongocxx::client client;
    mongocxx::database database;
};

// Пример использования
int main() {
    try {
        MongoDB db("mongodb://localhost:27017", "mydatabase");

        // Добавление пользователя
        if (db.addUser("testuser", "hashed_password")) {
            std::cout << "User added successfully." << std::endl;
        } else {
            std::cout << "Failed to add user." << std::endl;
        }

        // Получение пользователя
        auto user = db.getUser("testuser");
        if (!user.is_empty()) {
            std::cout << "User found: " << bsoncxx::to_json(user) << std::endl;
        } else {
            std::cout << "User not found." << std::endl;
        }

        // Удаление пользователя
        if (db.deleteUser("testuser")) {
            std::cout << "User deleted successfully." << std::endl;
        } else {
            std::cout << "Failed to delete user." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}