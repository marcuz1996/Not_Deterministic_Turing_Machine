# TURING MACHINE SIMULATOR

implementation in C language standard (only C libraries) of a not deterministic Turing Machine, sigle tape and only acceptor.

## CONVENTIONS  
• For semplicity the tape symbols are char, while states are integers. The character _ indicates the symbol blank.  
• The machine always starts from state 0 and from the first character of the input string.  
• Tape is unlimited both on the left and on the right and contains the character _ in each position.  
• The characters "L", "R", "S" are used for the movement of the head on tape.  
• The input file is provided by standard input, while the output file is on standard output.  

## STRUCTURE OF INPUT FILE  
The input file is divided into 4 parts:  
• the first, that starts with "tr", contains the transitions, one for line. ex: "0 a c R 1" we go from state 0 to state 1, we read "a", write c and head moves to right (R).  
• next part start with "acc" and contains, one for line, the number of acceptation state.  
• To avoid problems of infinite computations, in the next section, starting with "max", the number of maximum moves that can be made to accept a string is indicated.  
• The final part, which begins with "run", is a list of strings to be supplied to the machine, one for line.  

## STRUCTURE OF OUTPUT FILE:  
• we have "0" if string is not accepted;  
• "1" if the string is accepted  
• "U" if we are not arrived in acceptation state but we have reached the maximum steps.  

## EXAMPLE:  
In this example we accept only the strings that are equal read from beginning to end or from end to beginning.  

INPUT:  
tr  
0 a a R 0  
0 b b R 0  
0 a c R 1  
0 b c R 2  
1 a c L 3  
2 b c L 3  
3 c c L 3  
3 a c R 4  
3 b c R 5  
4 c c R 4  
4 a c L 3  
5 c c R 5  
5 b c L 3  
3 _ _ R 6  
6 c c R 6  
6 _ _ S 7  
acc  
7  
max  
800  
run  
aababbabaa  
aababbabaaaababbabaa  
aababbabaaaababbabaab  
aababbabaaaababbabaabbaababbabaaaababbabaa  
aababbabbaaababbabaabbaababbabaaaababbabaa  

OUTPUT:  
1  
1  
0  
U  
0
