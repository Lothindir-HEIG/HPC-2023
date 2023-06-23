import matplotlib.pyplot as plt

import numpy as np

# Reference table data
reference_table = {
    'tiny': {'Lignes': 100, 'Colonnes': 1000, 'Taille': '10 KB', 'Temps': 1.6},
    'small': {'Lignes': 1000, 'Colonnes': 1000, 'Taille': '1 MB', 'Temps': 9},
    'medium': {'Lignes': 1000, 'Colonnes': 10000, 'Taille': '10 MB', 'Temps': 81.2},
    'big': {'Lignes': 10000, 'Colonnes': 10000, 'Taille': '100 MB', 'Temps': 798.5},
    'huge': {'Lignes': 10000, 'Colonnes': 100000, 'Taille': '1 GB', 'Temps': 8400}
}

# Improvements table data
improvements_table = [
    {'Fichier': 'tiny', 'Buffer': 1024, 'Temps': 1.7},
    {'Fichier': 'tiny', 'Buffer': 2048, 'Temps': 1.6},
    {'Fichier': 'tiny', 'Buffer': 4096, 'Temps': 1.5},
    {'Fichier': 'tiny', 'Buffer': 8192, 'Temps': 1.5},
    {'Fichier': 'tiny', 'Buffer': 16384, 'Temps': 1.4},
    {'Fichier': 'tiny', 'Buffer': 32768, 'Temps': 1.4},
    {'Fichier': 'tiny', 'Buffer': 65536, 'Temps': 1.3},
    {'Fichier': 'tiny', 'Buffer': 131072, 'Temps': 1.3},
    {'Fichier': 'tiny', 'Buffer': 2147483647, 'Temps': 1.4},
    {'Fichier': 'tiny', 'Buffer': 4294967295, 'Temps': 1.4},
    {'Fichier': 'small', 'Buffer': 1024, 'Temps': 5.9},
    {'Fichier': 'small', 'Buffer': 2048, 'Temps': 4.6},
    {'Fichier': 'small', 'Buffer': 4096, 'Temps': 4},
    {'Fichier': 'small', 'Buffer': 8192, 'Temps': 3.4},
    {'Fichier': 'small', 'Buffer': 16384, 'Temps': 3.2},
    {'Fichier': 'small', 'Buffer': 32768, 'Temps': 3},
    {'Fichier': 'small', 'Buffer': 65536, 'Temps': 2.9},
    {'Fichier': 'small', 'Buffer': 131072, 'Temps': 3.1},
    {'Fichier': 'small', 'Buffer': 2147483647, 'Temps': 3.5},
    {'Fichier': 'small', 'Buffer': 4294967295, 'Temps': 3.5},
    {'Fichier': 'medium', 'Buffer': 1024, 'Temps': 47.5},
    {'Fichier': 'medium', 'Buffer': 2048, 'Temps': 35.3},
    {'Fichier': 'medium', 'Buffer': 4096, 'Temps': 28.2},
    {'Fichier': 'medium', 'Buffer': 8192, 'Temps': 23.9},
    {'Fichier': 'medium', 'Buffer': 16384, 'Temps': 21.7},
    {'Fichier': 'medium', 'Buffer': 32768, 'Temps': 20.5},
    {'Fichier': 'medium', 'Buffer': 65536, 'Temps': 21.1},
    {'Fichier': 'medium', 'Buffer': 131072, 'Temps': 21.2},
    {'Fichier': 'medium', 'Buffer': 2147483647, 'Temps': 23.8},
    {'Fichier': 'medium', 'Buffer': 4294967295, 'Temps': 22.1},
    {'Fichier': 'big', 'Buffer': 1024, 'Temps': 463},
    {'Fichier': 'big', 'Buffer': 2048, 'Temps': 339},
    {'Fichier': 'big', 'Buffer': 4096, 'Temps': 272},
    {'Fichier': 'big', 'Buffer': 8192, 'Temps': 227},
    {'Fichier': 'big', 'Buffer': 16384, 'Temps': 205},
    {'Fichier': 'big', 'Buffer': 32768, 'Temps': 194},
    {'Fichier': 'big', 'Buffer': 65536, 'Temps': 199},
    {'Fichier': 'big', 'Buffer': 131072, 'Temps': 200},
    {'Fichier': 'big', 'Buffer': 2147483647, 'Temps': 207},
    {'Fichier': 'big', 'Buffer': 4294967295, 'Temps': 206},
    {'Fichier': 'huge', 'Buffer': 1024, 'Temps': 4700},
    {'Fichier': 'huge', 'Buffer': 2048, 'Temps': 3400},
    {'Fichier': 'huge', 'Buffer': 4096, 'Temps': 2800},
    {'Fichier': 'huge', 'Buffer': 8192, 'Temps': 2300},
    {'Fichier': 'huge', 'Buffer': 16384, 'Temps': 2100},
    {'Fichier': 'huge', 'Buffer': 32768, 'Temps': 2000},
    {'Fichier': 'huge', 'Buffer': 65536, 'Temps': 1967},
    {'Fichier': 'huge', 'Buffer': 131072, 'Temps': 2005},
    {'Fichier': 'huge', 'Buffer': 2147483647, 'Temps': 2000},
    {'Fichier': 'huge', 'Buffer': 4294967295, 'Temps': 2000}
]

# # Extracting reference table data
# files = list(reference_table.keys())
# times = [reference_table[file]['Temps'] for file in files]

# # Extracting improvements table data
# speedups = []
# for row in improvements_table:
#     file = row['Fichier']
#     ref_time = reference_table[file]['Temps']
#     speedup = ref_time / row['Temps']
#     speedups.append(speedup)

# Plot scatter plot with buffer size as x-axis and speedup as y-axis for each file


# Prepare data for plotting
data = []
for row in improvements_table:
    file = row['Fichier']
    buffer_size = row['Buffer']
    ref_time = reference_table[file]['Temps']
    speedup = ref_time / row['Temps']
    data.append((file, buffer_size, speedup))

# Separate data by file
files = set([row[0] for row in data])
file_data = {file: [] for file in files}
for row in data:
    file_data[row[0]].append(row[1:])

# Plotting the graph
colors = ['b', 'g', 'r', 'c', 'm']  # Color for each file
markers = ['o', 's', 'D', 'v', '^']  # Marker for each file

fig, ax = plt.subplots(figsize=(15, 7))

for file, file_color, file_marker in zip(files, colors, markers):
    buffer_sizes = [row[0] for row in file_data[file]]
    speedups = [row[1] for row in file_data[file]]
    ax.scatter(buffer_sizes, speedups, c=file_color, marker=file_marker, label=file)
    # Plot variance
    ax.plot(buffer_sizes, speedups, c=file_color, alpha=0.5)

ax.set_xlabel('Buffer Size (bytes)')
ax.set_xticks([1024, 2048, 4096, 8192, 16384, 32768, 4294967295])
ax.set_xscale('log', base=2)
ax.set_ylabel('Speedup (times))')
ax.set_title('Relative Speedup vs. Buffer Size')
ax.legend()
ax.grid(True)
plt.savefig('speedup.png')