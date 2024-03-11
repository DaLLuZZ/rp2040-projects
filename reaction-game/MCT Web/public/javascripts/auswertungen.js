document.addEventListener("DOMContentLoaded", async function (event) {
    let spiel = document.getElementById('spiel');
    let spiele = await fetch("/zeiten/spiele",
        {
            method: "GET",
        }
    );
    spiele = await spiele.json(); //Konvertierung in Format zum besseren Verarbeiten
    for (const zeile of spiele) {
        const option = document.createElement('option'); //erstellt Option
        option.innerHTML = zeile.Startzeit;
        spiel.append(option);
    }
    let div = document.getElementById('diagramm');
    spiel.addEventListener('change', async function (event) {
        const gewaehltesSpiel = spiel.value;
        let daten = await fetch("/zeiten/spiele/" + gewaehltesSpiel,
            {
                method: "GET",
            }
        );

        daten = await daten.json(); //Konvertierung in Format zum besseren Verarbeiten
        var xValue = []; //Aktionen
        var yValue = []; //Zeiten
        for (zeile of daten) {
            xValue.push(zeile.Modus);
            yValue.push(zeile.Zeit / 1000);
        }

        var trace1 = {
            x: xValue,
            y: yValue,
            type: 'bar',
            text: yValue.map(String),
            textposition: 'auto',
            hoverinfo: 'none',
            opacity: 0.5,
            marker: {
                color: 'rgb(225,0,0)',
                line: {
                    color: 'rgb(8,48,107)',
                    width: 1.5
                }
            }
        };

        var data = [trace1];

        var layout = {
            title: gewaehltesSpiel
        };

        Plotly.newPlot('diagramm', data, layout);
    });
});