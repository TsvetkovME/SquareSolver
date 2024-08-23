#include <TXLib.h>
#include <stdio.h>
#include <math.h>



struct Test_data
{   int nTest;
    double a, b, c;
    double x1_exp, x2_exp;
    int nRoots_exp;
};

// TODO: Doxygen,
enum SOLVING_SQUARED_EQUATION {      //TODO read about enums
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS,
    INFINITY_ROOTS,
    SOLVING_ERR
};


enum INPUT_CONST {
    INCORRECT_IN,
    CORRECT_IN
};

enum TESTING {
    TEST_FAILED,
    TEST_DONE
};


const double DELTA = 1e-7;


int compare_doubles(double a, double b);

int solve_linear_equation(double a, double b, double* x);

int solve_squared_equation(double a, double b, double c, double* x1, double* x2);

int input_coeffs(double* a, double* b, double* c);

void print_roots(int roots, double x1, double x2);

int equality_of_answers(double x1, double x2, int nRoots, double x1_exp, double x2_exp, int nRoots_exp);

int run_test(const struct Test_data* test);

int run_tests();

void clear_stdin();



int main() {

#if 0
    double a = NAN, b = NAN, c = NAN, x1 = NAN, x2 = NAN;

    input_coeffs(&a, &b, &c);

    int number_of_roots = solve_squared_equation(a, b, c, &x1, &x2);

    print_roots(number_of_roots, x1, x2);
#endif

#if 1

    if (run_tests() == TEST_DONE) {
        printf("Тестирование прошло без ошибок\n");
    }

#endif
    return 0;
}



int compare_doubles(double a, double b) {
    return (fabs(a - b) < DELTA);      //abs и fabs
}


int solve_linear_equation(double a, double b, double* x) {
    if (x == NULL || !isfinite(a) || !isfinite(b)) {
        return SOLVING_ERR;
    }
    if (compare_doubles(a, 0.0)) {
        if (compare_doubles(b, 0.0)) {
            return INFINITY_ROOTS;
        }
        return NO_ROOTS;
    }

    *x = -b / a;
    if (compare_doubles(*x, 0.0)) {
        *x = 0.0;
    }

    return ONE_ROOT;
}


int solve_squared_equation(double a, double b, double c, double* x1, double* x2) { //константы и указатели?

    if (x1 == NULL || x2 == NULL || x1 == x2 || !isfinite(a) || !isfinite(b) || !isfinite(c)) {
        return SOLVING_ERR;
    }


    *x1 = *x2 = NAN;
    if (compare_doubles(a, 0.0)) {
        return solve_linear_equation(b, c, x1);
    }


    double d = b * b - 4 * a * c;

    if (d < 0) {
        return NO_ROOTS;
    }

    if (compare_doubles(d, 0.0)) {
        *x1 = *x2 = -b / (2 * a);
        return ONE_ROOT;
    }

    double sqrt_from_d = sqrt(d);
    *x1 = (-b - sqrt_from_d) / (2 * a);
    *x2 = (-b + sqrt_from_d) / (2 * a);

    if (compare_doubles(*x1, 0.0)) {
        *x1 = 0.0;
    }
    if (compare_doubles(*x2, 0.0)) {
        *x2 = 0.0;
    }
    return TWO_ROOTS;
}



int input_coeffs(double* a, double* b, double* c) { //stdin, stdout, stderr

    if (a == NULL || b == NULL || c == NULL || a == b || b == c || a == c) {
        return INCORRECT_IN;
    }

    printf("Эта программа решает квадратные уравнения - уравнения вида Ax^2 + Bx + C = 0.\nВведите через пробел коэффициенты A, B и C:\n");
    while (1) {
        if (scanf("%lg %lg %lg", a, b, c) == 3) {
            return CORRECT_IN;
        }
        printf("Вводить только числа через пробел!\n");
        clear_stdin();
    }
    return INCORRECT_IN;
}



void print_roots(int roots, double x1, double x2) {
    switch (roots) {
        case NO_ROOTS:
            printf("Нет решений\n");
            break;
        case ONE_ROOT:
            printf("Одно решение: x = %lg", x1);
            break;
        case TWO_ROOTS:
            printf("x1 = %lg\nx2 = %lg\n", x1, x2);
            break;
        case INFINITY_ROOTS:
            printf("X принадлежит действительным числам\n");
            break;
        case SOLVING_ERR:
            printf("Ошибка вычислений\n");
        default:
            printf("Ошибка вывода\n");
    }
}



int equality_of_answers(double x1, double x2, int nRoots, double x1_exp, double x2_exp, int nRoots_exp) {
    if (nRoots != nRoots_exp) {
        return 0;
    }
    if (nRoots == ONE_ROOT) {
        return compare_doubles(x1, x1_exp);
    }
    if (nRoots == TWO_ROOTS) {
        return (compare_doubles(x1, x1_exp) && compare_doubles(x2, x2_exp)) || (compare_doubles(x1, x2_exp) && compare_doubles(x2, x1_exp));
    }
    return 1;

}




int run_test(const struct Test_data* test) {
    double x1 = NAN, x2 = NAN;

    int nRoots = solve_squared_equation(test->a, test->b, test->c, &x1, &x2);

    if (!equality_of_answers(x1, x2, nRoots, test->x1_exp, test->x2_exp, test->nRoots_exp)) {
        printf("%d. Ошибка тестирования:\na = %lg, b = %lg, c = %lg,\nx1 ?= %lg, x2 ?= %lg, nRoots ?= %d,\nx1 = %lg, x2 = %lg, nRoots = %d.\n",
        test->nTest, test->a, test->b, test->c, x1, x2, nRoots, test->x1_exp, test->x2_exp, test->nRoots_exp);

        return 1;
    }
    return 0;
}

  //open

int run_tests() {

    const struct Test_data tests[] = {{1, 1, 5, 2, -0.4384471872, -4.561552813, TWO_ROOTS},
                                      {2, 0, 7, 9, (-9.0 / 7.0), 0, ONE_ROOT},
                                      {3, 0, 0, 5, 0, 0, NO_ROOTS},
                                      {4, 0, 0, 0, 0, 0, INFINITY_ROOTS},
                                      {5, 1, -2, 1, 1, 1, ONE_ROOT},
                                      {6, 1, 2, 1, -1, -1, ONE_ROOT},
                                      {7, 56.347, 12.396, 3.635, 0, 0, NO_ROOTS},
                                      {8, 56.347, 125.385, 3.635, -0.02937857915, -2.195850803, TWO_ROOTS}};


    const size_t nTests = sizeof(tests) / sizeof(tests[0]);

    int flag = 0;

    for (size_t i = 0; i < nTests; ++i) {
        flag += run_test(&tests[i]);
    }


    if (flag > 0) {
        return TEST_FAILED;
    }
    return TEST_DONE;
}



void clear_stdin() {
    while (getchar() != '\n');
}



