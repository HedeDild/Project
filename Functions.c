#define _CRT_SECURE_NO_WARNINGS
#include "calculator.h"

// Helper constant for degree to radian conversion
#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)

/**
 * @brief Updates the result history: prev_result gets last_result, last_result gets new_result.
 * @param new_result The result of the latest calculation.
 */
void update_results(double new_result) {
    // Only update if the new result is a valid number
    if (!isnan(new_result)) {
        prev_result = last_result;
        last_result = new_result;
    }
}

/**
 * @brief Displays the main menu options to the user.
 */
void display_menu() {
    printf("\n======================================================\n");
    printf("              Advanced Calculator Options\n");
    printf("======================================================\n");
    // Display both R and P (Previous Result)
    printf("Last Result (R): %.4lf | Previous Result (P): %.4lf\n", last_result, prev_result);
    printf("1. Mathematical Operations (+, -, x, /, %%, exp, log, |x|, x^y, n!)\n");
    printf("2. Trigonometric Operations (sin, cos, tan, cot, hyp)\n");
    printf("3. Number System Conversions (Dec/Bin/Hex)\n");
    printf("4. Clear/Restart Calculator\n");
    printf("5. Exit Program\n");
    printf("------------------------------------------------------\n");
    printf("Enter your choice (1-5): ");
}

/**
 * @brief Gets a valid menu choice from the user.
 * @param max_choice The maximum valid choice number.
 * @return The valid choice selected by the user.
 */
int get_menu_choice(int max_choice) {
    int choice;
    if (scanf("%d", &choice) != 1) {
        // Clear input buffer on failure
        while (getchar() != '\n');
        printf("Invalid input. Please enter a number.\n");
        return -1;
    }
    // Clear the rest of the line
    while (getchar() != '\n');

    if (choice < 1 || choice > max_choice) {
        printf("Invalid choice. Please enter a number between 1 and %d.\n", max_choice);
        return -1;
    }
    return choice;
}

/**
 * @brief Gets a double operand from the user, allowing 'R', 'P', or nested sub-menu calls (1, 2, 3).
 * * NOTE: This function is now recursive, allowing users to select 1, 2, or 3 to calculate an operand
 * using a sub-operation before returning the result to the parent operation.
 * * @param prompt The message to display to the user.
 * @param use_result_option Set to 1 to allow using 'R', 'P', or nested menus.
 * @return The double value entered, or NAN if input is invalid or a nested operation fails.
 */
double get_double_input(const char* prompt, int use_result_option) {
    char input_buffer[64];
    double value = NAN;

    printf("%s", prompt);
    if (use_result_option) {
        printf(" (or type 'R' for %.4lf / 'P' for %.4lf, or 1/2/3 for Nested Op): ", last_result, prev_result);
    }
    else {
        printf(": ");
    }

    // Read the line into a buffer
    if (scanf("%63s", input_buffer) != 1) {
        while (getchar() != '\n'); // Clear buffer
        printf("Invalid input format.\n");
        return NAN;
    }
    // Clear remaining input buffer
    while (getchar() != '\n');

    if (use_result_option) {
        // 1. Check for R/P
        if (strcmp(input_buffer, "R") == 0 || strcmp(input_buffer, "r") == 0) {
            printf("-> Using Last Result (R): %.4lf\n", last_result);
            return last_result;
        }
        if (strcmp(input_buffer, "P") == 0 || strcmp(input_buffer, "p") == 0) {
            printf("-> Using Previous Result (P): %.4lf\n", prev_result);
            return prev_result;
        }

        // 2. Check for Nested Menu Call
        if (strcmp(input_buffer, "1") == 0 || strcmp(input_buffer, "2") == 0 || strcmp(input_buffer, "3") == 0) {
            int nested_choice = atoi(input_buffer);
            printf("\n--- Nested Operation (%s selected) ---\n",
                nested_choice == 1 ? "Math" : (nested_choice == 2 ? "Trig" : "Conversion"));

            // Call the appropriate handler, which now returns a double
            switch (nested_choice) {
            case 1: value = handle_math_operations(); break;
            case 2: value = handle_trig_operations(); break;
            case 3:
                printf("Warning: Number conversions are integer-based and may lose precision when used as floating-point operands.\n");
                value = handle_conversion_operations();
                break;
            }

            if (!isnan(value)) {
                printf("-> Nested Result: %.4lf used as operand.\n", value);
                return value;
            }
            else {
                // Nested operation failed or user chose 'Back'. Retry main operand input.
                printf("\nNested operation failed or cancelled. Please re-enter the required operand.\n");
                return get_double_input(prompt, use_result_option);
            }
        }
    }

    // 3. Attempt to convert the string to a double (Standard input)
    if (sscanf(input_buffer, "%lf", &value) != 1) {
        printf("Invalid input. Must be a number, 'R', 'P', or a nested menu choice (1, 2, or 3).\n");
        return NAN;
    }

    return value;
}


// --- Mathematical Operations Implementations ---

double add(double a, double b) { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b) {
    if (b == 0) {
        printf("Error: Division by zero!\n");
        return NAN; // Not a Number
    }
    return a / b;
}
long long remainder_op(long long a, long long b) {
    if (b == 0) {
        printf("Error: Modulo by zero!\n");
        return 0;
    }
    return a % b;
}
double exponential(double x) { return exp(x); }
double logarithm(double x) {
    if (x <= 0) {
        printf("Error: Logarithm input must be positive.\n");
        return NAN;
    }
    return log(x); // Natural logarithm
}
double abs_square_root(double x) {
    // sqrt(x^2) is simply the absolute value of x
    return fabs(x);
}
double power(double base, double exp) { return pow(base, exp); }

/**
 * @brief Calculates the factorial of a non-negative integer n.
 * @param n The number to calculate the factorial of.
 * @return The factorial result (unsigned long long) or 0 for error.
 */
unsigned long long factorial(int n) {
    if (n < 0) {
        printf("Error: Factorial is undefined for negative numbers.\n");
        return 0;
    }
    if (n > 20) {
        // Limit to prevent overflow for unsigned long long
        printf("Error: Input too large for factorial (max 20).\n");
        return 0;
    }
    if (n == 0) return 1;

    unsigned long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}


// --- Trigonometric Operations Implementations (Using degrees) ---

double sine_deg(double deg) {
    return sin(deg * DEG_TO_RAD);
}
double cosine_deg(double deg) {
    return cos(deg * DEG_TO_RAD);
}
double tangent_deg(double deg) {
    double rad = deg * DEG_TO_RAD;
    double tan_val = tan(rad);
    // Check for values near 90 + 180*k (vertical asymptote)
    if (fabs(cos(rad)) < 1e-9) {
        printf("Warning: Tangent is undefined near 90 or 270 degrees.\n");
        return NAN;
    }
    return tan_val;
}
double cotangent_deg(double deg) {
    double tan_val = tangent_deg(deg);
    if (isnan(tan_val)) {
        // tan is undefined, check sin(rad)
        if (fabs(sin(deg * DEG_TO_RAD)) < 1e-9) {
            printf("Warning: Cotangent is undefined near 0 or 180 degrees.\n");
            return NAN;
        }
        // If tan is undefined (cos is zero), cot is 0
        return 0.0;
    }
    if (fabs(tan_val) < 1e-9) {
        printf("Warning: Cotangent approaches infinity.\n");
        return NAN;
    }
    return 1.0 / tan_val;
}
double hypotenuse(double a, double b) {
    // Calculates sqrt(a^2 + b^2)
    return hypot(a, b);
}


// --- Number System Conversion Implementations ---

/**
 * @brief Converts a decimal number to its binary string representation.
 */
void dec_to_bin(long long dec) {
    if (dec == 0) {
        printf("Binary: 0\n");
        return;
    }

    // Since a long long can be up to 64 bits, we need a buffer of about 65 chars
    char binary_str[65];
    int i = 0;
    unsigned long long temp_dec = (unsigned long long)llabs(dec);

    if (temp_dec == 0) {
        printf("Binary: 0\n");
        return;
    }

    while (temp_dec > 0 && i < 64) {
        binary_str[i++] = (temp_dec % 2) + '0';
        temp_dec /= 2;
    }
    binary_str[i] = '\0';

    printf("Binary: %s", (dec < 0 ? "-" : ""));

    // Print in reverse order
    for (int j = i - 1; j >= 0; j--) {
        printf("%c", binary_str[j]);
    }
    printf("\n");
}

/**
 * @brief Converts a binary string to a decimal number.
 */
long long bin_to_dec(const char* bin_str) {
    long long dec = 0;
    long long base = 1;
    int len = (int)strlen(bin_str);

    for (int i = len - 1; i >= 0; i--) {
        if (bin_str[i] == '1') {
            dec += base;
        }
        else if (bin_str[i] != '0') {
            printf("Error: Invalid binary digit '%c'.\n", bin_str[i]);
            return -1;
        }
        base *= 2;
    }
    return dec;
}

/**
 * @brief Converts a decimal number to its hexadecimal representation.
 */
void dec_to_hex(long long dec) {
    printf("Hexadecimal: %llX\n", dec);
}

/**
 * @brief Converts a hexadecimal string to a decimal number.
 */
long long hex_to_dec(const char* hex_str) {
    long long dec;
    // strtoll converts string to long long with base 16 (hex)
    if (sscanf(hex_str, "%llx", &dec) != 1) {
        printf("Error: Invalid hexadecimal format.\n");
        return -1;
    }
    return dec;
}

/**
 * @brief Converts a hexadecimal string to a binary string representation.
 */
void hex_to_bin(const char* hex_str) {
    long long dec = hex_to_dec(hex_str);
    if (dec != -1) {
        printf("Decimal: %lld\n", dec); // Show intermediate decimal value
        dec_to_bin(dec);
    }
}

/**
 * @brief Converts a binary string to a hexadecimal representation.
 */
void bin_to_hex(const char* bin_str) {
    long long dec = bin_to_dec(bin_str);
    if (dec != -1) {
        printf("Decimal: %lld\n", dec); // Show intermediate decimal value
        dec_to_hex(dec);
    }
}

/**
 * @brief Handles the flow for Mathematical Operations sub-menu. Returns the result as a double.
 */
double handle_math_operations() {
    int math_choice;
    double a = NAN, b = NAN, result_d = NAN;
    long long a_ll, b_ll, result_ll;
    int n;

    printf("\n--- Mathematical Operations ---\n");
    printf("1. Add (+)\n2. Subtract (-)\n3. Multiply (x)\n4. Divide (÷)\n");
    printf("5. Remainder (%%)\n6. Exponential (exp(x))\n7. Logarithmic (log(x))\n");
    printf("8. Absolute value of input (sqrt(x^2))\n9. Power (x^y)\n10. Factorial (n!)\n");
    printf("11. Back to Previous Operation/Main Menu\n");
    printf("Enter choice (1-11): ");

    math_choice = get_menu_choice(11);
    // If choice is invalid or "Back", return NAN
    if (math_choice == -1 || math_choice == 11) return NAN;

    // Handle single-operand functions (exp, log, |x|)
    if (math_choice == 6 || math_choice == 7 || math_choice == 8) {
        // get_double_input allows R/P/Nested Ops
        a = get_double_input("Enter a single number (x)", 1);
        if (isnan(a)) return NAN;

        switch (math_choice) {
        case 6: result_d = exponential(a); printf("exp(%.4lf) = %.4lf\n", a, result_d); break;
        case 7: result_d = logarithm(a); printf("log(%.4lf) = %.4lf\n", a, result_d); break;
        case 8: result_d = abs_square_root(a); printf("|%.4lf| (sqrt(x^2)) = %.4lf\n", a, result_d); break;
        }
        return result_d;
    }

    // Handle Factorial (integer only) - Cannot use recursive get_double_input here
    if (math_choice == 10) {
        printf("Enter a non-negative integer (n): ");
        if (scanf("%d", &n) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); return NAN; }
        while (getchar() != '\n');

        result_ll = factorial(n);
        if (result_ll != 0) {
            printf("%d! = %llu\n", n, result_ll);
            return (double)result_ll;
        }
        return NAN;
    }

    // Handle Remainder (integer only) - Cannot use recursive get_double_input here
    if (math_choice == 5) {
        printf("Enter two integers (a %% b) - Note: R/P/Nested Op is NOT available for integer-only input.\n");
        printf("Enter the first integer (a): ");
        if (scanf("%lld", &a_ll) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); return NAN; }
        printf("Enter the second integer (b): ");
        if (scanf("%lld", &b_ll) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); return NAN; }
        while (getchar() != '\n');

        result_ll = remainder_op(a_ll, b_ll);
        if (b_ll != 0) {
            printf("%lld %% %lld = %lld\n", a_ll, b_ll, result_ll);
            return (double)result_ll;
        }
        return NAN;
    }

    // Handle two-operand floating-point functions (Add, Sub, Mul, Div, Pow)
    // get_double_input allows R/P/Nested Ops
    a = get_double_input("Enter the first number (a)", 1);
    if (isnan(a)) return NAN;
    b = get_double_input("Enter the second number (b)", 1);
    if (isnan(b)) return NAN;

    switch (math_choice) {
    case 1: result_d = add(a, b); printf("%.4lf + %.4lf = %.4lf\n", a, b, result_d); break;
    case 2: result_d = subtract(a, b); printf("%.4lf - %.4lf = %.4lf\n", a, b, result_d); break;
    case 3: result_d = multiply(a, b); printf("%.4lf x %.4lf = %.4lf\n", a, b, result_d); break;
    case 4: result_d = divide(a, b);
        if (!isnan(result_d)) printf("%.4lf ÷ %.4lf = %.4lf\n", a, b, result_d);
        break;
    case 9: result_d = power(a, b); printf("%.4lf ^ %.4lf = %.4lf\n", a, b, result_d); break;
    default: result_d = NAN; // Should not happen
    }

    return result_d;
}

/**
 * @brief Handles the flow for Trigonometric Operations sub-menu. Returns the result as a double.
 */
double handle_trig_operations() {
    int trig_choice;
    double angle, a, b, result_d = NAN;

    printf("\n--- Trigonometric Operations ---\n");
    printf("NOTE: Angles are in degrees.\n");
    printf("1. Sine (sin)\n2. Cosine (cos)\n3. Tangent (tan)\n4. Cotangent (cot)\n");
    printf("5. Hypotenuse (hyp, calculates c = sqrt(a^2 + b^2))\n");
    printf("6. Back to Previous Operation/Main Menu\n");
    printf("Enter choice (1-6): ");

    trig_choice = get_menu_choice(6);
    // If choice is invalid or "Back", return NAN
    if (trig_choice == -1 || trig_choice == 6) return NAN;

    // Handle Hypotenuse (two-operand)
    if (trig_choice == 5) {
        // get_double_input allows R/P/Nested Ops
        a = get_double_input("Enter side a", 1);
        if (isnan(a)) return NAN;
        b = get_double_input("Enter side b", 1);
        if (isnan(b)) return NAN;

        result_d = hypotenuse(a, b);
        printf("Hypotenuse of %.4lf and %.4lf is %.4lf\n", a, b, result_d);
        return result_d;
    }

    // Handle single-operand functions (angle in degrees)
    // get_double_input allows R/P/Nested Ops
    angle = get_double_input("Enter the angle in degrees", 1);
    if (isnan(angle)) return NAN;

    switch (trig_choice) {
    case 1: result_d = sine_deg(angle); printf("sin(%.4lf°) = %.4lf\n", angle, result_d); break;
    case 2: result_d = cosine_deg(angle); printf("cos(%.4lf°) = %.4lf\n", angle, result_d); break;
    case 3: result_d = tangent_deg(angle);
        if (!isnan(result_d)) printf("tan(%.4lf°) = %.4lf\n", angle, result_d);
        break;
    case 4: result_d = cotangent_deg(angle);
        if (!isnan(result_d)) printf("cot(%.4lf°) = %.4lf\n", angle, result_d);
        break;
    default: result_d = NAN; // Should not happen
    }

    return result_d;
}

/**
 * @brief Handles the flow for Number System Conversion sub-menu. Returns the decimal equivalent as a double.
 */
double handle_conversion_operations() {
    int conv_choice;
    char input_str[64];
    long long dec_val = -1; // Use -1 as sentinel for failed conversion
    double result_d = NAN;


    printf("\n--- Number System Conversions ---\n");
    printf("Note: String input conversions (Bin/Hex) do NOT support R/P/Nested Ops.\n");
    printf("1. Dec to Bin\n2. Bin to Dec\n3. Dec to Hex\n4. Hex to Dec\n");
    printf("5. Hex to Bin (Intermediate)\n6. Bin to Hex (Intermediate)\n");
    printf("7. Back to Previous Operation/Main Menu\n");
    printf("Enter choice (1-7): ");

    conv_choice = get_menu_choice(7);
    // If choice is invalid or "Back", return NAN
    if (conv_choice == -1 || conv_choice == 7) return NAN;

    // Conversions involving a decimal input (supports R/P/Nested Ops via get_double_input)
    if (conv_choice == 1 || conv_choice == 3) {
        // Use get_double_input but check that the value is an integer before conversion
        double dec_d = get_double_input("Enter Decimal number (will be truncated to integer)", 1);
        if (isnan(dec_d)) return NAN;

        // Truncate to long long for conversion
        dec_val = (long long)dec_d;

        switch (conv_choice) {
        case 1: dec_to_bin(dec_val); break;
        case 3: dec_to_hex(dec_val); break;
        }
        // Return the decimal value as a double
        result_d = (double)dec_val;
        return result_d;
    }

    // Conversions involving string inputs (Bin/Hex) - Cannot use recursive get_double_input here
    printf("Enter the number string: ");
    if (scanf("%63s", input_str) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); return NAN; }
    while (getchar() != '\n');

    switch (conv_choice) {
    case 2:
        dec_val = bin_to_dec(input_str);
        if (dec_val != -1) {
            printf("Decimal: %lld\n", dec_val);
            result_d = (double)dec_val;
        }
        break;
    case 4:
        dec_val = hex_to_dec(input_str);
        if (dec_val != -1) {
            printf("Decimal: %lld\n", dec_val);
            result_d = (double)dec_val;
        }
        break;
    case 5:
        hex_to_bin(input_str);
        break;
    case 6:
        bin_to_hex(input_str);
        break;
    }

    return result_d;
}