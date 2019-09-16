/**
 * CS 2110 Fall 2018 HW2
 * Part 2 - Coding with bases
 *
 * @author Bojun Yang
 *
 * Global rules for this file:
 * - You may not use more than 2 conditionals per method. Conditionals are
 *   if-statements, else-if statements, or ternary expressions. The else block
 *   associated with an if-statement does not count toward this sum. Boolean
 *   expressions outside of if-statements, else-if statements and ternary
 *   expressions do not count toward this sum either.
 * - You may not use more than 2 looping constructs per method. Looping
 *   constructs include for loops, while loops and do-while loops.
 * - You may not use nested loops.
 * - You may not declare any file-level variables.
 * - You may not declare any objects, other than String in select methods.
 * - You may not use switch statements.
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any other method from this or
 *   another file to implement any method.
 * - The only Java API methods you are allowed to invoke are:
 *     String.length()
 *     String.charAt()
 * - You may not invoke the above methods from string literals.
 *     Example: "12345".length()
 * - When concatenating numbers with Strings, you may only do so if the number
 *   is a single digit.
 *
 * Method-specific rules for this file:
 * - You may not use multiplication, division or modulus in any method, EXCEPT
 *   decimalStringToInt.
 * - You may declare exactly one String variable each in intToBinaryString and
 *   and intToHexString.
 */
public class Bases
{
    /**
     * Convert a string containing ASCII characters (in binary) to an int.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid binary numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: binaryStringToInt("111"); // => 7
     */
    public static int binaryStringToInt(String binary)
    {
        int result = 0;
        for (int i = 0; i < binary.length(); i++) {
            if (binary.charAt(i) == '1') {
                result += (0x1 << (binary.length() - i - 1));
            }
        }
        return result;
    }

    /**
     * Convert a string containing ASCII characters (in decimal) to an int.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid decimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: decimalStringToInt("123"); // => 123
     *
     * You may use multiplication, division, and modulus in this method.
     */
    public static int decimalStringToInt(String decimal)
    {
        int result = 0;
        int power = 1;
        for (int i = decimal.length() - 1; i >= 0; i--) {
            result += (decimal.charAt(i) - '0') * power;
            power = power * 10;
        }
        return result;
    }

    /**
     * Convert a string containing ASCII characters (in hex) to an int.
     * The input string will only contain numbers and uppercase letters A-F.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid hexadecimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: hexStringToInt("A6"); // => 166
     */
    public static int hexStringToInt(String hex)
    {
        int result = 0;
        int hexint;
        for (int i = hex.length() - 1; i >= 0; i--) {
            hexint = hex.charAt(i) - '0'; // 0 - 9
            if (hex.charAt(i) > '9') {
                hexint = hex.charAt(i) - 'A' + 10;
            }
            result += hexint << ((hex.length() - 1 - i) << 2);
        }
        return result;
    }

    /**
     * Convert a int into a String containing ASCII characters (in binary).
     * You do not need to handle negative numbers.
     * The String returned should contain the minimum number of characters necessary to
     * represent the number that was passed in.
     *
     * Example: intToBinaryString(7); // => "111"
     *
     * You may declare one String variable in this method.
     */
    public static String intToBinaryString(int binary)
    {
        String out = "";
        int mask = 0x1;
        while (binary > 0) {
            if ((binary & mask) == 0x1) {
                out = "1" + out;
            } else {
                out = "0" + out;
            }
            binary = binary >> 1;
        }
        if (out.length() == 0) {
            return "0";
        }
        return out;
    }

    /**
     * Convert a int into a String containing ASCII characters (in hexadecimal).
     * The output string should only contain numbers and uppercase letters A-F.
     * You do not need to handle negative numbers.
     * The String returned should contain the minimum number of characters necessary to
     * represent the number that was passed in.
     *
     * Example: intToHexString(166); // => "A6"
     *
     * You may declare one String variable in this method.
     */
    public static String intToHexString(int hex)
    {
        String out = "";
        int mask = 0xF;
        int newhex;
        while (hex > 0) {
            newhex = (hex & mask);
            if (newhex > 9) { //A - F
                out = (char) ('A' + (newhex - 10)) + out;
            } else { // 0 - 9
                out = (char) ('0' + newhex) + out;
            }
            hex = hex >> 4;
        }
        if (out.length() == 0) {
            return "0";
        }
        return out;
    }
}
