# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.

import subprocess
import re
import numpy as np


def main():
    m = 100
    for methodIndex in range(18):
        for n in (10, 32, 100, 316, 1000, 3162, 10000):
            data = []
            for i in range(100):
                stdout = subprocess.run(['x64\Release\exectime.exe', str(methodIndex), str(m), str(n)],
                                        stdout=subprocess.PIPE).stdout.decode('utf-8')
                tokens = re.findall(r'(\[.+\]): ([\.\d]+)', stdout)[0]
                data.append(float(tokens[1]))
            print(methodIndex, str(n) + 'x' + str(n), tokens[0], np.mean(data), np.std(data))


if __name__ == '__main__':
    import sys

    sys.exit(main())
