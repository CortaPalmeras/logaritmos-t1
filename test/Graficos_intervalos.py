import numpy as np
import matplotlib.pyplot as plt

# Generar datos de altura aleatorios que siguen una distribución normal
np.random.seed(0)  # Para reproducibilidad
altura_media = 170  # Media de la altura en cm
desviacion_estandar = 10  # Desviación estándar de la altura en cm
num_muestras = 1000  # Número de muestras de altura

alturas = np.random.normal(loc=altura_media, scale=desviacion_estandar, size=num_muestras)

# Calcular el intervalo de confianza para la media de las alturas
media_muestral = np.mean(alturas)
error_estandar = desviacion_estandar / np.sqrt(num_muestras)  # Error estándar de la media
nivel_confianza = 0.95  # Nivel de confianza del 95%
z_score = 1.96  # Valor crítico para un intervalo de confianza del 95%

limite_inferior = media_muestral - z_score * error_estandar
limite_superior = media_muestral + z_score * error_estandar

# Graficar un histograma de las alturas
plt.hist(alturas, bins=30, density=True, color='skyblue', edgecolor='black')

# Graficar la campana (distribución normal) con la media y el intervalo de confianza
x = np.linspace(altura_media - 4 * desviacion_estandar, altura_media + 4 * desviacion_estandar, 100)
y = 1 / (desviacion_estandar * np.sqrt(2 * np.pi)) * np.exp(-0.5 * ((x - altura_media) / desviacion_estandar) ** 2)
plt.plot(x, y, color='red')

# Agregar líneas verticales para mostrar el intervalo de confianza
plt.axvline(limite_inferior, color='green', linestyle='--', label='Limite Inferior de IC (95%)')
plt.axvline(limite_superior, color='green', linestyle='--', label='Limite Superior de IC (95%)')

# Configurar título y etiquetas de los ejes
plt.title('Distribución de Alturas')
plt.xlabel('Altura (cm)')
plt.ylabel('Densidad de Probabilidad')

# Mostrar leyenda
plt.legend()

# Mostrar el gráfico
plt.show()
