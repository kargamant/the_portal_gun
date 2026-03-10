import numpy as np

data = np.fromfile('portal_gun_sound.raw', dtype=np.uint8)

with open("c_injection.txt", 'w') as file:
    file.write('const uint8_t portal_audio[] = {\n')
    file.write(', '.join(map(str, data)))
    file.write('\n};\n\n')
    file.write(f'const size_t portal_audio_size = {len(data)};\n')
