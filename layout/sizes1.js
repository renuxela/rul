window.onload = function() {
    const firstElem = document.getElementById("picture1").querySelector("img");
    const firstMargins = firstElem.getBoundingClientRect();
    const firstLeftMargin = firstMargins.left;

    const secondElems = document.getElementsByClassName("button-container");
    const secondElem = secondElems[0];
    const secondWidth = parseFloat(getComputedStyle(secondElem).width);
    const finishMargin = (firstLeftMargin - secondWidth) / 2;
    secondElem.style["margin-left"] = `${finishMargin}px`;
};
