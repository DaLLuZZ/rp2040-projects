import express from "express";
import path from 'path'
import {fileURLToPath} from 'url';
import router from "./routes/auswertungen.js";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();
app.use(express.static(path.join(__dirname, 'public')));
app.use(express.json());
app.use(express.text());
app.use('/zeiten', router);

app.listen(3000, function () {
    console.log('App listening on port 3000!');
});
