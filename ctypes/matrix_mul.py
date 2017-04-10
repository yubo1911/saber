# -*- coding: utf-8 -*-
import ctypes
import itertools


class MatrixMul(object):
    def __init__(self):
        self._libc = ctypes.cdll.LoadLibrary('./matrix_mul.so')

    def mul(self, ma, mb):
        l = len(ma)
        m = len(ma[0])
        n = len(mb[0])
        flat_ma = itertools.chain(*ma)
        flat_mb = itertools.chain(*mb)
        cma = (ctypes.c_int * (l * m))(*flat_ma)
        cmb = (ctypes.c_int * (m * n))(*flat_mb)
        cmres = (ctypes.c_int * (l * n))(*([0] * (l * n)))
        self._libc.mul(cmres, cma, cmb, ctypes.c_int(l), ctypes.c_int(m),
                       ctypes.c_int(n))
        mres = []
        for i in range(l):
            mres.append(cmres[i*n:(i+1)*n])
        return mres


if __name__ == '__main__':
    ma = [[0, 1, 2],
          [2, 3, 4]]
    mb = [[1, 2],
          [3, 4],
          [5, 6]]
    mm = MatrixMul()
    mres = mm.mul(ma, mb)
    print(mres)

    import time
    ma = [[1]*100] * 100
    mb = [[1]*100] * 100
    st = time.time()
    mres = mm.mul(ma, mb)
    print('ctypes used {}'.format(time.time() - st))
    st = time.time()
    mres = [[0]*100]*100
    for i in range(100):
        for j in range(100):
            s = 0
            for k in range(100):
                s += ma[i][k] * mb[k][j]
            mres[i][j] = s
    print('raw python used {}'.format(time.time() - st))
