let activeDrinks = [];

/**
 *
 * @param slider primäres HTML-Element des Sliders
 * @return {Element} Container des Multisliders im shadowRoot
 */
function getContainerFromSlider(slider) {
    return slider.shadowRoot.querySelector('.container');
}

/**
 * Funktion zum Hinzufügen von Getränken --> Färbt Getränkekarte und initalisiert den Multislider
 * @param idx Index des hinzuzufügenden Getränks
 */
function addDrink(idx) {
    let card = document.getElementById("drink" + idx);
    if (activeDrinks.includes(idx)) {
        card.classList.remove(getSelectedCardColor(idx).css);
        activeDrinks = activeDrinks.filter((item) => {
            return item !== idx;
        });
    } else {
        card.classList.add(getSelectedCardColor(idx).css);
        activeDrinks.push(idx);
    }
    activeDrinks.sort((a, b) => {
        return a - b
    })
    // Der Slider muss entfernt und neu initialisiert werden, weil sich dynamisch zur Laufzeit keine Pointer entfernen hinzufügen lassen
    document.querySelector('tc-range-slider')?.remove();
    let slider = document.createElement('tc-range-slider');
    let wrapper = document.getElementById('verhaeltnis');
    slider.setAttribute('slider-bg', "#cbd5e1");
    slider.setAttribute('slider-bg-fill', "#cbd5e1");
    const shadow = slider.shadowRoot;
    for (let i = 0; i < activeDrinks.length; i++) {
        slider.setAttribute('value' + i, (100 / activeDrinks.length) * i);
    }
    if (activeDrinks.length === 0) {
        slider.setAttribute("disabled", "true");
    }
    // fügt jedem Pointer in dem Slider ein Label mit der Prozentzahl hinzu
    slider.setAttribute("theme", "circle");
    slider.setAttribute("css-links", "stylingPointer.css");
    slider.setAttribute("step", "0.5"); // or false
    slider.setAttribute("moving-tooltip", "true"); // or false
    slider.setAttribute("moving-tooltip-distance-to-pointer", "-40"); // px
    slider.setAttribute("moving-tooltip-width", "37");
    slider.setAttribute("moving-tooltip-height", "35"); // px
    slider.setAttribute("moving-tooltip-bg", '#5e5e5e');
    slider.setAttribute("moving-tooltip-text-color", '#efefef');
    slider.setAttribute("moving-tooltip-units", '%');
    slider.formatTooltipValue = (value) => {
        return Number(value).toLocaleString('en-US', {minimumFractionDigits: 1});
    };
    wrapper.append(slider);
    // stylen des Multisliders
    dyeSlider(getContainerFromSlider(wrapper.querySelector('tc-range-slider')));
    // EventListener, um Styling aktuell zu halten
    document.querySelector('tc-range-slider').addEventListener('change', (ev) => {
        adjustColoredRangePosition(getContainerFromSlider(ev.target));
    })
}

function getVolumen() {
    return document.getElementById('volumen').value;
}

/**
 * Sendet einen Request mit den relevanten Daten an den ESP32
 */
function sendGetraenk() {
    const buttonStart = document.querySelector('button');
    // Set Request Options
    const xhr = new XMLHttpRequest();
    const url = "mixtur";
    xhr.open("POST", url, true);
    xhr.setRequestHeader("Content-Type", "application/json");
    const drinks = [];
    let slider = document.querySelector('tc-range-slider');
    // Fügt dem zu übertragenden Array Daten hinzu
    for (let i = 1; i <= activeDrinks.length; i++) {
        drinks.push({
            "drink": activeDrinks[i - 1],
            "anteil": (getVolumen() * ((slider['value' + i] ? slider['value' + i] : 100) - (i === 1 ? 0 : slider['value' + (i - 1)]))) / 100
        });
    }
    const data = JSON.stringify({
        "getraenk": drinks
    });
    // Absenden des Requests mit Fehlerbehandlung via Status-Code
    xhr.send(data);
    xhr.onreadystatechange = function () {
        if (this.readyState === 4 && this.status === 200) {
        } else if (this.readyState === 4 && this.status === 404) {
            alert('Es konnte keine Verbindung zum Server hergestellt werden!');
        } else if (this.readyState === 4 && this.status === 423) {
            alert('Es wird bereits ein Getränk gemischt');
        }
    }
}