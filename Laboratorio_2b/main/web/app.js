function setColor(color) {
    fetch(`/led/${color}`, {
        method: "POST"
    })
    .then(() => actualizarEstado());
}

function apagarLED() {
    fetch("/led/off", {
        method: "POST"
    })
    .then(() => actualizarEstado());
}

function actualizarEstado() {
    fetch("/led")
        .then(response => response.json())
        .then(data => {
            document.getElementById("estado").innerText =
                data.on ? "Encendido - " + data.color : "Apagado";
        });
}

setInterval(actualizarEstado, 1000);
actualizarEstado();
