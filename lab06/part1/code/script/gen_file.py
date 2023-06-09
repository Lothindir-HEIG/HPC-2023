import random
import string
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-n','--num_lines', type=int, default=100)
parser.add_argument('-m','--line_length', type=int, default=None)
parser.add_argument('-o','--output_file', type=str, default='output.txt')
args = parser.parse_args()

if args.line_length is None:
    args.line_length = args.num_lines

def generate_random_line(length):
    line = ''.join(random.choices(string.ascii_letters, k=length))
    return line + '\n'

def generate_random_lines(file_path, num_lines, line_length):
    with open(file_path, 'w') as file:
        for _ in range(num_lines):
            line = generate_random_line(line_length)
            file.write(line)

if __name__ == '__main__':
    print('Generating file with {} lines of length {}...'.format(args.num_lines, args.line_length))
    generate_random_lines(args.output_file, args.num_lines, args.line_length)
