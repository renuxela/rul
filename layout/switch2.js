const circle1 = document.getElementById("circle1");

circle1.onclick = function() {
    window.scrollTo({
        top: window.innerHeight, 
        behavior: 'smooth' 
    });
}