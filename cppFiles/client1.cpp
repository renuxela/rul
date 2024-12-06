#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <httplib.h>
#include <gumbo.h>

// Функция для поиска элемента по классу
std::string findElementByClass(GumboNode* node, const std::string& className) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return "";
    }

    GumboAttribute* classAttr = gumbo_get_attribute(&node->v.element.attributes, "class");
    if (classAttr && className == classAttr->value) {
        // Извлечение текстового содержимого
        if (node->v.element.children.length > 0) {
            GumboNode* child = static_cast<GumboNode*>(node->v.element.children.data[0]);
            if (child->type == GUMBO_NODE_TEXT) {
                return child->v.text.text;
            }
        }
    }

    // Рекурсивный поиск
    const GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        std::string result = findElementByClass(static_cast<GumboNode*>(children->data[i]), className);
        if (!result.empty()) {
            return result;
        }
    }

    return "";
}

// Функция для обработки "click" (отправка запроса)
void onSendButtonClick(const std::string& serverAddress, std::string& editableInput, int& page) {
    httplib::Client client(serverAddress.c_str());

    // Подготовка данных
    nlohmann::json dataToSend = {
        {"editableInput", editableInput},
        {"page", page}
    };

    auto res = client.Post("/api/checkButtonClick",
                           dataToSend.dump(),
                           "application/json");

    if (res && res->status == 200) {
        auto jsonData = nlohmann::json::parse(res->body);

        std::cout << "Ответ сервера:" << std::endl;
        std::cout << "ToF: " << jsonData["ToF"] << std::endl;
        std::cout << "Task: " << jsonData["task"] << std::endl;
        std::cout << "Rule: " << jsonData["rule"] << std::endl;

        // Обновление данных
        editableInput.clear();
        page = jsonData["page"];
    } else {
        std::cerr << "Ошибка при отправке данных" << std::endl;
    }
}

int main() {
    std::string serverAddress = "http://localhost:5500";

    // Загружаем HTML (симуляция)
    std::ifstream htmlFile("example.html");
    if (!htmlFile.is_open()) {
        std::cerr << "Не удалось открыть HTML-файл" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << htmlFile.rdbuf();
    std::string htmlContent = buffer.str();
    GumboOutput* output = gumbo_parse(htmlContent.c_str());

    // Ищем элементы
    std::string editableInput = findElementByClass(output->root, "editableInput");
    std::string sendButton = findElementByClass(output->root, "sendButton");
    std::string task = findElementByClass(output->root, "field__block-text");
    std::string rule = findElementByClass(output->root, "field__block-text");
    int page = 0;

    // Обработка события "click"
    onSendButtonClick(serverAddress, editableInput, page);

    // Освобождение ресурсов
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return 0;
}
