import numpy as np
import matplotlib.pyplot as plt

arboles_accesos = []
arboles_tiempos = []


def leer_accesos():
    with open("", "r") as f:
        actual = -1
        for linea in f:
            if linea[0] == "a":
                actual += 1
                arboles_accesos += []
            else:
                arboles_accesos[actual] += linea


def leer_tiempos():
    with open("archivo_tiempos", "r") as f:
        actual = -1
        for linea in f:
            if linea[0] == "a":
                actual += 1
                arboles_tiempos += []
            else:
                arboles_tiempos[actual] += linea


def crear_histograma(actual, lista, tipo):
    datos = lista[actual - 10]

    plt.hist(datos, bins=10, color="skyblue", edgecolor="black")

    plt.title(f"Histograma de {tipo} de arbol ")
    plt.xlabel(f"{tipo}")
    plt.ylabel("Frecuencia")

    plt.savefig(f"grafico {tipo} del arbol {actual}")


def crear_intervalo_confianza(actual, lista, tipo):
    media = np.mean(lista[actual - 10])
    desviacion = np.std(lista[actual - 10])
    error = desviacion / np.sqrt(100)
    confianza = 0.95
    inferior = media - 1.96 * error
    superior = media + 1.96 * error
    x = np.linspace(media - 4 * desviacion, media + 4 * desviacion, 100)
    y = (
        1
        / (desviacion * np.sqrt(2 * np.pi))
        * np.exp(-0.5 * ((x - media) / desviacion) ** 2)
    )
    plt.plot(x, y, color="red")
    plt.title(f"Distribucion de {tipo}")
    plt.xlabel(f"{tipo}")
    plt.ylabel("Densidad de Probabilidad")
    plt.legend()
    plt.savefig(f"grafico {tipo} del arbol {actual} ")


def crear_graficos():
    leer_accesos()
    leer_tiempos()
    for i in range(0, 15):
        actual = 10
        crear_histograma(actual, arboles_accesos, "accesos")
        crear_histograma(actual, arboles_tiempos, "tiempos")
        crear_intervalo_confianza(actual, arboles_accesos, "accesos")
        crear_intervalo_confianza(actual, arboles_tiempos, "tiempos")
        actual += 1
