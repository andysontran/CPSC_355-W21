/*
Author: Tran, Andy (Son)
UCID: 30019100
Course: CPSC 355 W2021
Assignment 2a

-- VERSION 1 -- 
Description:
Written in ARMv8 A64 assembly language, a program that finds the maximum of
y = -5x^3 - 21x^2 + 4x + 16 in the range -5 <= x <= 6, by stepping through
the range one by one in a loop and testing. 

Use only long integers for x, and do not factor the expression. Use the 
printf() function to display to the screen the values of x, y and the 
current maximum on each iteration of your loop.

Criteria for V1: 
*   Write the program without macros (i.e., do not use m4), and use only mul, 
    add and mov instructions to do your calculations. Use a pre-test loop, where
    the test is at the top of the loop.

To verify, run under gdb, capturing output using the script UNIX command.
    *   Single step though the program (use ni) for at least one iteration of 
        the loop, displaying the istruction being executed (use display/i $pc).
    *   Also print out the contents of particular registers (use p) at key 
        points in the program to show that it is working as expected.
*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// Define format string for call to printf()
display:    .string "x = %d, y = %d: Current maximum for y = %d\n"      // Output format
            
            // Define the main function for the program
            .balign 4                                                   // Instructions must be word aligned by 4 bits 
            .global main                                                // Make "main" visible to the OS
main:       stp     x29, x30, [sp, -16]!                                // Save frame pointer (fp, x29) and link register (lr, x30) to stack, allocating 16 bytes, pre-increment SP
            mov     x29, sp                                             // Update frame pointer (fp) to current stack pointer (sp) (after incrementing in previous step)

            mov     x19, -5                                             // Set x19 general purpose register to immediate value of -5 (lower bound of range), this will be the value of x
            mov     x20, 0                                              // Set x20 general purpose register to immediate value of 0, this will be the value of y at x 
            mov     x21, 0                                              // Set x21 general purpose register to immediate value of 0, this will initialize the current maximum value of y
            mov     x22, 0                                              // Set x22 general purpose register to immediate value of 0, this will initialize the loop counter 

test:       cmp     x19, 6                                              // Compare the current value of x to the maximum range value of 6 -- PRE-TEST LOOP
            b.gt    done                                                // If x > 6 is satisfied, then branch to "done"

            mov     x20, 0                                              // Set y = 0 at the start of each loop

            mov     x23, -5                                             // Set x23 general purpose register to immediate value of -5, this will be the leading term (-5x^3) of the function
            mul     x23, x23, x19                                       // Compute -5x and override 
            mul     x23, x23, x19                                       // Compute -5x^2 and override    
            mul     x23, x23, x19                                       // Compute -5x^3 and override
            
            mov     x24, -21                                            // Set x24 general purpose register to immediate value of -21, this will be the second term (-21x^2) of the function
            mul     x24, x24, x19                                       // Compute -21x and override
            mul     x24, x24, x19                                       // Compute -21x^2 and override 

            mov     x25, 4                                              // Set x25 general purpose register to immediate value of 4, this will be the third term (4x) of the function
            mul     x25, x25, x19                                       // Compute 4x and override 

            add     x20, x20, x23                                       // Add value of leading term (-5x^3) to y and override 
            add     x20, x20, x24                                       // Add value of second term (-21x^2) to y and override
            add     x20, x20, x25                                       // Add value of third term (4x) to y and override
            add     x20, x20, 16                                        // Add value of constant term (16) to y and override

            cmp     x22, 0                                              // Compare loop counter to 0
            b.eq    Ymax                                                // If loop counter = 0, then branch to "Ymax" and set the current maximum value of y to the current value of y

            cmp     x20, x21                                            // Compare the current value of y to the current maximum of y
            b.gt    Ymax                                                // If the current value of y > current maximum of y, then branch to "Ymax" to set the current maximum value of y to the current value of y

print:      adrp    x0, display                                         // Set first argument of printf()
            add     x0, x0, :lo12:display                               // Add low 12 bits to x0 register 
            mov     x1, x19                                             // Store current value of x in x1 register for printf()
            mov     x2, x20                                             // Store current value of y in x2 register for printf()
            mov     x3, x21                                             // Store current maximum value of y in x3 register for printf()
            bl      printf                                              // Call printf() function
            
            add     x19, x19, 1                                         // Increment current value of x by adding immediate value of 1 to x and override
            add     x22, x22, 1                                         // Increment loop counter by adding immediate value of 1 to loop counter and override
            b       test                                                // Branch to "test" to check if current value of x is within the range [-5, 6]        

Ymax:       mov     x21, x20                                            // Override the current maximum value of y with the current value of y (new maximum y value)
            b       print                                               // Branch to "print" to display and increment the values

done:       mov     x0, 0                                               // Set return value     
            ldp     x29, x30, [sp], 16                                  // Restore stack
            ret                                                         // Return to OS
            