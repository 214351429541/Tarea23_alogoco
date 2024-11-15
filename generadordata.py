import csv
import random
import string

# Función para generar cadenas aleatorias de longitud n
def generar_cadena(longitud):
    return ''.join(random.choices(string.ascii_lowercase, k=longitud))

# Crear dataset con cadenas vacías
def generar_dataset_cadenas_vacias():
    with open('dataset.csv', 'a', newline='') as csvfile:
        fieldnames = ['Tipo de Caso', 'Cadena 1', 'Cadena 2', 'Costo']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        
        # Escribir encabezado solo si el archivo está vacío
        csvfile.seek(0, 2)  # Ir al final del archivo
        if csvfile.tell() == 0:  # Si está vacío
            writer.writeheader()

        for _ in range(640):  # Generamos 100 casos
            cadena_no_vacia = generar_cadena(random.randint(1, 10))  # Cadena no vacía
            writer.writerow({'Tipo de Caso': 'Cadenas Vacías', 
                             'Cadena 1': "", 
                             'Cadena 2': cadena_no_vacia, 
                             'Costo': len(cadena_no_vacia)})

# Crear dataset con caracteres aleatorios
def generar_dataset_caracteres_aleatorios():
    with open('dataset.csv', 'a', newline='') as csvfile:
        fieldnames = ['Tipo de Caso', 'Cadena 1', 'Cadena 2', 'Costo']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        
        for _ in range(640):  # Generamos 100 casos
            longitud_1 = random.randint(5, 10)
            longitud_2 = random.randint(5, 10)
            cadena1 = generar_cadena(longitud_1)
            cadena2 = generar_cadena(longitud_2)
            writer.writerow({'Tipo de Caso': 'Caracteres Aleatorios', 
                             'Cadena 1': cadena1, 
                             'Cadena 2': cadena2})

# Crear dataset con caracteres repetidos
def generar_cadena_repetida(longitud):
    char = random.choice(string.ascii_lowercase)
    return char * longitud

def generar_dataset_caracteres_repetidos():
    with open('dataset.csv', 'a', newline='') as csvfile:
        fieldnames = ['Tipo de Caso', 'Cadena 1', 'Cadena 2', 'Costo']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        for _ in range(640):  # Generamos 100 casos
            longitud = random.randint(1, 10)  # Longitud aleatoria
            cadena_repetida = generar_cadena_repetida(longitud)
            writer.writerow({'Tipo de Caso': 'Caracteres Repetidos', 
                             'Cadena 1': cadena_repetida, 
                             'Cadena 2': cadena_repetida, 
                             'Costo': 0})

# Crear dataset con longitudes diferentes
def generar_dataset_longitudes_diferentes():
    with open('dataset.csv', 'a', newline='') as csvfile:
        fieldnames = ['Tipo de Caso', 'Cadena 1', 'Cadena 2', 'Costo']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        for _ in range(640):  # Generamos 100 casos
            longitud_1 = random.randint(5, 10)
            longitud_2 = random.randint(10, 15)
            cadena1 = generar_cadena(longitud_1)
            cadena2 = generar_cadena(longitud_2)
            writer.writerow({'Tipo de Caso': 'Longitudes Diferentes', 
                             'Cadena 1': cadena1, 
                             'Cadena 2': cadena2, 
                             'Costo': abs(longitud_1 - longitud_2)})

# Crear dataset con transposiciones
def generar_dataset_transposiciones():
    with open('dataset.csv', 'a', newline='') as csvfile:
        fieldnames = ['Tipo de Caso', 'Cadena 1', 'Cadena 2', 'Costo']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        for _ in range(640):  # Generamos 100 casos
            cadena = generar_cadena(random.randint(5, 10))  # Cadena aleatoria
            transpuesta = cadena[:2] + cadena[3] + cadena[2] + cadena[4:]  # Transposición de 2 caracteres
            writer.writerow({'Tipo de Caso': 'Transposiciones', 
                             'Cadena 1': cadena, 
                             'Cadena 2': transpuesta, 
                             'Costo': 1})

# Llamar a todas las funciones para generar el dataset completo
generar_dataset_cadenas_vacias()
generar_dataset_caracteres_aleatorios()
generar_dataset_caracteres_repetidos()
generar_dataset_longitudes_diferentes()
generar_dataset_transposiciones()
