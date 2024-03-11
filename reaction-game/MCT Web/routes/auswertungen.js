import Database from '../modules/database.js';
import express from 'express';

const router = express.Router();

// POST neue Aktion mit Zeit
router.post('/', async function (req, res) {
    let body = req.body;
    const modus = body.modus;
    const zeit = body.zeit;
    const spielID = body.game_ID;
    await Database.getInstance().then((db) => db.zeitEinfuegen(modus, zeit, spielID))
    res.statusMessage = "Erfolgreich hinzugefuegt";
    res.status(200).end();
});

// POST neues Spiel
router.post('/spiel', async function (req, res) {
    let body = req.body;
    const starttime = body.starttime;
    await Database.getInstance().then((db) => db.spielEinfuegen(starttime))
    res.statusMessage = "Erfolgreich hinzugefuegt";
    res.send(await Database.getInstance().then((db) => db.getID(starttime)));
});

// GET Zeiten mit Spielzeit
router.get('/spiele/:spielzeit', async function (req, res) {
    let spielzeit = req.params.spielzeit;
    res.type('application/json');
    res.send(await Database.getInstance().then((db) => db.gibZeiten(spielzeit)));
});

// GET Startzeiten der Spiele
router.get('/spiele', async function (req, res) {
    res.type('application/json');
    res.send(await Database.getInstance().then((db) => db.getGames()));
});

export default router