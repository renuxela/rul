const initialization = require('./materialsParams.js');

class Controller {

    async firstPageOnLoad(req, res) {

        const materials = await initialization('Materials.json');
        const nounTests = materials.russianSections.morphology.independentParts.noun.tests;

        const firstTask = nounTests[0].pages[0].task;
        const firstRule = nounTests[0].pages[0].rule;

        const firstPageData = {
            task: firstTask,
            rule: firstRule,
        };

        res.json(firstPageData);

    }

}

module.exports = new Controller();

