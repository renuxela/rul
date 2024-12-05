const Router = require('express');

const controller = require('./controller');

const router = new Router();

router.get('/firstPageOnLoad', controller.firstPageOnLoad);

module.exports = router;