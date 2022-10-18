#include "Header.h"
class SHAMIR {
public:
	int64_t p;
    int64_t m;


    void gen_p() {
        p = gen_prost();
    }

    void ca_calc() {
        ca = vzaim_prost(p - 1);
    }
    void cb_calc() {
        cb = vzaim_prost(p - 1);
    }

    void da_calc() {
        da = obrat(ca, p - 1);
    }
    void db_calc() {
        db = obrat(cb, p - 1);
    }

    void x1_calc() {
        x1 = ost(m, ca, p);
    }
    void x2_calc() {
        x2 = ost(x1, cb, p);
    }
    void x3_calc() {
        x3 = ost(x2, da, p);
    }
    void x4_calc() {
        x4 = ost(x3, db, p);
    }

    int64_t get_x1() {
        return x1;
    }
    int64_t get_x2() {
        return x2;
    }
    int64_t get_x3() {
        return x3;
    }
    int64_t get_x4() {
        return x4;
    }

    void set_x1(int64_t t){
        x1 = t;
    }
    void set_x2(int64_t t) {
        x2 = t;
    }
    void set_x3(int64_t t) {
        x3 = t;
    }

private:
    int64_t ca;
    int64_t da;
    int64_t cb;
    int64_t db;
    int64_t x1;
    int64_t x2;
    int64_t x3;
    int64_t x4;

};

class RSA {
public:
    int64_t m;
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
    void calc_c() {
        c = ost(m, e, n);
    }
    int64_t calc_dec_m() {
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
    int64_t A;
    int64_t B;

    void gen_p() {
        p = gen_prost();
    }
    void gen_g() {
        g = gen_numb(p);
    }
    void gen_a() {
        a = gen_numb(p);
    }
    void gen_b() {
        b = gen_numb(p);
    }
    
    void calc_A() {
        A = ost(g, a, p);
    }
    void calc_B() {
        B = ost(g, b, p);
    }
    void calc_k_a() {
        k_a = ost(B, a, p);
    }
    void calc_k_b() {
        k_b = ost(A, b, p);
    }

    int64_t get_k_a() {
        return k_a;
    }
    int64_t get_k_b() {
        return k_b;
    }
private:
    int64_t a;
    int64_t b;
    int64_t k_a;
    int64_t k_b;

};

class EG {
public:
    int64_t m;
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
        g = gen_numb(p);
    }
    void gen_x() {
        x = gen_numb(p);
    }
    void gen_k() {
        k = vzaim_prost(p-1);
    }
    void calc_Y() {
        Y = ost(g, x, p);
    }
    void calc_a() {
        a = ost(g, k, p);
    }
    void calc_b() {
        b = ost_with_numb(Y, k, m, p);
    }
    void calc_m_dec() {
        m_dec = ost_with_numb(a, p - 1 - x, b, p);
    }

private:
    int64_t k;
    int64_t x;
};

void shamir(string str) {
    SHAMIR sh_Alice;
    SHAMIR sh_Bob;
    string str_dec="";
    sh_Alice.gen_p(); //Алиса выбрала p
    sh_Bob.p = sh_Alice.p; //Алиса передает p бобу
    cout << "Зашифрованное сообщение: ";
    for (auto i : str) {
        sh_Alice.m = int(i);
        sh_Alice.ca_calc(); //Алиса считает са
        sh_Alice.da_calc(); //Алиса считает da
        sh_Bob.cb_calc(); //Боб считает cb
        sh_Bob.db_calc(); //Боб считает db
        sh_Alice.x1_calc();  //Алиса считает x1
        sh_Bob.set_x1(sh_Alice.get_x1()); //Алиса передает Бобу x1
        sh_Bob.x2_calc(); //боб считает х2
        sh_Alice.set_x2(sh_Bob.get_x2()); //Боб передает Алисе х2
        sh_Alice.x3_calc(); //Алиса считает х3
        cout << sh_Alice.get_x3()<<" ";
        sh_Bob.set_x3(sh_Alice.get_x3()); // Алиса передает Бобу x3
        sh_Bob.x4_calc(); //Боб считает x4
        str_dec += char(sh_Bob.get_x4());
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
    rsa_Bob.n = rsa_Alice.n;
    rsa_Bob.e = rsa_Alice.e;
    cout << "Зашифрованное соощение: ";
    for (auto i : str) {
        rsa_Bob.m = int(i);
        rsa_Bob.calc_c();
        cout << rsa_Bob.c << " ";
        rsa_Alice.c = rsa_Bob.c;
        str_dec += char(rsa_Alice.calc_dec_m());
    }
    cout << "\nРасшифрованное сообщение: " << str_dec;
}

void dh(string str) {
    DH dh_Alice;
    DH dh_Bob;
    string str_enc = "";
    dh_Alice.gen_p();
    dh_Alice.gen_g();
    dh_Alice.gen_a();

    dh_Bob.p = dh_Alice.p;
    dh_Bob.g = dh_Alice.g;
    dh_Bob.gen_b();

    dh_Alice.calc_A();
    dh_Bob.calc_B();

    dh_Alice.B = dh_Bob.B;
    dh_Bob.A = dh_Alice.A;

    dh_Alice.calc_k_a();
    dh_Bob.calc_k_b();
    cout << "Зашифрованное сообщение: ";
    for (auto i : str) {
        str_enc += char(int(i) + dh_Alice.get_k_a() % 10);
        cout << char(int(i) + dh_Alice.get_k_a()%10);
    }
    cout << "\nРасшифрованное сообщение: ";
    for (auto i : str_enc) {
        cout << char(int(i) - dh_Bob.get_k_b() % 10);
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

    eg_Bob.p = eg_Alice.p;
    eg_Bob.g = eg_Alice.g;
    eg_Bob.Y = eg_Alice.Y;

    cout << "Зашифрованное сообщение: ";
    for (auto i : str) {

        eg_Bob.m = int(i);
        eg_Bob.gen_k();
        eg_Bob.calc_a();
        eg_Bob.calc_b();
        cout << "(" << eg_Bob.a << "," << eg_Bob.b << "), ";
        eg_Alice.a = eg_Bob.a;
        eg_Alice.b = eg_Bob.b;        
        eg_Alice.calc_m_dec();
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
