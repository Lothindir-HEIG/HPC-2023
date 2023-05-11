import matplotlib.pyplot as plt
import argparse

def f(x, maxperf, maxband):
    if x * maxband < maxperf:
        return x * maxband
    else:
        return maxperf

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='Roofline plotter',description='Generate roofline plot with FLOPS and OPS by default. Data is taken from input.txt file.')
    parser_show = parser.add_mutually_exclusive_group(required=False)
    parser.add_argument('name', type=str, help='Output file name')
    parser.add_argument('-t', '--title', type=str, help='Plot title', default='Roofline plot')
    parser.add_argument('-s', '--size', type=str, help='Plot size [x,y] (default 10,5)', default='10,5')
    parser.add_argument('-i', '--input', type=str, help='Input file', required=False)
    parser.add_argument('--uops', action='store_true', help='Show uOPS in graph')
    parser.add_argument('--ilp', type=int, help='ILP factor (default 2)', default=2)
    parser.add_argument('--times', action='store_true', help='Generate time plot')
    parser.add_argument('-l', '--linestyle-split', type=int, help='Threshold to split line styles (default 0)', default=0)
    parser.add_argument('-n', '--number', type=int, help='Number of segments to plot (default all)', default=-1)
    parser_show.add_argument('--no-ops', action='store_true', help='Do not show OPS in graph')
    parser_show.add_argument('--no-flops', action='store_true', help='Do not show FLOPS in graph')
    args = parser.parse_args()

    fig_x = int(args.size.split(',')[0])
    fig_y = int(args.size.split(',')[1])
    input_file = 'input.txt' if args.input is None else f'input_{args.input}.txt'

    with open(input_file, 'r') as file:
        lines = file.readlines()
        header_lines = 4
        maxperf_f = float(lines[0]) * args.ilp
        maxperf_u = float(lines[1]) * args.ilp
        maxperf_a = float(lines[2]) * args.ilp
        maxband = float(lines[3])
        if args.times:
            time_labels = lines[4].strip().split()
            header_lines += 1
        segments = [0]
        segments_labels = []
        points = []
        max_oi = 2
        if len(lines) > header_lines:
            for line in lines[header_lines:]:
                if line.strip() == '':
                    continue
                if line.startswith('#'):
                    if args.number > 0 and len(segments) - 1 >= args.number:
                        break
                    segments.append(len(points))
                    segments_labels.append(line.strip()[1:])
                    continue

                line_values = line.strip().split()
                if (len(line_values) < 3):
                    points.append((float(line_values[0]), float(line_values[1]), ''))
                elif len(line_values) < 4:
                    points.append((float(line_values[0]), float(line_values[1]), line_values[2]))
                else:
                    points.append((float(line_values[0]), float(line_values[1]), line_values[2], line_values[3]))

                if float(line_values[0]) > max_oi:
                    max_oi = int(float(line_values[0]))
            segments.append(len(points))

    if segments[1] == 0:
        del segments[1]
        
    plt.figure(figsize=(fig_x, fig_y))

    x_values = [x/10 for x in range(0, (max_oi + 1) * 10 + 1)]
    y_values_f = [f(x, maxperf_f, maxband) for x in x_values]
    y_values_u = [f(x, maxperf_u, maxband) for x in x_values]
    y_values_a = [f(x, maxperf_a, maxband) for x in x_values]

    if points != []:
        colors = ['orange', 'green', 'blue', 'cyan', 'magenta', 'yellow', 'grey']
        for s in range(0, len(segments) - 1):
            print(f'Plotting segment {s} [{segments[s]}:{segments[s + 1]}]')
            c = colors[s % len(colors)]
            p_x = []
            p_y = []
            for i, point in enumerate(points[segments[s]:segments[s + 1]]):
                plt.plot(point[0], point[1], color=c, marker='o')
                if not args.times:
                    label = f'{point[2]}'
                    plt.annotate(label, (point[0], point[1]), (point[0] + 0.1, point[1] + 0.1))
                elif len(point) > 3:
                    label = f'{point[3]}'
                    plt.annotate(label, (point[0], point[1]), (point[0], point[1] + 250))
                p_x.append(point[0])
                p_y.append(point[1])
            if s < args.linestyle_split:
                plt.plot(p_x, p_y, color=c, zorder=0, label=segments_labels[s], linestyle='--')
            else:
                plt.plot(p_x, p_y, color=c, zorder=0, label=segments_labels[s])

    secondary_linestyle = '-' if args.no_ops else '--'

    if not args.no_ops:
        plt.plot(x_values, y_values_a, color='red', linestyle='-', label='OPS', zorder=100)
    if not args.no_flops:
        plt.plot(x_values, y_values_f, color='blue', linestyle=secondary_linestyle, label='FLOPS')
    if args.uops:
        plt.plot(x_values, y_values_u, color='black', linestyle=secondary_linestyle, label='uOPS')
    plt.xlabel('Operational Intensity')
    plt.ylabel('Performance [MOPS/s]')
    plt.title(args.title)
    plt.legend()
    plt.savefig(f'{args.name}.png')
    # plt.show()

    if args.times:
        plt.clf()
        print("Generating time plot")
        plt.figure(figsize=(fig_x, fig_y))
        for s in range(0, len(segments_labels)):
            print(f'Plotting time segment {s} [{segments[s]}:{segments[s + 1]}]')
            c = colors[s % len(colors)]
            p_x = []
            p_y = []
            for i, point in enumerate(points[segments[s]:segments[s + 1]]):
                plt.plot(time_labels[i], float(point[2]), color=c, marker='o')
                p_x.append(i)
                p_y.append(float(point[2]))
            if s < args.linestyle_split:
                plt.plot(p_x, p_y, color=c, zorder=0, label=segments_labels[s], linestyle='--')
            else:
                plt.plot(p_x, p_y, color=c, zorder=0, label=segments_labels[s])
        plt.xlabel('Clusters')
        plt.ylabel('Time [s]')
        plt.title(args.title)
        plt.legend()
        plt.savefig(f'{args.name}_times.png')
        # plt.show()

