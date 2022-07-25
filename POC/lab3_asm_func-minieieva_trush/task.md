# POK Lab work 3. Write given function on assembler (FASM only)

### Task (4 points)
#### Implement two of the three functions assigned to you using the assembler.
#### Write a program that calls your functions from the assembler program `main_a.asm`.
#### Write a program that calls your functions from the C program `main_c.c`.
#### Provide makefile for automated building of your project.
#### Provide README.md with the important information: target operating system (OS), GitHub link, task variants implemented, team members, and implemented additional tasks if any. This information would be extracted by the tests.

### Important requirements:
- do not change the project structure
- output files should be in 'bin/' directory, named `call_from_c` and `call_from_s`

### Info about tests:
- automated tests will parse your `README.md` to check your variant number and OS
- tests will be publicly available (not in 2021, but after)
- input arrays always have equal size
- do not change project template structure

### Evaluation
- project can be compiled using your makefile, without any modifications, on the system cpecified in README - 0.25
- easy tests pass - 0.25
- middle tests pass - 0.5
- hard tests pass - 1
- All requirements satisfied (README, project structure, deadline, submitting rules, ) - 1
- Point after personal code review from teacher or teacher assistant (only if code CAN BE COMPILED) - 1

<!-- ### Submition -->
<!-- - create a new branch `dev` -->
<!-- - all development should be in `dev` branch -->
<!-- - when you think your lab work is ready, make a `pull request` to `master` branch (DO NOT MERGE IT) and submit your task on CMS -->

### Penalties
- deadline overdue to one week - maximum 50% of each point per task
- deadline overdue on more then one week - graidind starts with (-4) points (maximum 0)
- task not submitted to the exam session - (-8) points
- plagiarism more than 40% using moss checking system - (-4) for the lab, and letter to the faculty administration
- additional penalties (a.e. for archives on GitHub, similarity with compiled-generated code) - up to teacher, but not more than (-4) points in total.

### Tasks

1. Sort 32-bit uint array.<br>
function prototype:
```c
void func (uint32_t* input_array, size_t size);
```
2. Sort 32-bit int array.<br>
function prototype: 
```c
void func (int32_t* input_array, size_t size);
```
3. Sort 64-bit uint array.<br>
function prototype:
```c
void func (uint64_t* input_array, size_t size);
```
4. Sort 64-bit int array.<br>
function prototype:
```c
void func (int64_t* input_array, size_t size);
```
5. Solve a\*x+b=0 in 32-bit int (approximately) for two arrays, save results in third.<br>
function prototype
```c
void func (int32_t* a, int32_t* b, int32_t* x, size_t size);
```
remarks:<br>
For each `a` and `b` the function should find such `x` that `|a*x + b|` is as small as possible.<br>
* ` a == 3, b == 10` \t `-> x == -3`
* ` a == 3, b == 8` \t `-> x == -3`
* ` a == 3, b == -8` \t `-> x == 3`
* ` a == -3, b == 8` \t `-> x == 3`
* ` a == 4, b == 10` \t `-> x == -2, x == -3` (both are valid)

6. Check if a\*x+b=0 solvable, 32-bit int, save as one-byte true/false.<br>
function prototype
```c
void func (int32_t* a, int32_t* b, uint8_t* result, size_t size);
```
7. Solve a\*x+b=0 in 32-bit float, save results in third.<br>
function prototype
```c
void func (float* a, float* b, float* x, size_t size);
```
8. Solve a\*x+b=0 in 64-bit float, save results in third.<br>
function prototype
```c
void func (double* a, double* b, double* x, size_t size);
```
9. Find [geometric mean](https://en.wikipedia.org/wiki/Geometric_mean) and [arithmetic mean](https://en.wikipedia.org/wiki/arithmetic_mean) of given 32-bit int array<br>
function prototype
```c
void func (int32_t* input_array, size_t size, double* geometric_mean, double *arithmetic_mean);
```
10. Find [geometric mean](https://en.wikipedia.org/wiki/Geometric_mean) and [arithmetic mean](https://en.wikipedia.org/wiki/arithmetic_mean) of given 64-bit int array <br>
function prototype
```c
void func (int64_t* input_array, size_t size, double *geometric_mean, dounle *arithmetic_mean);
```
11. Find min, max, mean, variance of given 32-bit int array.<br>
function prototype 
```c
void func (int32_t* input_array, size_t size, int32_t* min, int32_t* max, double* mean, double* variance);
```
12. Find min, max, mean, variance of given 64-bit int array.<br>
function prototype
```c
void func (int64_t* input_array, size_t size, int64_t* min, int64_t* max, double* mean, double* variance);
```
13. Given 32-bit uint array check, if each number is prime or not, save as array of true/false<br>
function prototype
```c
void func (uint32_t* input_array, uint8_t* is_prime_array, size_t size);
```
14. Given 64-bit uint array check, if each number is prime or not, save as array of true/false<br>
function prototype
```c
void func (uint64_t* input_array, uint8_t* is_prime_array, size_t size);
```
15. Find sum of digits in a number <br>
function prototype
```c
uint32_t func (uint32_t number);
```

---
## Additional tasks
#### 2. (2 points) Call your asm function from Python, Rust, Pascal or other programming language
#### 1. (4 points) The same function but for one of the following ISA's: ARM Cortex A, ARM Cortex M, AVR8
#### 3. (4 points) Implement your C-strings lab (could be with fixed buffer) on assembler, and compare code performance

---
## Bonus Tasks
#### (up to two points for first two tasks, up to one for three next)

16. Given a square (NxN) matrix as a linear one-dimensional array of 64-bit uint, transpose it inplace <br>
function prototype
```c
void func (uint64_t* matrix, uint8_t size);
```

Additional tasks: \
(2 points) Use SIMD instructions to improve performance \
(2 points) Implement a recursive "cache-oblivious" algorithm.

17. Given a square (NxN) matrix as a two-dimensional array (array of arrays) of 32-bit uint, transpose it inplace <br>
function prototype
```c
void func (uint32_t** matrix, uint8_t size);
```
18. Given a pointer to a linked list, reverse it inplace. <br>
function prototype 
```c
struct linked_list_node* func (struct linked_list_node* node);
```
where `struct linked_list_node` is defined as
```c
struct linked_list_node
{
    struct linked_list_node* next;
    int32_t data;
};
```
19. Given a pointer to a linked list, reverse it inplace. <br>
function prototype 
```c
struct linked_list_node* func (struct linked_list_node* node);
```
where `struct linked_list_node` is defined as
```c
struct linked_list_node
{
    int64_t data;
    struct linked_list_node* next;
};
```
remarks: <br>
Note that the structure differs from the one given in 18th task.

20. Given an unsigned 2-byte value, convert it to a sequence of 4 ASCII characters, representing the value in hexadecimal.<br>
function prototype 
```c
void func (uint16_t number, char* chars);
```
remarks: <br>
A few examples of values and corresponding ascii: \
10 -> 000A\
17 -> 0011\
255 -> 00FF\
266 -> 010A\
57005 -> DEAD\
Don't set the `'\0'` at the end of `chars` in this function.
Don't forget about endianess of your system!

21. Given an expression in polish notation, evaluate it and return the result. <br>
function prototype
```c
int64_t func (char* expression);
```
remarks: <br>
For simplicity, we assume the following conditions are always met:
- `expression` can be evaluated
- `expression` is a valid non-null and non-empty C-string consisting of ASCII characters
- `expression` can only contain operators `+`,`-` and numbers `0-9`, encoded as ASCII characters
- 64-bit integer overflows cannot occur

In order to determine the size of the string, consider implementing a separate `strlen` procedure.

Examples of expressions and corresponding evaluation results:\
`"+11"` -> 2 \
`"-1+2-33"` -> -1 \
`"-+8-642"` -> 8 


---
## Work in progress tasks
> Note to contributors: this is a pool of other tasks the are or were considered, but either were discarded due to difficulty, or are still being worked on. It's possible to switch out some of the tasks from previous sections with these. In such case, the replaced task goes into the [Previous tasks (replaced)](#previous-tasks-replaced) section. Also, the total number of tasks must be divisible by 3, therefore new tasks must be added in triplets. 

22. Given a C-string, count the number of characters, words and lines in it. \
function prototype
```c
void func (char *string, uint64_t *chars, uint64_t *words, uint64_t *lines)
```
Remarks:
For simplicity, we assume the following conditions are always met:
- All pointers are non-null. The function should set `chars`, `words` and `lines` accordingly before it returns.
- `string` is always a valid C-string. However, it can be an empty string `""`.
- The only space character this function should detect is `' '`, which is `0x20` in hex. All other sybols, except `'\n'` and `\0` can be assumed to be letters.

In all other cases behaviour of this function should be similar to the standard UNIX command line tool `wc`. Use it to verify the correctness of your function.

A few common pitfalls:
- all characters, even `' '` and `'\n'` should be counted. For example, string `" \n \n"` should result in `chars = 4; words = 0; lines = 2`.
- `lines` represents the number of `'\n'` characters in your string. This means that for a string `"Hello, world!"`, the result should be `lines = 0`, while for `"Hello, world!\n"` it should be `lines = 1`.
- The number of words does not correspond to the number of spaces. For example, `" \n hello\n"`, which contains two spaces, results in `words = 1; chars = 9; lines = 2`.

23. Given arrays with coefficients for line equations and coordinates of a point P in 2D Euclidian space, find the distance to each line. \
function prototype
```c
void func (int32_t p_x, int32_t p_y, int32_t *a, int32_t *b, int32_t *c, float *distances, uint64_t size)
```
Tips:
- In order to compute the reciprocal of square root <img src="https://render.githubusercontent.com/render/math?math=\frac{1}{\sqrt{x}}">, use the `rsqrtss` instruction. The result is approximate, but that is acceptable in this task. Do not forget to load your desired operand to a 32-bit float beforehand.

24. Given arrays with coefficients for line equations and coordinates of a point P in 2D Euclidian space, return the index of a line, to which P is the closest. \
function prototype
```c
uint64_t func (int32_t p_x, int32_t p_y, int32_t *a, int32_t *b, int32_t *c, uint64_t size)
```
Tips:
- In order to compute the reciprocal of square root <img src="https://render.githubusercontent.com/render/math?math=\frac{1}{\sqrt{x}}">, use the `rsqrtss` instruction. The result is approximate, but that is acceptable in this task. Do not forget to load your desired operand to a 32-bit float beforehand.
- To compare two 32-bit float values in `xmm` registers, use the `comiss` instuction combined with `ja/jb` conditional jump instructions. For more info, see [this](https://stackoverflow.com/a/30563286) stackoverflow answer. 

25. Given two arrays of points A and B, pairs of which define lines <img src="https://render.githubusercontent.com/render/math?math=\overline{AB_i}%3D(A_i%2C%20B_i)">, and coordinates of a point P in 2D Euclidian space, return the index of the line to which P belongs. If P is not on any of the lines, return -1 \
function prototype
```c
int64_t func (int64_t *a_x, int64_t *a_y, int64_t *b_x, int64_t *b_y, uint64_t size, int64_t p_x, int64_t p_y)
```

26. Given two arrays a and b of 32-bit signed fixed point values with radix point positions given in r_a and r_b, compute their sum and store the result in an array of 64-bit signed **Q**<img src="https://render.githubusercontent.com/render/math?math=\min(r_a,%20r_b)"> fixed point values. \
function prototype
```c
void func (int32_t* a, int32_t* b, uint8_t r_a, uint8_t r_b, int64_t* results, uint64_t size)
```
Hints:
- **Q**f notation means a fixed point value with ``f`` least significant bits used for the fractional part and the rest for the integer part. The radix point is the bit position of the "dot" which divides the integer and the fractional part. \
For example, 32-bit signed **Q**16 Fixed-point values, in general, are just integers where 16 least significant bytes are used for the fractional part, the most significant bit is the sign bit, and 15 bits between the sign bit and the start of the fractional part are used for the integer part
- When moving from 32-bit to 64-bit registers, don't forget to sign-extend

27. Given two arrays a and b of 32-bit signed **Q**20 fixed-point values, compute their products and store the results in an array of same type \
function prototype
```c
void func (int32_t* a, int32_t* b, int32_t* results, uint64_t size)
```
remarks: \
For simplicity, we assume that overflows that occur when the 12-bit integer part is too big (e.g. `2048.0 * 4.0`) can be ignored. However, this does not mean that overflows cannot occur in the entire 32-bit fixed-point value! Take a look at the documentation of `mul`, `sar` and `shrd` instructions in order to better understand implications of this and how to handle them. 

Hint: \
Consider initially decreasing the accuracy of fixed-point values by shifting both by `10` and then proceeding with the multiplication.   

28. Given an array of 32-bit float point values, convert them into 32-bit **Q**16 fixed point values without using float multiplication. \
function prototype
```c
void func (float* floats, int32_t* fixed, uint64_t size)
```
remarks: \
For detailed discussion about how such function should behave, see [this](https://stackoverflow.com/a/65974190) stackoverflow answer. However, note that assembly implementation will somewhat differ, as some instructions simplify this problem.

29. Given an array of 16-bit uint that each represent [15,11] Hamming codes with parity check (SECDED), correct single-bit errors by changing the appropriate bit and detect two-bit errors by setting a boolean flag in another array.\
function prototype
```c
void func (uint16_t* codes, uint8_t* two_errors, uint64_t size)
```
Hint: Error correction and detection can be achieved only by using a few shift and `xor` instructions. Do not attempt to use the matrix method! In case you need a refresher about how to work with hamming codes, consider watching 3blue1brown's [video](https://www.youtube.com/watch?v=X8jsijhllIA) on this topic. If you want to know how such error correction can be implemented in hardware, consider [this](https://www.youtube.com/watch?v=h0jloehRKas) video as well.

30. Given an andjacency matrix of an undirected graph with n nodes, check whether it is a connected graph and return 0 or 1 accordingly.\
function prototype
```c
uint8_t func (uint8_t* adjacency, uint8_t nodes)
```
The matrix is given as a linear 2D array of size n*n.\
Hints:
- A connected graph is a graph where there is a path from any node to any other node.
- Use DFS algorithm to traverse all nodes in the graph from some starting point. Push pointers to nodes onto the stack.
- Create a local array of size `nodes`, where each element is a boolean flag that shows whether or not the node was already visited.
- To create a local array, do not attempt to allocate it dynamically! Static arrays in `.data` are allowed, but discouraged. The best approach is to allocate it on a stack. In order to properly index it without affecting the DFS stack, consider using `rbx` stack frame variable.
- Dont forget about stack alignment.
