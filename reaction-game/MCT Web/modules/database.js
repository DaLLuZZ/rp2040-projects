import {Sequelize, QueryTypes} from 'sequelize';
import path from "path";
import {fileURLToPath} from "url";

export default class Database {
    constructor(database) {
        if (typeof database === 'undefined') {
            throw new Error('Cannot be called directly');
        }
        this._database = database;
    }

    static async _init() {
        //Verbindung zur Datenbank
        const __filename = fileURLToPath(import.meta.url);
        const __dirname = path.dirname(__filename);
        const database = new Sequelize(
            {
                dialect: "sqlite",
                storage: path.join(__dirname, '..', 'db', 'MCT.db')
            }
        );
        //Testen der Verbindung
        try {
            await database.authenticate();
        } catch (error) {
            console.error('Fehler bei der Verbindung zur Datenbank:', error);
        }

        return database;
    }

    static async getInstance() {
        // Prüfen, ob Instanz bereits existiert
        if (!Database.instance) {
            // call _init
            let db = await this._init();
            // neue Instanz initialisieren
            Database.instance = new Database(db);
        }
        // Instanz zurückgeben
        return Database.instance;
    }

    //fügt Zeit einer Aktion hinzu
    async zeitEinfuegen(modus, zeit, spiel_ID) {
        await this._database.query("INSERT INTO Zeiten (Modus, Zeit, Spiel_ID) VALUES (:modus, :zeit, :spiel_ID)",
            {
                replacements: {
                    modus: modus,
                    zeit: zeit,
                    spiel_ID: spiel_ID
                },
                type: QueryTypes.INSERT
            }
        );
    }

    //fügt Spiel mit Startzeit hinzu
    async spielEinfuegen(startzeit) {
        await this._database.query("INSERT INTO Spiele (startzeit) VALUES (datetime(:startzeit))",
            {
                replacements: {
                    startzeit: startzeit,
                },
                type: QueryTypes.INSERT
            }
        );
    }

    //Gibt ID des Spiels zur genannten Startzeit zurück
    async getID(starttime) {
        const id = await this._database.query("SELECT ID FROM Spiele WHERE Startzeit LIKE (datetime(:starttime))",
            {
                replacements: {
                    starttime: starttime,
                },
                type: QueryTypes.SELECT
            }
        );
        return id;
    }

    // Gibt Zeiten der Spiele zurück
    async getGames() {
        const games = await this._database.query("SELECT Startzeit FROM Spiele",
            {
                type: QueryTypes.SELECT
            }
        );
        return games;
    }

    // Gibt Zeiten der Aktionen eines Spiels zurück
    async gibZeiten(spiel) {
        const zeiten = await this._database.query("SELECT Modus, Zeit FROM Zeiten INNER JOIN Spiele ON Zeiten.Spiel_ID = Spiele.ID WHERE Startzeit LIKE (:spiel)",
            {
                replacements: {
                    spiel: spiel,
                },
                type: QueryTypes.SELECT
            }
        );
        return zeiten;
    }
}
