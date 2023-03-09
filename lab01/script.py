import argparse
from PIL import Image

def resize_and_convert_to_binary(input_file, output_file, new_size):
    with Image.open(input_file) as im:
        im = im.convert('1') 
        im = im.resize((new_size, new_size))
        data = im.load()

        with open(output_file, 'w') as f:
            for y in range(new_size):
                for x in range(new_size):
                    if data[x, y] == 255:
                        f.write('0')
                    else:
                        f.write('1')
                f.write('\n')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Convertit une image en noir et blanc et redimensionne pour produire un fichier binaire.')
    parser.add_argument('image', help='Chemin vers le fichier image.')
    parser.add_argument('taille', type=int, help='Taille désirée en pixels pour l\'image carrée de sortie.')
    parser.add_argument('-o', '--output', default='output.txt', help='Chemin pour le fichier binaire de sortie. (default: %(default)s)')

    args = parser.parse_args()

    resize_and_convert_to_binary(args.image, args.output, args.taille)
