import numpy as np
import argparse
import os

parser = argparse.ArgumentParser(description='Convert raw audio file to paste-ready C array.')
parser.add_argument('ifilename', help='Input .raw file')
parser.add_argument('ofilename', nargs='?', default='c_injection.txt', help='Output .txt file (default: c_injection.txt)')
args = parser.parse_args()

data = np.fromfile(args.ifilename, dtype=np.uint8)
output_file = args.ofilename

with open(output_file, 'w') as file:
    file.write('const uint8_t portal_audio[] = {\n')
    file.write(', '.join(map(str, data)))
    file.write('\n};\n\n')
    file.write(f'const size_t portal_audio_size = {len(data)};\n')

print(f"Written to {output_file}")