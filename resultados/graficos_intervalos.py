import numpy as np
import matplotlib.pyplot as plt



def leer_accesos(arboles_accesos):
    with open("accesos_ss.txt", "r") as f:
        for linea in f:
            if linea[0] == "a":
                arboles_accesos.append(list())
            else:
                arboles_accesos[-1].append(int(linea))


def leer_tiempos(arboles_tiempos):
    with open("tiempos_ss.txt", "r") as f:
        for linea in f:
            if linea[0] == "a":
                
                arboles_tiempos.append(list())
            else:
                arboles_tiempos[-1].append(float(linea))



def crear_histograma(actual, lista, tipo):
    datos = lista

    min_value = min(datos)
    max_value = max(datos)
    bins = 'auto'

    plt.hist(datos, bins=bins, color="skyblue", edgecolor="black")

    plt.title(f"Histograma de {tipo} de árbol {actual}")
    plt.xlabel(f"{tipo}")
    plt.ylabel("Frecuencia")

    plt.savefig(f"grafico_{tipo}_del_arbol_{actual}.png")
    plt.close()


def crear_intervalo_confianza(actual, lista, tipo):
    datos = lista

    media = np.mean(datos)
    desviacion = np.std(datos)
    inferior = media - 1.96 * desviacion
    superior = media + 1.96 * desviacion

    x = np.linspace(media - 4 * desviacion, media + 4 * desviacion, 100)
    y = (1 / (desviacion * np.sqrt(2 * np.pi))) * np.exp(-0.5 * ((x - media) / desviacion) ** 2)

    plt.plot(x, y, color="red", label="Distribución Normal")

    plt.axvline(inferior, color='blue', linestyle='--', label='Límite Inferior IC')
    plt.axvline(superior, color='blue', linestyle='--', label='Límite Superior IC')

    plt.fill_between(x, 0, y, where=(x >= inferior) & (x <= superior), color='lightblue', alpha=0.5)

    plt.title(f"Distribución de {tipo} con Intervalo de Confianza")
    plt.xlabel(f"{tipo}")
    plt.ylabel("Densidad de Probabilidad")
    plt.legend()
    plt.savefig(f"grafico {tipo} del arbol {actual}")
    plt.close()



def crear_graficos():
    arboles_accesos = []
    arboles_tiempos = []
    leer_accesos(arboles_accesos)
    leer_tiempos(arboles_tiempos)
    actual = 10
    for i in range(0, 7):
        crear_histograma(actual, arboles_accesos[i], "accesos")
        crear_histograma(actual, arboles_tiempos[i], "tiempos")
        crear_intervalo_confianza(actual, arboles_accesos[i], "accesos")
        crear_intervalo_confianza(actual, arboles_tiempos[i], "tiempos")
        actual += 1


crear_graficos()