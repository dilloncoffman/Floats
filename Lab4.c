#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <float.h>

#define NORM 0
#define DNORM 1
#define SPEC 2
#define BIAS 127

typedef struct Float {
    int sign;
    unsigned int exp;
    float mantis;
    int mode;
}flt;
/*
 Declare a "typedef struct {} flt;" to contain data for a float
 The data should include:
 An integer for the sign
 (1 for positive, -1 for negative)
 An integer for the exponent value
 (should be bit value to integer minus BIAS or -126 for denormalized)
 A float to contain the value of the mantissa
 (Actual float value extracted from the binary value)
 An integer to contain the mode using the defines above
 (NORM, DNORM and SPEC)
 Example after processing: -15.375 = 1 10000010 11101100000000000000000
 sign = -1
 exp = 3
 man = 0.9218750000
 mode = NORM
 */




/*
 Write a function get_flt_bits_int to return an integer with the
 bits copied from a float.
 Example:
 for f = -15.375,
 the bits of int n = 11000001011101100000000000000000
 Look at the slides and code from the float lectures and use the
 indirection trick.  This can easily be done in one line of code.
 The function should accept a float and return an int.
 */
int get_flt_bits_int(float f){
    return *(int*)&f;
}




/*
 Write a function that returns the sign of a float as a char.
 You should call get_flt_bits_int to get the bits in an int
 and return '1' if the sign is negative else return '0'.  The
 function should accept a float and return a char.
 */

char get_flt_sign_char(float f){
    int bits = get_flt_bits_int(f);
    char sign = '\0';
    if(bits >> 31 == 0){ //if shifted bits = 0, positive
        sign = '0';
    } else {
        sign = '1'; //if shifted bits are not = 0, negative
    }
    return sign;
}





/*
 Write a function that returns the sign of a float as an integer.
 You should call get_flt_bits_int to get the bits in an int
 and return -1 if the sign is negative else return 1.  The function
 should accept a float and return an int.
 */
int get_flt_sign_val(float f){
    int bits = get_flt_bits_int(f);
    int sign = 0;
    if(bits >> 31 == 0){ //if shifted bits = 0, positive
        sign = 1;
    } else {
        sign = -1; //if shifted bits are not = 0, negative
    }
    return sign;
}



/*
 Write a function to return a string containing the
 actual binary value of the exponent of a float in a
 char array.  You should call get_flt_bits_int to get
 the bits in an int and return the string.
 Example:
 for f = -15.375
 n = 11000001011101100000000000000000
 the exponent bits are "10000010"
 The function should accept a float and return a string.
 */
char* get_flt_exp_str(float f){
    
    int bits = get_flt_bits_int(f); //get the bits for the entire float
    bits >>= 23;
    
    char *output = NULL; //use char pointer
    int size = 9; //size is 9 because of 8 bits for exponent and 9 for null at end of string
    output = (char*)malloc(sizeof(char)*size);
    
    int i;
    for(i = 7; i >= 0; i--){
        if(bits & 1){ //if bit is a 1, add a 1 to the char array
            output[i] = '1';
        }
        else { //otherwise, add a 0
            output[i] = '0';
        }
        bits >>= 1; //pop bit off the end to check next bit
    }
    output[8] = '\0'; //add null at end of char array
    return (output);
}
/*
 Write a function to return an integer containing the
 actual integer value of the exponent of a float.  You
 should call get_flt_bits_int to get the bits in an int
 and return the int with the exponent value.
 Example:
 for f = -15.375
 n = 11000001011101100000000000000000
 the exponent bits are 10000010
 the actual value of the exponent is 3
 The function should accept a float and return an int.
 */
int get_flt_exp_val(float f){
    int bits = get_flt_bits_int(f);
    bits >>= 23;
    
    //need to shift from left to right and & 1 checks for bit in that position, if so, then add it
    int m = 1;
    int i = 0;
    int res = 0;
    for(i=0; i<8; i++){
        if(bits & 1) //if that bit is a 1, add current value of multiplier to res
            res += m;
        m *= 2; // increase multiplier to appropriate position
        bits >>= 1; //go to next bit to check
    }
    return res - BIAS;
}

/*
 Write a function to return an integer containing the
 mode of the exponent of a float.  You should call
 get_flt_exp_val to get the bits in an int and return
 the int with the mode value.
 Example:
 for f = -15.375
 n = 11000001011101100000000000000000
 the exponent bits are 10000010
 the mode is NORM
 The function should accept a float and return an int.
 */
int get_flt_exp_mode(float f){
    int exp = get_flt_exp_val(f);
    //if exponent has all 1's return SPEC
    if(exp == 128){ //128 because 255 - the bias of 127 = 128
        return SPEC;
    }
    //if exponent has all 0's return DNORM
    if(exp == -127){
        return DNORM;
    } else { //exponent has 1's and 0's return NORM
        return NORM;
    }
    
}






/*
 Write a function to return a string containing the
 actual binary value of the mantissa of a float in a
 char array.  You should call get_flt_bits_int to get
 the bits in an int and return the string.
 Example:
 for f = -15.375
 n = 11000001011101100000000000000000
 the mantissa bits are "11101100000000000000000"
 The function should accept a float and return a string.
 */
char* get_flt_man_str(float f){
    int bits = get_flt_bits_int(f); //get the bits for the entire float
    bits >>= 9; //shift all bits over
    bits <<= 9; //shift bits left 9 to knock off sign bit and exponent bits, replacing end with 0's
    
    char *output1 = NULL; //use char pointer
    int size = 24; //size is 24 because of 23 bits for mantissa and 24 for null at end of string
    output1 = (char*)malloc(sizeof(char)*size); //malloc appropriate space
    
    int i; //counter
    for(i = 22; i >= 0; i--){
        if(bits & 1){ //if the current bit is a 1, put a 1 into the output char array
            output1[i] = '1';
        }
        else { //otherwise, put a 0 in that position
            output1[i] = '0';
        }
        bits >>= 1; //pop the bits off the end to check next bit
    }
    output1[23] = '\0'; //put null at end of string
    return (output1);
}




/*
 Write a function to return a float containing the
 actual float value of the mantissa of a float.  You
 should call get_flt_bits_int to get the bits in an int
 and return the float with the mantissa value.
 Example:
 for f = -15.375
 n = 11000001011101100000000000000000
 the mantissa bits are 11101100000000000000000
 the actual value of the mantissa is 0.9218750000
 The function should accept a float and return a float.
 */
float get_flt_man_val(float f){
    //get the mantissa bit string
    char* manString = get_flt_man_str(f);
    //m is multiplier, i a counter for the loop
    float m = 2.0;
    int i;
    float sum = 0;
    //starting at i=0, for i <=22, perform appropriate operation to get value of mantissa
    for(i=0; i<=22; i++){
        sum += (1.0/(m))*(manString[i]-48);
        m *= 2; //multiplier *= 2, so as to properly calculate next bit to be added to sum
    }
    return sum;
}




/*
 Write a function to return a string containing the
 actual binary value of a float in a char array.  You
 should call get_flt_sign_char, get_flt_exp_str and
 get_flt_man_str to get the bits in an char and two
 strings and return the concatenated string.
 Example:
 for f = -15.375
 n = 11000001011101100000000000000000
 The sign is '1'
 the exponent is "10000010"
 and the mantissa bits are "11101100000000000000000"
 The string should be formatted as:
 "1 10000010 11101100000000000000000" to clearly
 separate the 3 parts.
 The function should accept a float and return a string.
 */
char* get_flt_bits_str(float f){
    //get all appropriate bit strings necessary
    char sign = get_flt_sign_char(f);
    char* exp = get_flt_exp_str(f);
    char* man = get_flt_man_str(f);
    char* output = NULL;
    int size = 35; //make room for spaces and null at end
    output = (char*)malloc(sizeof(char)*size);
    int i = 0;
    int j = 1;
    
    output[0] = sign;
    output[1] = ' ';
    for(j = 1; exp[j-1] != '\0'; j++, i++){ //add exponent string to entire float string with a space before
        output[j+1] = exp[i];
    }
    output[10] = ' ';
    for(j = 0; man[j] != '\0'; j++, i++){ //add mantissa to entire float string with a space before
        output[i+3] = man[j];
    }
    output[34] = '\0';
    return output;
    
}




/*
 Write a function to separate the parts of a float
 into a flt struct as described above.  You should
 call get_flt_sign_val, get_flt_exp_mode,
 get_flt_exp_val and get_flt_man_val.
 Hint:  make sure to set exponent to -126 for
 DNORM mode.
 */
struct Float get_flt_val_flt(float f){
    //define a Float struct to be used
    struct Float flt;
    //get all parts of float value assigned
    flt.sign = get_flt_sign_val(f);
    flt.exp = get_flt_exp_val(f);
    flt.mantis = get_flt_man_val(f);
    //check mode, if DNORM exponent = -126
    if(get_flt_exp_mode(f) == NORM){
        flt.mode = NORM;
    }
    if(get_flt_exp_mode(f) == DNORM){
        flt.mode = DNORM;
        flt.exp = -126;
    }
    if(get_flt_exp_mode(f) == SPEC){
        flt.mode = SPEC;
    }
    return flt; //return newly created flt struct to be used in get_flt_bits_val
    
}



/*
 Write a function to print a flt struct to screen.
 It should accept a flt struct and return nothing.
 Hint: Use if statement to print mode.
 */
void print_flt(float f){
    //define a Float struct to use and call get_flt_val_flt to populate the parts of the float value passed in to be printed
    struct Float flt = get_flt_val_flt(f);
    //print all parts of that float
    printf("sign = %i\n", flt.sign);
    printf("exp = %i\n", flt.exp);
    printf("man = %f\n", flt.mantis);
    //check mode of float, print the mode
    if(flt.mode == NORM){
        printf("mode = normalized\n\n");
    }
    if(flt.mode == DNORM){
        printf("mode = denormalized\n\n");
    }
    if(flt.mode == SPEC){
        printf("mode = specialized\n\n");
    }
}



/*
 Write a function to get the actual float value back
 out of a flt struct.
 Hints:
 The float value produced will depend on the mode.
 To set a float to infinity use the math library constant INFINITY
 To set a float to not-a-number use the math library constant NAN
 Check the slides and text for conditions for NORN, DNORM and SPEC
 You need to return (sign) * M * 2^e
 */
float get_flt_bits_val(float f){
    //define a Float struct to use and call get_flt_val_flt to populate the parts of the float value passed in
    struct Float flt = get_flt_val_flt(f);
    //if exponent is all 1's or 255, return sign * INFINITY
    if(flt.exp == 255 || flt.mantis == 0.000000){
        return (flt.sign) * INFINITY;
    }
    //if mantissa is an imaginary number
    if(flt.mantis == 0.500000){
        return NAN;
    }
    //if DNORM don't add 1 to mantissa
    if(flt.mode == DNORM){
        return (flt.sign) * flt.mantis * powf(2, flt.exp);
    }
    //otherwise it is NORM so return 1 + mantissa
    return (flt.sign) * (1+flt.mantis) * powf(2, flt.exp);
}




/*
 Write a main function that calls an prints results for
 each function when completed.
 get_flt_sign_char
 get_flt_sign_val
 
 get_flt_exp_str
 get_flt_exp_val
 
 get_flt_man_str
 get_flt_man_val
 
 get_flt_bits_str
 
 get_flt_val_flt
 print_flt
 
 get_flt_bits_val
 */
int main(){
    
    float f = -15.375;
    printf("f = %f\n\n", f);
    printf("sign as char = %c\n", get_flt_sign_char(f));
    printf("sign as int = %i\n\n", get_flt_sign_val(f));
    printf("exp string = %s\n", get_flt_exp_str(f));
    printf("exp value = %i\n\n", get_flt_exp_val(f));
    printf("man = %s\n", get_flt_man_str(f));
    printf("m = %f\n\n", get_flt_man_val(f));
    printf("bits = %s\n\n", get_flt_bits_str(f));
    print_flt(f);
    
    
    printf("ff = %f\n", get_flt_bits_val(f));
    
    return 0;
}

