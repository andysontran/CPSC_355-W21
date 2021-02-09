/*
Author: Tran, Andy (Son)
UCID: 30019100
Course: CPSC 355 W2021
Assignment 2b

-- VERSION 2 --
Description:
Written in ARMv8 A64 assembly language, a program that finds the maximum of
y = -5x^3 - 21x^2 + 4x + 16 in the range -5 <= x <= 6, by stepping through
the range one by one in a loop and testing. 

Use only long integers for x, and do not factor the expression. Use the 
printf() function to display to the screen the values of x, y and the 
current maximum on each iteration of your loop.

Criteria for V2: 
*   Optimize the V1 program by putting the loop test at the bottom of the loop
    (make sure it is still a pre-test loop), and making use of the madd 
    instruction. 
*   Also, add macros to the V1 program to make it more readable
    use (m4). In particular, provide macros for heavily used registers.

To verify, run under gdb, capturing output using the script UNIX command.
    *   Set a breakpoint just after the place where the final result is calculated,
        and then print out the maximum. Do not single step through this version.
*/

/*-----------------------------------------------------------------------------------------------------------------*/

// Define macros
define(x_r, x19)                                                        // Assign "x_r" macro to x19 register to store the current value of x
define(y_r, x20)                                                        // Assign "y_r" macro to x20 register to store the current value of y
define(max_r, x21)                                                      // Assign "max_r" macro to x21 register to store the current maximum value of y         
define(count_r, x22)                                                    // Assign "count_r" macro to x22 register to store the loop counter value
define(a_r, x23)                                                        // Assign "a_r" macro to x23 register to store the leading term (-5x^3) of the function
define(b_r, x24)                                                        // Assign "b_r" macro to x24 register to store the second term (-21x^2) of the function
define(c_r, x25)                                                        // Assign "c_r" macro to x25 register to store the third term (4x) of the function

// Define format string for call to printf()
fmt:        .string "x = %d, y = %d: Current maximum for y = %d\n"      // Output format
            
            // Define the main function for the program
            .balign 4                                                   // Instructions must be word aligned by 4 bits
            .global main                                                // Make "main" visible to the OS

main:       stp     x29, x30, [sp, -16]!                                // Save frame pointer (fp, x29) and link register (lr, x30) to stack, allocating 16 bytes, pre-increment SP
            mov     x29, sp                                             // Update frame pointer (fp) to current stack pointer (sp) (after incrementing in previous step)

            mov     x_r, -5                                             // Initialize x to immediate value of -5 (lower bound of range)
            mov     y_r, 0                                              // Initialize y to immediate value of 0
            mov     max_r, 0                                            // Initialize current maximum value of y to immediate value of 0
            mov     count_r, 0                                          // Initialize loop counter to immediate value of 0

loop:       mov     y_r, 0                                              // Set y = 0 at the start of each loop

            mov     a_r, -5                                             // Set "a_r" macro to immediate value of -5, this will be the leading term (-5x^3) of the function
            mul     a_r, a_r, x_r                                       // Compute -5x and override
            mul     a_r, a_r, x_r                                       // Compute -5x^2 and override
            madd    y_r, a_r, x_r, y_r                                  // Compute -5x^3 and add current value of y (0), then store result in y (now -5x^3)

            mov     b_r, -21                                            // Set "b_r" macro to immediate value of -21, this will be the second term (-21x^2) of the function
            mul     b_r, b_r, x_r                                       // Compute -21x and override
            madd    y_r, b_r, x_r, y_r                                  // Compute -21x^2 and add current value of y (-5x^3), then store result in y (now -5x^3 - 21x^2)

            mov     c_r, 4                                              // Set "c_r" macro to immediate value of 4, this will be the third term (4x) of the function
            madd    y_r, c_r, x_r, y_r                                  // Compute 4x and add current value of y (-5x^3 - 21x^2), then store result in y (now -5x^3 - 21x^2 + 4x)

            add     y_r, y_r, 16                                        // Add value of constant term (16) to y and override (now -5x^3 - 21x^2 + 4x + 16)

            cmp     count_r, 0                                          // Compare loop counter to 0
            b.eq    Ymax                                                // If loop counter = 0, then branch to "Ymax" and set the current maximum value of y to the current value of y

            cmp     y_r, max_r                                          // Compare the current value of y to the current maximum of y
            b.gt    Ymax                                                // If the current value of y > current maximum value of y, then branch to "Ymax" to set the current maximum value of y to the current value of y

print:      adrp    x0, fmt                                             // Set first argument of printf()
            add     x0, x0, :lo12:fmt                                   // Add low 12 bits to x0 register
            mov     x1, x_r                                             // Store current value of x in x1 register for printf()
            mov     x2, y_r                                             // Store current value of y in x2 register for printf()
            mov     x3, max_r                                           // Store current maximum value of y in x3 register for printf()
            bl      printf                                              // Call printf() function

            add     x_r, x_r, 1                                         // Increment current value of x by adding immediate value of 1 to x and override
            add     count_r, count_r, 1                                 // Increment loop counter by adding immediate value of 1 to loop counter and override
            b       test                                                // Branch to "test" to check if current value of x is within the range [-5, 6]

Ymax:       mov     max_r, y_r                                          // Override the current maximum value of y with the current value of y (new maximum y value)
            b       print                                               // Branch to "print" to display and increment the values

test:       cmp     x_r, 6                                              // Compare the current value of x to the maximum range value of 6 -- PRE-TEST LOOP
            b.le    loop                                                // If x > 6 is satisfied, then branch to "done"

done:       mov     x0, 0                                               // Set return value
            ldp     x29, x30, [sp], 16                                  // Restore stack
            ret                                                         // Return to OS