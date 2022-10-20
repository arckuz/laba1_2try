#include "Header.h"

class SHAMIR {
public:
    int64_t p;
    int64_t m;
    int64_t x1;
    int64_t x2;

    void gen_p() {
        p = gen_prost();
    }

    void a_calc() {
        a = vzaim_prost(p - 1);
    }

    void a_obr_calc() {
        a_obr = obrat(a, p - 1);
    }

    void x1_calc(int64_t inc_m) {
        x1 = ost(inc_m, a, p);
    }

    void x2_calc(int64_t inc_x) {
        x2 = ost(inc_x, a_obr, p);
    }

private:
    int64_t a;
    int64_t a_obr;
};

class RSA {
public:
    int64_t n;
    int64_t e;
    int64_t c;

    void gen_p() {
        p = gen_prost();
    }
    void gen_q() {
        q = gen_prost();
    }
    void calc_n() {
        n = p * q;
    }
    void calc_f() {
        f = (p - 1) * (q - 1);
    }
    void calc_e() {
        e = vzaim_prost(f);
    }
    void calc_d() {
        d = obrat(e, f);
    }
    void calc_c(int m_inc, int e_inc, int n_inc) {
        c = ost(m_inc, e_inc, n_inc);
    }
    int64_t calc_dec_m(int64_t c) {
        dec_m = ost(c, d, n);
        return dec_m;
    }

private:
    int64_t p;
    int64_t q;
    int64_t f;
    int64_t d;
    int64_t dec_m;
};

class DH {
public:
    int64_t p;
    int64_t g;
    int64_t open_key;

    void gen_p() {
        p = gen_prost();
    }
    void gen_g() {
        g = gen_numb(p);
    }
    void gen_secret_key() {
        secret_key = gen_numb(p);
    }
    void calc_open_key() {
        open_key = ost(g, secret_key, p);
    }
    void calc_key(int64_t OpenKeyFromU) {
        key = ost(OpenKeyFromU, secret_key, p);
    }
    int64_t get_key() {
        return key;
    }
private:
    int64_t secret_key;
    int64_t key;

};

class EG {
public:
    int64_t m_dec;
    int64_t p;
    int64_t g;
    int64_t a;
    int64_t b;
    int64_t Y;

    void gen_p() {
        p = gen_prost();
    }
    void gen_g() {
        g = vzaim_prost(p);
    }
    void gen_x() {
        x = gen_numb(p);
    }
    void gen_k(int64_t p_inc) {
        k = vzaim_prost(p_inc-1);
    }
    void calc_Y() {
        Y = ost(g, x, p);
    }
    void calc_a(int64_t g_inc, int64_t p_inc) {
        a = ost(g_inc, k, p_inc);
    }
    void calc_b(int m, int64_t Y_inc, int64_t p_inc) {
        b = ost_with_numb(Y_inc, k, m, p_inc);
    }
    void calc_m_dec(int64_t a_inc, int64_t b_inc) {
        m_dec = ost_with_numb(a_inc, p - 1 - x, b_inc, p);
    }

private:
    int64_t k;
    int64_t x;
};

void shamir(string str) {
    SHAMIR sh_Alice;
    SHAMIR sh_Bob;
    string str_dec = "";
    sh_Alice.gen_p(); //Алиса выбрала p
    sh_Bob.p = sh_Alice.p; //Алиса передает p бобу
    cout << "Зашифрованное сообщение: ";
    for (auto i : str) {
        int m = int(i);
        sh_Alice.a_calc(); //Алиса считает са
        sh_Alice.a_obr_calc(); //Алиса считает da
        sh_Bob.a_calc(); //Боб считает cb
        sh_Bob.a_obr_calc(); //Боб считает db
        sh_Alice.x1_calc(m);  //Алиса считает x1
        sh_Bob.x1_calc(sh_Alice.x1); //боб считает х1
        sh_Alice.x2_calc(sh_Bob.x1); //Алиса считает х2
        cout << sh_Alice.x2 << " ";
        sh_Bob.x2_calc(sh_Alice.x2); //Боб считает x2
        str_dec += char(sh_Bob.x2);
    }
    cout << "\nРасшифрованное сообщение: " << str_dec;
}

void rsa(string str) {
    RSA rsa_Alice;
    RSA rsa_Bob;
    string str_dec = "";

    rsa_Alice.gen_p(); //Алиса выбрала р
    rsa_Alice.gen_q(); //Алиса выбрала q
    rsa_Alice.calc_n();
    rsa_Alice.calc_f();
    rsa_Alice.calc_e();
    rsa_Alice.calc_d();
    cout << "Зашифрованное соощение: ";
    for (auto i : str) {
        int m = int(i);
        rsa_Bob.calc_c(m, rsa_Alice.e, rsa_Alice.n);
        cout << rsa_Bob.c << " ";
        str_dec += char(rsa_Alice.calc_dec_m(rsa_Bob.c));
    }
    cout << "\nРасшифрованное сообщение: " << str_dec;
}

void dh(string str) {
    DH dh_Alice;
    DH dh_Bob;
    string str_enc = "";

    dh_Alice.gen_p();
    dh_Alice.gen_g();
    dh_Alice.gen_secret_key();
    dh_Alice.calc_open_key();

    dh_Bob.p = dh_Alice.p;
    dh_Bob.g = dh_Alice.g;
    dh_Bob.gen_secret_key();
    dh_Bob.calc_open_key();

    dh_Alice.calc_key(dh_Bob.open_key);
    dh_Bob.calc_key(dh_Alice.open_key);

    cout << "Зашифрованное сообщение: ";
    for (auto i : str) {
        str_enc += char(int(i) + dh_Alice.get_key() % 10);
        cout << char(int(i) + dh_Alice.get_key() % 10);
    }
    cout << "\nРасшифрованное сообщение: ";
    for (auto i : str_enc) {
        cout << char(int(i) - dh_Bob.get_key() % 10);
    }
}

void eg(string str) {

    EG eg_Alice;
    EG eg_Bob;
    string str_dec;

    eg_Alice.gen_p();
    eg_Alice.gen_g();
    eg_Alice.gen_x();
    eg_Alice.calc_Y();

    cout << "Зашифрованное сообщение: ";
    for (auto i : str) {

        int m = int(i);
        eg_Bob.gen_k(eg_Alice.p);
        eg_Bob.calc_a(eg_Alice.g, eg_Alice.p);
        eg_Bob.calc_b(m, eg_Alice.Y, eg_Alice.p);
        cout << "(" << eg_Bob.a << "," << eg_Bob.b << "), ";       
        eg_Alice.calc_m_dec(eg_Bob.a, eg_Bob.b);
        str_dec += char(eg_Alice.m_dec);
    }
    cout << "\nРасшифрованное сообщение: " << str_dec;
}

int main()
{
    int num;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Выберите что хотите сделать: ";
    cout << "\n1.Шамир";
    cout << "\n2.RSA";
    cout << "\n3.Диффи-Хеллман";
    cout << "\n4.Эль-Гамаль" << endl;
    cin >> num;
    cin.get();
    string str;
    cout << "Введите сообщение для шифровки: ";
    getline(cin, str);
    switch (num)
    {
    case 1:
        shamir(str);
        break;
    case 2:
        rsa(str);
        break;
    case 3:
        dh(str);
        break;
    case 4:
        eg(str);
        break;
    default:
        break;
    }
}
