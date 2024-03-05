// Eine Datei, um den Multislider (für Einstellen des Anteils) passend zu den Getränkekarten zu stylen

/**
 * Funktion für Farben zum Stylen
 * @param i Index des Getränks (zwischen 1 und 6)
 * @return Farbe als css-Color und Hexadezimal-Wert
 */
function getSelectedCardColor(i) {
    switch (i) {
        case 1:
            return {
                css: "border-primary",
                color: "#0d6efd"
            };
        case 2:
            return {
                css: "border-success",
                color: "#198754"
            };
        case 3:
            return {
                css: "border-danger",
                color: "#d63444"
            };
        case 4:
            return {
                css: "border-warning",
                color: "#ffc107"
            };
        case 5:
            return {
                css: "border-info",
                color: "#0dcaf0"
            };
        case 6:
            return {
                css: "border-dark",
                color: "#212529"
            };
    }
}


/**
 *
 * @param container HTML-Container des Multisliders
 */
function adjustColoredRangePosition(container) {
    for (let i = 0; i < activeDrinks.length; i++) {
        let elem = container.querySelector(`.range-${i}`);
        let pointerLeft = container.querySelector('.pointer-' + (i - 1)) ? container.querySelector('.pointer-' + (i - 1)).getBoundingClientRect().left : container.getBoundingClientRect().left;
        let pointerRight = container.querySelector('.pointer-' + i) ? container.querySelector('.pointer-' + i).getBoundingClientRect().left : container.getBoundingClientRect().right;
        elem.style.left = (pointerLeft - container.getBoundingClientRect().left) + 'px';
        elem.style.width = (pointerRight - pointerLeft) + 'px';
    }
}

/**
 * Setzt Divs über den Multislider, um diesen passend zu färben
 * @param container HTML-Container des Multisliders
 */
function dyeSlider(container) {
    for (let i = 0; i < activeDrinks.length; i++) {
        let colorRange = document.createElement('div');
        colorRange.className = 'range-' + i;
        colorRange.style.backgroundColor = getSelectedCardColor(activeDrinks[i]).color;
        colorRange.style.height = "100%";
        colorRange.style.position = "absolute"
        colorRange.style.zIndex = 10;
        container.append(colorRange);
    }
    adjustColoredRangePosition(container);
}