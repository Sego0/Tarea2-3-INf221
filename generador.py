import random
import string

def generate_test_files(size):
    # Función auxiliar para generar una cadena aleatoria
    def random_string(length):
        return ''.join(random.choices(string.ascii_lowercase, k=length))

    # Generar cadenas similares unas a otras
    similar_strings = []
    for _ in range(10):
        base_string = random_string(size)
        modified = list(base_string)
        # Realizar varios cambios aleatorios (por ejemplo, entre 1 y size // 4 cambios)
        num_changes = random.randint(1, max(1, size // 4))
        for _ in range(num_changes):
            index = random.randint(0, len(modified) - 1)
            modified[index] = random.choice(string.ascii_lowercase)  # Cambiar un carácter
        similar_strings.append((base_string, ''.join(modified)))

    # Generar cadenas con caracteres repetidos
    repeated_strings = []
    for _ in range(10):
        char = random.choice(string.ascii_lowercase)
        modified = [char] * size
        # Introducir varios cambios (por ejemplo, entre 1 y size // 3 cambios)
        num_changes = random.randint(1, max(1, size // 3))
        for _ in range(num_changes):
            index = random.randint(0, size - 1)
            modified[index] = random.choice(string.ascii_lowercase)
        repeated_strings.append((''.join([char] * size), ''.join(modified)))

    # Generar cadenas donde la transposición es necesaria
    transposable_strings = []
    for _ in range(10):
        base_string = random_string(size)
        if size > 1:
            used = []
            modified = list(base_string)
            num_changes = random.randint(1, max(1, size // 2))
            for _ in range(num_changes):
                index = random.randint(0, size - 2)  # Elegir un par adyacente para transponer
                while index in used:
                    index = random.randint(0, size - 2)
                used.append(index)
                used.append(index+1)
                modified[index], modified[index + 1] = modified[index + 1], modified[index]  # Transponer caracteres
            transposable_strings.append((base_string, ''.join(modified)))
        else:
            transposable_strings.append((base_string, base_string))  # Para tamaño 1 no se puede transponer

    # Generar cadenas completamente distintas
    distinct_strings = []
    for _ in range(10):
        distinct_strings.append((random_string(size), random_string(size)))

    # Escribir las cadenas en archivos
    def write_to_file(filename, pairs):
        with open(filename, 'w') as f:
            for s1, s2 in pairs:
                f.write(f"{s1},{s2}\n")

    write_to_file("similar_strings_"+str(size)+".txt", similar_strings)
    write_to_file("repeated_strings_"+str(size)+".txt", repeated_strings)
    write_to_file("transposable_strings_"+str(size)+".txt", transposable_strings)
    write_to_file("distinct_strings_"+str(size)+".txt", distinct_strings)

# Llamar a la función con el tamaño deseado
n = 1000
generate_test_files(n)
