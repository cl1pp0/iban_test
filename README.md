# IBAN Test
Tests IBANs for validity. Uses the GNU multiple precision arithmetic library (GNU MP) for calculation.

DISCLAIMER:<br>
This is intended for educational purpose, not for production or serious financial use. No warranty.

## Compile with:
```
$ gcc -o iban-test iban-test.c -lgmp
```

## Usage:
```
$ ./iban_test <iban>
```
Tests &lt;iban&gt; for validity.
```
$ ./iban_test -h
```
Prints help screen.

## Example:
```
$ ./iban-test DE07123412341234123412
IBAN "DE07123412341234123412" is valid
```
