const fs = require("fs");
const path = require("path");

async function initialization() {

    async function asyncExport() {
        const param = await require('D:/rul/server/materialsParams.js');
        return param;    
    }

    const exported = await asyncExport();
    const nounTests = await exported.getNounTest();
    console.log(`JSON was parsed like ${nounTests}`);
    return nounTests; 
}

class Controller {

    async firstPageOnLoad(req, res) {

        const nounTests = await initialization();

        const firstPageData = {
            question_text: nounTests[0].pages[0].question,
            rule_text: nounTests[0].pages[0].rule,
        };

        res.json(firstPageData);

    }

    async checkButtonClick(req, res) {

    }

}

module.exports = new Controller();