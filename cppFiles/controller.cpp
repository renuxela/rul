#include <crow_all.h>
#include <nlohmann/json.hpp>
#include "materialsParams.h"

crow::response firstPageOnLoad() {
    try {
        auto materials = readJSON("Materials.json");
        auto nounTests = materials["russianSections"]["morphology"]["independentParts"]["noun"]["tests"];
        auto firstTask = nounTests[0]["pages"][0]["task"];
        auto firstRule = nounTests[0]["pages"][0]["rule"];

        nlohmann::json response;
        response["task"] = firstTask;
        response["rule"] = firstRule;

        return crow::response(response.dump());
    } catch (const std::exception& e) {
        return crow::response(500, e.what());
    }
}

crow::response checkButtonClick(const crow::request& req) {
    try {
        auto materials = readJSON("Materials.json");
        auto nounTests = materials["russianSections"]["morphology"]["independentParts"]["noun"]["tests"];
        auto test = nounTests[0];

        auto body = nlohmann::json::parse(req.body);
        int page = body["page"];
        std::string editableInput = body["editableInput"];

        auto isTestFinish = [&](int page, const nlohmann::json& test) -> bool {
            if (test["pages"].contains(page)) {
                return false;
            }
            nlohmann::json updatedData = {
                {"ToF", "Вы прошли тест!"},
                {"task", "Заданий больше нет."},
                {"rule", "Отправляйтесь к следующему тесту!"},
                {"page", page}
            };
            return true;
        };

        auto isCorrectAnswer = [&](int& page, const nlohmann::json& test) {
            if (editableInput == test["pages"][page]["answer"]) {
                ++page;
                if (!isTestFinish(page, test)) {
                    return crow::response(200);
                }
            } else {
                return crow::response(400);
            }
        };

        return crow::response(200);
    } catch (const std::exception& e) {
        return crow::response(500, e.what());
    }
}
