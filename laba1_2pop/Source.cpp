#include "Header.h"

int64_t nod(int64_t a, int64_t b) {
    while (b != 0) {
        a %= b;
        swap(a, b);
    }
    return a;
}

int64_t vzaim_prost(int64_t p) {
    random_device rd;
    ranlux24_base gen(rd());
    uniform_int_distribution<> dist(2, p);
    while (true) {
        int64_t k = dist(gen);
        if (nod(p, k) == 1) {
            return k;
        }

    }
}

int64_t obrat(int64_t b, int64_t a) {
    int64_t x = 1, y = 0, x1 = 0, y1 = 1, x2 = 0, y2 = 0, q = 0, r = 1, a1;
    a1 = a;

    while (r != 0) {
        q = a / b;
        r = a % b;
        x2 = x - q * x1;
        y2 = y - q * y1;
        x = x1;
        y = y1;
        x1 = x2;
        y1 = y2;
        a = b;
        b = r;

    }
    if (y < 0) {
        y = y + a1;
    }

    return y;
}

int64_t ost(int64_t a0, int64_t x0, int64_t p0) {
    int64_t a = a0, x = x0, q = 1, p = p0;
    while (x > 0) {
        if (x % 2 == 0) {
            x /= 2;
            a = (a * a) % p;
        }
        else {
            x--;
            q = (a * q) % p;
        }
    }
    return q;
}

int64_t ost_with_numb(int64_t Y, int64_t k, int64_t M, int64_t p) {
    int64_t q, q0;
    q = Y % p;
    q0 = q;
    for (int i = 1; i < k; i++) {
        q = (q * q0) % p;
    }
    q = (q * M) % p;
    return q;
}

int64_t gen_prost() {
    random_device rd;
    ranlux24_base gen(rd());
    uniform_int_distribution<> dist(10000, 400000);
    while (true) {
        int64_t p = dist(gen);
        int64_t i;
        for (i = 2; i < p / 2; i++) {
            if (p % i == 0) {
                break;
            }
        }
        if (i == p / 2) {
            return p;
        }
    }
}

int64_t gen_numb(int64_t p) {
    random_device rd;
    ranlux24_base gen(rd());
    uniform_int_distribution<> dist(2,p-1);
    int64_t t = dist(gen);
    return t;
}
