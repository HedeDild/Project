#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Global variables to store the last two numerical results
extern double last_result;   // R (most recent)
extern double prev_result;   // P (previous result)

// --- Menu Functions ---
void display_menu();
int get_menu_choice(int max_choice);
/**
 * @brief Handles Mathematical Operations. Returns the result as a double.
 * @return The result of the calculation, or NAN if cancelled/failed.
 */
double handle_math_operations();
/**
 * @brief Handles Trigonometric Operations. Returns the result as a double.
 * @return The result of the calculation, or NAN if cancelled/failed.
 */
double handle_trig_operations();
/**
 * @brief Handles Number System Conversions. Returns the decimal equivalent as a double.
 * @return The result of the conversion, or NAN if cancelled/failed.
 */
double handle_conversion_operations();

// --- Helper Function ---
/**
 * @brief Updates the result history: prev_result gets last_result, last_result gets new_result.
 * Called only when a top-level calculation is successfully completed.
 * @param new_result The result of the latest calculation.
 */
void update_results(double new_result);

// --- Input Function (Refactored for Recursion) ---
/**
 * @brief Gets a double operand from the user, allowing 'R', 'P', or nested sub-menu calls (1, 2, 3).
 * @param prompt The message to display to the user.
 * @param use_result_option Set to 1 to allow using 'R' and 'P'.
 * @return The double value entered, or NAN if input is invalid or a nested operation fails.
 */
double get_double_input(const char* prompt, int use_result_option);

// --- Mathematical Operations ---
double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
long long remainder_op(long long a, long long b);
double exponential(double x); // exp(x)
double logarithm(double x);   // log(x) (natural log)
double abs_square_root(double x); // sqrt(x^2) which is |x|
double power(double base, double exp); // x^y
unsigned long long factorial(int n); // n!

// --- Trigonometric Operations (Inputs in Degrees for user-friendliness) ---
double sine_deg(double deg);
double cosine_deg(double deg);
double tangent_deg(double deg);
double cotangent_deg(double deg);
double hypotenuse(double a, double b); // hypot(a,b)

// --- Number System Conversions ---
void dec_to_bin(long long dec);
long long bin_to_dec(const char* bin_str);

void dec_to_hex(long long dec);
long long hex_to_dec(const char* hex_str);

void hex_to_bin(const char* hex_str);
void bin_to_hex(const char* bin_str);

#endif // CALCULATOR_H#pragma once
