# Computer Science I
## Lab 7.0 - Arrays
[School of Computing](https://computing.unl.edu)  
[College of Engineering](https://engineering.unl.edu/)  
[University of Nebraska-Lincoln](https://unl.edu)  
[University of Nebraska-Omaha](https://unomaha.edu)  

This lab introduces arrays and dynamic memory using the C programming
language.

## Prior to Lab

* Read and familiarize yourself with this handout.
* Read the required chapters(s) of the textbook as
  outlined in the course schedule.

In addition we recommend the following supplemental material:

* Read Chapters 7 and 20 of [Computer Science I](https://bitbucket.org/chrisbourke/computersciencei/raw/5a04789d5f54b9f8df9f27e7087e4b33b237275f/ComputerScienceOne.pdf)
* Watch [Videos 7.1 thru 7.7](https://www.youtube.com/playlist?list=PL4IH6CVPpTZVkiEnCEOdGbYsFEdtKc5Bx)

## Peer Programming Pair-Up

***For students in the online section:*** you may complete
the lab on your own if you wish or you may team up with a partner
of your choosing, or, you may consult with a lab instructor to get
teamed up online (via Zoom).

***For students in the on campus section:*** your lab instructor
may team you up with a partner.  

To encourage collaboration and a team environment, labs are be
structured in a *peer programming* setup.  At the start of
each lab, you will be randomly paired up with another student
(conflicts such as absences will be dealt with by the lab instructor).
One of you will be designated the *driver* and the other
the *navigator*.  

The navigator will be responsible for reading the instructions and
telling the driver what to do next.  The driver will be in charge of the
keyboard and workstation.  Both driver and navigator are responsible
for suggesting fixes and solutions together.  Neither the navigator
nor the driver is "in charge."  Beyond your immediate pairing, you
are encouraged to help and interact and with other pairs in the lab.

Each week you should alternate: if you were a driver last week,
be a navigator next, etc.  Resolve any issues (you were both drivers
last week) within your pair.  Ask the lab instructor to resolve issues
only when you cannot come to a consensus.  

Because of the peer programming setup of labs, it is absolutely
essential that you complete any pre-lab activities and familiarize
yourself with the handouts prior to coming to lab.  Failure to do
so will negatively impact your ability to collaborate and work with
others which may mean that you will not be able to complete the
lab.  

## Lab Objectives & Topics

By the end of this lab you should
* Understand what memory leaks are, how they're caused,
  and how to prevent them
* Know how to work with arrays: how to create them, use them,
  pass and return them from functions
* Understand the relationship between arrays and pointers

### Background

Arrays are ordered collections that hold multiple values of a
certain type (`int` or `double` values for example).  
Individual *elements* in an array can be accessed using an
*index* value and the square bracket `[]` syntax.  For example,
suppose we have an `int` array named `arr`. The first element
is at index `0` and can be accessed using `arr[0]`, the second
is at index `1` and can be accessed using `arr[1]`, etc.  If
there are `n` elements in the array the last one would be at
index `n-1`: `arr[n-1]`.  This is known as zero-indexing.

You should take care that you do not access elements beyond
the range of the array's indices.  Attempts to access
elements outside this range is *undefined behavior* and
may result in unexpected results, a segmentation fault
or other run time error.  

To avoid undefined behavior, it is necessary to do your own
**bookkeeping**: you must manually keep track of the size
of an array in a separate (`int`) variable and use it.

#### Static Arrays

Static arrays are arrays whose size is specified at compile
time by hardcoding the size when declared.  For example:

```c
int array[100];
```

declares and creates an `int` array that holds 100 integers.
Static arrays are allocated and stored on the *program stack*
and so their use is extremely limited: such arrays cannot be
too big (or it would result in a *stack overflow*) and they
cannot be returned from a function since the stack frame is
popped off and destroyed when the function returns.  

#### Dynamic Arrays

Dynamic arrays are allocated on the program heap and are
much more versatile.  

The size of dynamic arrays are not specified at compile
time.  At runtime, a chunk of memory is dynamically allocated
to accommodate the space we need to hold a certain number
of values.  To create a dynamic array, you use the
`malloc()` (**m**emory **allo**cation) function.  The
full signature of this function is

`void * malloc(size_t size);`

* `size` is the **number of bytes** you are requesting `malloc()` to
  allocate (`size_t` can be thought of simply as an integer)
* Upon success, `malloc()` returns a *generic `void` pointer*: a
  pointer to the chunk of memory it allocated for you.  By
  returning a generic pointer, one function can be used for
  *any* type of memory allocation (`int`, `double`, etc.).
* In general, you should typecast the generic pointer to the
  type of array you are using (ex: `(int *)` or `(double *)`)
* To determine how many bytes you need to allocate, you can use
  the `sizeof()` macro which results in the number of bytes
  any type requires (ex: `sizeof(int)` or `sizeof(double)`)
* If `malloc()` fails for any reason, it returns `NULL`

Here is a full example of the proper usage of `malloc()`:

```c
//create a dynamic integer array of size 10
int n = 10;
int *arr = (int *) malloc(sizeof(int) * n);
arr[0] = 42;
...
arr[9] = 101;
```

As you can see, once created, you can use the same square
bracket and indexing syntax as before.  Another example
involving `double` values:

```c
//create a dynamic double array of size 20
int n = 20;
double *arr = (double *) malloc(sizeof(double) * n);
arr[0] = 42.5;
...
arr[19] = 101.9;
```

#### Memory Management

In addition to keeping track of the size of an array, you
also need to manually *manage* the memory you allocate.
Memory is a limited resource, once you are done using it
you should be responsible and "clean up" by giving the
memory back so that it can be reused (either by your
own program or the operating system).  

You can deallocate or release memory by using the
`free()` function and passing it the pointer to the
memory you want to free:

`free(arr);`

Failure to release unneeded memory may result in a *memory leak*
whereby a program continually allocates memory but never releases
it, becoming a resource hog.  Eventually, the program uses so much
memory that it slows to a halt or is terminated as the operating
system is unable to allocate more resources to it.

#### 2-D Arrays

You can create two-dimensional arrays to hold *tables* (or
matrices) of data in rows/columns.  To create a 2-D array,
you can use a pointer-to-pointer syntax; example: `int **`.
The idea is that this points to an *array of pointers* and
each of those pointers points to an array of integers (which
can be interpreted as a *row*).  An example:

```c
//create a 10 x 20 table of integers:
int n = 10;
int m = 20;
//create an array of n pointers:
int **table = (int **) malloc(sizeof(int *) * n);
for(int i=0; i<n; i++) {
  //create the i-th row of 20 integers
  table[i] = (int *) malloc(sizeof(int) * m);
}
```

Once created, you can use two indices to access values:
```c
//top-left most element:
table[0][0] = 42;
//bottom-right most element:
table[9][19] = 101;
```

When freeing the memory, you have to work backwards and
free each row before you free the array of pointers:
```c
for(int i=0; i<n; i++) {
  free(table[i]);
}
free(table)
```



# Activities

Clone the repository from GitHub containing the code for this lab by using
the following URL: https://github.com/cbourke/CSCE155-C-Lab07

## Fixing a Memory Leak

In this exercise, we'll observe and fix a memory leak in action.

**Instructions**

1. Change to the `memoryLeak` directory
2. Build the executable using the `make` command.  Make is a
   utility that reads a `makefile`--a specification for source
   file dependencies and the process by which they are built.  
   The `makefile` for this project has been provided for you.
3. A program named `memLeak` will be created in your directory.  
   You can run it as follows:

   `./memLeak 1000000 10`

   which will run the program and create 10 random arrays each
   of 1 million integer elements.  It then sorts the array
   and reports the median element.  The program will execute
   rather quickly so it will be difficult to observe the memory
   leak.
4. To diagnose the memory leak so we can fix it, we'll use a
   *dynamic analysis* tool called [Valgrind](http://valgrind.org).
   A dynamic analysis tool is a program that can run and
   monitor other programs to analyze the the resources it uses
   and call attention to errors and potential errors that may
   occur at *runtime*.  

   Run the Valgrind tool using the following command:

   `valgrind --leak-check=full --show-leak-kinds=all ./memLeak 1000000 10`

   This starts Valgrind and runs your program with it.
5. Once Valgrind is done, it should produce a report that
   includes a total number of bytes that was lost due to
   the memory leak as well as where the memory was originally
   allocated (though not where it was lost).  
6. Using this report as a guide, fix the memory leak by
   freeing the appropriate memory.  Recompile and rerun
   Valgrind to verify the leak has been fixed.

## Static to Dynamic Arrays

Navigate to the `exercises` directory.  Several starter files
have been provided for you.  The starter demo file `array_demo.c`
can be used to perform ad-hoc tests as you write your functions.
Recall that you can compile these files using:
```text
gcc -c array_utils.c
gcc array_utils.o array_demo.c
```
*or*: we have provided a `makefile` that you can use (simply
type `make array_demo`).

The program, as provided, generates an integer array
of size 10 filled with random values, prints the result
and then frees up the memory.  The problem is that the
`generateRandomArray()` function uses a static array.

**Instructions**

1. Compile and run the program and observe the results.
2. Fix the `generateRandomArray()` function to properly use
   a dynamic array.  Recompile and run the program to make
   sure your fix works.

## Protecting Elements

When passing an array to a function, it is passed as a pointer
or *passed by reference*.  Consequently, a function may make
changes to the elements in the array.  You probably noticed
this when you ran the demo program: the `getSum()` function
successfully summed the elements in the array but also zeroed
out all the elements in the array (observe the second time
the array was printed).  Let's fix this.

**Instructions**

1. In both the `array_utils.c` and `array_utils.h` files,
   change the signature of the `getSum()` function to:

   `int getSum(const int *arr, int size)`

   The `const` keyword (short for *constant*) makes the
   array a read-only array.  The compiler will detect if
   the function attempts to make changes and raise a
   compile-time error if it does.
2. Try to recompile the files and observe the compiler error.
3. Change the `getSum()` function and remove the line of
   code that changes the elements.  Recompile and verify that
   the array is no longer zeroed out.

## Error Handling

Observe how error handling was implemented in the functions
we wrote.  If a function is designed to return an error code,
of course it should do so appropriately.  However, even if
a function is not designed to return an error code, basic
defensive programming and error handling still need to be
implemented.  In general:

* If the function is a `void` function it should perform a
  noop (no operation) and immediately return
* If a function returns a boolean (`int`), it should return
  a reasonable default value
* If a function returns a pointer, it should return `NULL` for
  any error conditions; this is really the only thing that *can*
  be returned

## More Exercises

You will now get some practice by implementing several more
utility functions.  For each function below:
1. Write documentation in the `array_utils.h` file in your own
   words so you have an understanding of what it does.
2. Implement the function in the `array_utils.c` file.
3. You can do ad-hoc testing using the `array_demo.c` or
   you can run a much larger cmocka test file (compile
   it using `make array_test`) that will be used for grading.

Look for ways to make your life easier: some functions may
be able to utilize others.

* `double getMean(const int *arr, int size)` - computes the
  average of elements in `arr`

* `int getMin(const int *arr, int size)` - returns the minimum
  element in `arr`

* `int getIndexOfMin(const int *arr, int size)` - returns
  the *index* of the the minimum element in `arr`

* `int getMax(const int *arr, int size)` - returns the maximum
  element in `arr`

* `int getIndexOfMax(const int *arr, int size)` - returns
  the *index* of the the maximum element in `arr`

* `int * filterThreshold(const int *arr, int size, int threshold, int *resultSize)` -
  creates and returns a new array that contains
  elements in `arr` that are greater than or equal to `threshold`.
  The size of the returned array is communicated through the
  pass-by-reference variable, `resultSize`.  For example, if we
  pass the array
  `[10, 5, 32, 8, 7, 28, 15, 12]`
  with `threshold = 10` then the returned array should be
  `[10, 32, 28, 15, 12]`
  and the `resultSize` would be set to `5`.

* `int **createMultiplicationTable(int n, int m)` - creates and
  returns a new `(n x m)` 2-D array that contains the values in
  a multiplication table.  For example, if `n = 3, m = 5` then
  the result should look like:
```text
[ 1   2   3   4   5 ]
[ 2   4   6   8  10 ]
[ 3   6   9  12  15 ]
```  

## Handin/Grader Instructions

Compile and run the `cmocka` tests and make sure they all pass.  
Hand in your completed source file, `array_utils.c` and
header file `array_utils.h` through the handin and verify
your program is correct by using the grader.

## Advanced Activity (Optional)

Read more about the capabilities of [Valgrind](http://valgrind.org/),
(a suggested [tutorial](http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Debugging.html#Valgrind)) and how to use it.  
Think of other situations that this tool may have come in
handy and try using it for the next bug that you encounter.  
