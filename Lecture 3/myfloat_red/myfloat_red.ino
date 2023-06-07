#include <math.h>
#include <stdio.h>

// Low precision floating-point type
typedef struct myfloat {
    signed char mantissa;
    signed char exponent;
} myfloat_type;

// Conversion from double to low precision type
myfloat_type double_to_myfloat(double x) {
    myfloat_type result;
    int sign, mantissa, exponent;

    // Get the sign of x
    sign = (x < 0) ? -1 : 1;

    // Get the absolute value of x and multiply by 100
    x = fabs(x) * 100.0;

    // Calculate the exponent
    exponent = 0;
    while (x >= 12800.0) {
        x /= 2.0;
        exponent++;
    }
    while (x < 6400.0 && exponent > -126) {
        x *= 2.0;
        exponent--;
    }

    // Round the mantissa and store it as a signed char
    mantissa = (int)(x + 0.5) * sign / 100;
    if (mantissa == 0) {
        exponent = 0;
    }

    // Store the mantissa and exponent in the result struct
    result.mantissa = (signed char)mantissa;
    result.exponent = (signed char)exponent;

    return result;
}

// Conversion from low precision type to double
double myfloat2double(myfloat_type* arg1) {
    double res = (double)(arg1->mantissa) * pow(2, arg1->exponent);
    return res;
}

// Multiply two low precision types
void mult_float(myfloat_type* arg1, myfloat_type* arg2, myfloat_type* result) {
    int temp;
    unsigned char sign;

    sign = 0x80 & ((unsigned char)arg1->mantissa ^ (unsigned char)arg2->mantissa); // Find sign of result 

    char i = 0;
    temp = (int)(arg1->mantissa) * (int)(arg2->mantissa);

    temp = temp & 0x7f00; // Take away sign from product

    while (abs(temp) > 128) {
        i++;
        temp = temp >> 1;
    }

    result->mantissa = (unsigned char)temp;

    result->mantissa = result->mantissa | sign; // Add recorded sign

    result->exponent = arg1->exponent + arg2->exponent + i;
}

void setup() {
    Serial.begin(9600);
    delay(10);

    double da[100];
    myfloat_type mda[100];
    double a = 1.001;
    myfloat_type f1;
    myfloat_type f;
    double error_sum = 0;
    double error_sum2 = 0;

    // Generate random numbers and convert them to low-precision format
    for (int i = 0; i < 100; i++) {
        da[i] = random(-5, 5)/10.0+random(-5, 5);
        mda[i] = double_to_myfloat(da[i]);
    }

    // Original calculation
    for (int i = 0; i <= 99; i++) {
        a *= da[i];

        double x = mda[i].mantissa * pow(2, mda[i].exponent);
        double diff = x - da[i];
        error_sum += diff;
        error_sum2 += pow(diff, 2);
    }

    double meanError = error_sum / 100;
    double powerMeanError = error_sum2 / 100;

    Serial.print("Mean error: ");
    Serial.println(meanError);
    Serial.print("Power mean error: ");
    Serial.println(powerMeanError);

    unsigned long startTime = micros(); // Start measuring execution time

    // Multiply 'a' with each element of 'da' array
    for (int i = 0; i < 100; i++) {
        a *= da[i];
    }

    unsigned long endTime = micros(); // Stop measuring execution time
    unsigned long executionTime = endTime - startTime; // Calculate the execution time

    startTime = micros(); // Start measuring execution time
    // Iterate over 'mda' array and multiply with 'f1'
    for (int i = 0; i < 100; i++) {
        mult_float(&f1, &mda[i], &f);
        memcpy(&f1, &f, sizeof(myfloat_type));
    }
    endTime = micros(); // Stop measuring execution time
    unsigned long myexecutionTime = endTime - startTime; // Calculate the execution time

    Serial.print("Relative Execution Time: ");
    Serial.println(myexecutionTime - executionTime);
}

void loop() {
    // Empty loop
}
