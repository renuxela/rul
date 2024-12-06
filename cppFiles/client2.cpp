#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <httplib.h>
#include <gumbo.h>

// Функция для поиска элементов по классу
std::vector<std::string> findElementsByClass(GumboNode* node, const std::string& className) {
    std::vector<std::string> elements;

    if (node->type != GUMBO_NODE_ELEMENT) {
        return elements;
    }

    GumboAttribute* classAttr = gumbo_get_attribute(&node->v.element.attributes, "class");
    if (classAttr && className == classAttr->value) {
        // Получаем текстовое содержимое элемента
        if (node->v.element.children.length > 0) {
            GumboNode* child = static_cast<GumboNode*>(node->v.element.children.data[0]);
            if (child->type == GUMBO_NODE_TEXT) {
                elements.push_back(child->v.text.text);
            }
        }
    }

    // Рекурсивный обход дочерних элементов
    const GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        auto childElements = findElementsByClass(static_cast<GumboNode*>(children->data[i]), className);
        elements.insert(elements.end(), childElements.begin(), childElements.end());
    }

    return elements;
}

// Функция для загрузки данных с сервера
nlohmann::json fetchFromServer(const std::string& serverAddress, const std::string& endpoint) {
    httplib::Client client(serverAddress.c_str());
    auto res = client.Get(endpoint.c_str());

    if (!res || res->status != 200) {
        throw std::runtime_error("Ошибка загрузки данных");
    }

    return nlohmann::json::parse(res->body);
}

// Основная функция firstPageOnLoad
void firstPageOnLoad(const std::string& serverAddress, GumboNode* htmlRoot) {
    try {
        // Запрос к серверу
        auto jsonData = fetchFromServer(serverAddress, "/api/firstPageOnLoad");

        // Обновляем HTML-страницу
        auto fieldTextElements = findElementsByClass(htmlRoot, "field__block-text");
        if (fieldTextElements.size() >= 2) {
            fieldTextElements[0] = jsonData["task"];
            fieldTextElements[1] = jsonData["rule"];
        }

        // Вывод для проверки
        std::cout << "Task: " << jsonData["task"] << std::endl;
        std::cout << "Rule: " << jsonData["rule"] << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

int main() {
    std::string serverAddress = "http://localhost:5500";

    // Загрузка HTML-документа
    std::ifstream htmlFile("example.html");
    if (!htmlFile.is_open()) {
        std::cerr << "Не удалось открыть HTML-файл" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << htmlFile.rdbuf();
    std::string htmlContent = buffer.str();

    // Парсинг HTML с помощью Gumbo
    GumboOutput* output = gumbo_parse(htmlContent.c_str());

    // Выполнение функции firstPageOnLoad
    firstPageOnLoad(serverAddress, output->root);

    // Освобождение ресурсов
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return 0;
}
