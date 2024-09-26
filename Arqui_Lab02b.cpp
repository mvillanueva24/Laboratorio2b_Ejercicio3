#include <iostream>
#include <bitset>
#include <cmath>
#include <iomanip>

using namespace std;

union FloatBits {
    float f;
    unsigned int bits;
};

struct Float32 {
    int signo;
    int exponente;
    long long significando;
};

Float32 descomponer(float num) {
    FloatBits fb;
    fb.f = num;

    Float32 result;

    // Separacion del signo, exponente y significando
    result.signo = (fb.bits >> 31) & 1;
    result.exponente = (fb.bits >> 23) & 0xFF;
    result.significando = fb.bits & 0x7FFFFF;

    if (result.exponente != 0) {
        result.significando |= (1 << 23);
    }

    // Imprimir descomposición
    cout << "Numero: " << num << endl;
    cout << "Signo: " << result.signo << endl;
    cout << "Exponente: " << result.exponente << endl;
    cout << "Significando: " << result.significando << endl;
    cout << "-------------------------------" << endl;

    return result;
}

float juntar(const Float32& num) {
    FloatBits fb;
    unsigned int resultado = 0;

    // Union del signo, exponente y significando
    resultado |= (num.signo << 31);
    resultado |= (num.exponente << 23);
    resultado |= (num.significando & 0x7FFFFF);

    fb.bits = resultado;
    return fb.f;
}

float multiplicar(Float32 numX, Float32 numY) {

    Float32 result;

    // suma de exponentes y resta del bias
    result.exponente = (numX.exponente + numY.exponente) - 127;

    // Validacion de overflow o underflow
    if (result.exponente >= 255) {
        cout << "Overflow exponente";
        return 0;
    }
    else if (result.exponente <= 0) {
        cout << "Underflow exponente";
        return 0;
    }

    // Multiplicacion de significandos
    result.significando = numX.significando * numY.significando;

    // Normalización
    if (result.significando & (1LL << 47)) {
        result.significando = result.significando >> 24;
        result.significando += 1;
    }
    else {
        result.significando = result.significando >> 23;
    }

    result.signo = numX.signo ^ numY.signo;

    return(juntar(result));

}

int main() {
    float x, y;
    cout << "Ingrese un numero flotante: ";
    cin >> x;
    cout << "Ingrese otro numero flotante: ";
    cin >> y;

    // Verificar si alguno de ellos es 0
    if (x == 0 || y == 0) {
        cout << "Resultado de la multiplicacion: 0" << endl;
        return 0;
    }

    // Descomponer ambos números
    Float32 numX = descomponer(x);
    Float32 numY = descomponer(y);

    float resultado = multiplicar(numX, numY);

    cout << "\nResultado de la multiplicacion realizada: " << resultado << endl;
    cout << "Resultado de la multiplicacion normal (C++): " << x * y << endl;

    cout << "\n--- Con setpresicion(20) ---";

    cout << fixed << setprecision(20);  // Mostrar hasta 20 decimales

    cout << "\nResultado de la multiplicacion realizada: " << resultado << endl;
    cout << "Resultado de la multiplicacion normal (C++): " << x * y << endl;

}