#!/usr/bin/perl

# Generating Equivalence Classes Script for CS220 AIG Galois Rewriting Project
#
# This script is used as a supplement to generate equivalence classes between
# different boolean functions.
#
# Bryan Duane Rowe II <browe001@ucr.edu>
# Eric Clay <eclay003@ucr.edu>
# Aditya Swarup <aswar002@ucr.edu>
# Alex Rogers <aroge005@ucr.edu>

use strict;
use warnings;

#
# Returns a 1 or a 0 based on AND logic.
#
sub AndLogic
{
	my ($left, $right) = @_;
	if ($left eq "0" && $right eq "0") {
		return "0";
	}
	
	elsif ($left eq "0" && $right eq "1") {
		return "0";
	}
	
	elsif ($left eq "1" && $right eq "0") {
		return "0";
	}
	
	elsif ($left eq "1" && $right eq "1") {
		return "1";
	}
	
	else {
 		die("Invalid input for AND logic: $left $right");
	}
}

#
# Returns a 1 or a 0 based on literal boolean expression.  Simplifies.
# Only valid boolean function formats are found in @functions.
# Must be in literal form.  No variables.  Example: (10')'(11')
#
sub ComputeBooleanFunction
{
	my ($booleanFunc) = @_;
	
	# Replace all non () negations.  Example: 1'0' -> 01
	$booleanFunc =~ s/1'/0/g;
	$booleanFunc =~ s/0'/1/g;
	
	# Replace all () expressions with actual values.
	$booleanFunc =~ s/\(1\)/1/g;
	$booleanFunc =~ s/\(0\)/0/g;
	$booleanFunc =~ s/\(1\)'/0/g;
	$booleanFunc =~ s/\(0\)'/1/g;
	$booleanFunc =~ s/\(11\)'/0/g;
	$booleanFunc =~ s/\(10\)'/1/g;
	$booleanFunc =~ s/\(01\)'/1/g;
	$booleanFunc =~ s/\(00\)'/1/g;
	$booleanFunc =~ s/\(00\)/0/g;
	$booleanFunc =~ s/\(01\)/0/g;
	$booleanFunc =~ s/\(10\)/0/g;
	$booleanFunc =~ s/\(11\)/1/g;
	
	$booleanFunc =~ s/1'/0/g;
	$booleanFunc =~ s/0'/1/g;
	
	# If contains ()
	if ($booleanFunc =~ /\(/ && $booleanFunc =~ /\)/) {
		die("You can't have more than 2 bits in parentheses");
	}
	else {
		while (length($booleanFunc) > 1 )  {
			my $combine = AndLogic(substr($booleanFunc, 0, 1), substr($booleanFunc, 1, 1));
			$booleanFunc = $combine . substr($booleanFunc, 2);
		}
		return $booleanFunc;
	}
}

#
# Truth table info:
# Input: Boolean function
# Output: "Truth table string"
#
# The truth table string will consist of the output of the boolean function based
# on different input combinations.  There is one output bit per input combination.
# The input combination will be counting up from binary 0000 until 1111 (depending
# on the number of variables).  If there are 2 variables, it counts up from 00 to 11.
# The output will be in this order as well.
#
# For example, for a 2-variable boolean function ab:
# The boolean function is ab.
# The input combination is 00, 01, 10, 11.
# The output truth table string is "0001" (the first 0 is for "00", the second 0 is for "01", the third 0 is for "10", the last bit 1 is for "11")
# The order is important.
#
sub GenerateTruthTableString
{
	my ($func) = @_;
	
	my $tts = "";
	
	# Contains 4 variables.
 	if ($func =~ /a/ && $func =~ /b/ && $func =~ /c/ && $func =~ /d/) {
 		my @combo = qw/
 		0000
 		0001
 		0010
 		0011
 		0100
 		0101
 		0110
 		0111
 		1000
 		1001
 		1010
 		1011
 		1100
 		1101
 		1110
 		1111
 		/;
 		
 		foreach my $co (@combo) {
 			my $a = substr $co, 0, 1;
  			my $b = substr $co, 1, 1;
   			my $c = substr $co, 2, 1;
    		my $d = substr $co, 3, 1;
    		
    		# Replace a with 1 or 0, etc.
    		my $funcLiteral = $func;
    		$funcLiteral =~ s/a/$a/g;
       		$funcLiteral =~ s/b/$b/g;
    		$funcLiteral =~ s/c/$c/g;
     		$funcLiteral =~ s/d/$d/g;
    		
    		# Concatenate with current truth table string.
    		$tts .= ComputeBooleanFunction($funcLiteral);
 		}
 		return $tts;
 	}
 	
	# Contains 3 variables.
 	elsif ($func =~ /a/ && $func =~ /b/ && $func =~ /c/) {
 		my @combo = qw/
 		000
 		001
 		010
 		011
 		100
 		101
 		110
 		111
 		/;
 		
 		foreach my $co (@combo) {
 			my $a = substr $co, 0, 1;
  			my $b = substr $co, 1, 1;
   			my $c = substr $co, 2, 1;
    		
    		# Replace a with 1 or 0, etc.
    		my $funcLiteral = $func;
    		$funcLiteral =~ s/a/$a/g;
       		$funcLiteral =~ s/b/$b/g;
    		$funcLiteral =~ s/c/$c/g;
    		
    		# Concatenate with current truth table string.
    		$tts .= ComputeBooleanFunction($funcLiteral);
 		}
 		return $tts;
 	}
 	
	# Contains 2 variables.
 	elsif ($func =~ /a/ && $func =~ /b/) {
 		my @combo = qw/
 		00
 		01
 		10
 		11
 		/;
 		
 		foreach my $co (@combo) {
 			my $a = substr $co, 0, 1;
  			my $b = substr $co, 1, 1;
    		
    		# Replace a with 1 or 0, etc.
    		my $funcLiteral = $func;
    		$funcLiteral =~ s/a/$a/g;
       		$funcLiteral =~ s/b/$b/g;
    		
    		# Concatenate with current truth table string.
    		$tts .= ComputeBooleanFunction($funcLiteral);
 		}
 		return $tts;
 	}
 	
	# Contains 1 variable.
 	elsif ($func =~ /a/) {
 		my @combo = qw/
 		0
 		1
 		/;
 		foreach my $co (@combo) {
 			my $a = substr $co, 0, 1;
    		
    		# Replace a with 1 or 0, etc.
    		my $funcLiteral = $func;
    		$funcLiteral =~ s/a/$a/g;
    		
    		# Concatenate with current truth table string.
    		$tts .= ComputeBooleanFunction($funcLiteral);
 		}
 		return $tts;
 	}
 	
 	# Error
 	else {
 		die("Boolean Function string $func did not contain a variable.");
 		return $tts;
 	}	
}

# Key   = Truth table string.
# Value = An array of functions (strings) from @functions that have the same truth table.
my %hash = ();
my @functions = qw/
aaaa
(aa)'(aa)
(aa)(aa)'
(aa)'(aa)'
a'aaa
(a'a)'(aa)
(a'a)(aa)'
(a'a)'(aa)'
aa'aa
(aa')'(aa)
(aa')(aa)'
(aa')'(aa)'
aaa'a
(aa)'(a'a)
(aa)(a'a)'
(aa)'(a'a)'
aaaa'
(aa)'(aa')
(aa)(aa')'
(aa)'(aa')'
a'a'aa
(a'a')'(aa)
(a'a')(aa)'
(a'a')'(aa)'
a'aa'a
(a'a)'(a'a)
(a'a)(a'a)'
(a'a)'(a'a)'
a'aaa'
(a'a)'(aa')
(a'a)(aa')'
(a'a)'(aa')'
aa'a'a
(aa')'(a'a)
(aa')(a'a)'
(aa')'(a'a)'
aa'aa'
(aa')'(aa')
(aa')(aa')'
(aa')'(aa')'
aaa'a'
(aa)'(a'a')
(aa)(a'a')'
(aa)'(a'a')'
a'a'a'a
(a'a')'(a'a)
(a'a')(a'a)'
(a'a')'(a'a)'
aa'a'a'
(aa')'(a'a')
(aa')(a'a')'
(aa')'(a'a')'
a'a'aa'
(a'a')'(aa')
(a'a')(aa')'
(a'a')'(aa')'
a'aa'a'
(a'a)'(a'a')
(a'a)(a'a')'
(a'a)'(a'a')'
a'a'a'a'
(a'a')'(a'a')
(a'a')(a'a')'
(a'a')'(a'a')'

aaab
(aa)'(ab)
(aa)(ab)'
(aa)'(ab)'
a'aab
(a'a)'(ab)
(a'a)(ab)'
(a'a)'(ab)'
aa'aa
(aa')'(ab)
(aa')(ab)'
(aa')'(ab)'
aaa'b
(aa)'(a'b)
(aa)(a'b)'
(aa)'(a'b)'
aaab'
(aa)'(ab')
(aa)(ab')'
(aa)'(ab')'
a'a'ab
(a'a')'(ab)
(a'a')(ab)'
(a'a')'(ab)'
a'aa'b
(a'a)'(a'b)
(a'a)(a'b)'
(a'a)'(a'b)'
a'aab'
(a'a)'(ab')
(a'a)(ab')'
(a'a)'(ab')'
aa'a'b
(aa')'(a'b)
(aa')(a'b)'
(aa')'(a'b)'
aa'ab'
(aa')'(ab')
(aa')(ab')'
(aa')'(ab')'
aaa'b'
(aa)'(a'b')
(aa)(a'b')'
(aa)'(a'b')'
a'a'a'b
(a'a')'(a'b)
(a'a')(a'b)'
(a'a')'(a'b)'
aa'a'b'
(aa')'(a'b')
(aa')(a'b')'
(aa')'(a'b')'
a'a'ab'
(a'a')'(ab')
(a'a')(ab')'
(a'a')'(ab')'
a'aa'b'
(a'a)'(a'b')
(a'a)(a'b')'
(a'a)'(a'b')'
a'a'a'b'
(a'a')'(a'b')
(a'a')(a'b')'
(a'a')'(a'b')'

abaa
(ab)'(aa)
(ab)(aa)'
(ab)'(aa)'
a'baa
(a'b)'(aa)
(a'b)(aa)'
(a'b)'(aa)'
ab'aa
(ab')'(aa)
(ab')(aa)'
(ab')'(aa)'
aba'a
(ab)'(a'a)
(ab)(a'a)'
(ab)'(a'a)'
abaa'
(ab)'(aa')
(ab)(aa')'
(ab)'(aa')'
a'b'aa
(a'b')'(aa)
(a'b')(aa)'
(a'b')'(aa)'
a'ba'a
(a'b)'(a'a)
(a'b)(a'a)'
(a'b)'(a'a)'
a'baa'
(a'b)'(aa')
(a'b)(aa')'
(a'b)'(aa')'
ab'a'a
(ab')'(a'a)
(ab')(a'a)'
(ab')'(a'a)'
ab'aa'
(ab')'(aa')
(ab')(aa')'
(ab')'(aa')'
aba'a'
(ab)'(a'a')
(ab)(a'a')'
(ab)'(a'a')'
a'b'a'a
(a'b')'(a'a)
(a'b')(a'a)'
(a'b')'(a'a)'
ab'a'a'
(ab')'(a'a')
(ab')(a'a')'
(ab')'(a'a')'
a'b'aa'
(a'b')'(aa')
(a'b')(aa')'
(a'b')'(aa')'
a'ba'a'
(a'b)'(a'a')
(a'b)(a'a')'
(a'b)'(a'a')'
a'b'a'a'
(a'b')'(a'a')
(a'b')(a'a')'
(a'b')'(a'a')'

abab
(ab)'(ab)
(ab)(ab)'
(ab)'(ab)'
a'bab
(a'b)'(ab)
(a'b)(ab)'
(a'b)'(ab)'
ab'ab
(ab')'(ab)
(ab')(ab)'
(ab')'(ab)'
aba'b
(ab)'(a'b)
(ab)(a'b)'
(ab)'(a'b)'
abab'
(ab)'(ab')
(ab)(ab')'
(ab)'(ab')'
a'b'ab
(a'b')'(ab)
(a'b')(ab)'
(a'b')'(ab)'
a'ba'b
(a'b)'(a'b)
(a'b)(a'b)'
(a'b)'(a'b)'
a'bab'
(a'b)'(ab')
(a'b)(ab')'
(a'b)'(ab')'
ab'a'b
(ab')'(a'b)
(ab')(a'b)'
(ab')'(a'b)'
ab'ab'
(ab')'(ab')
(ab')(ab')'
(ab')'(ab')'
aba'b'
(ab)'(a'b')
(ab)(a'b')'
(ab)'(a'b')'
a'b'a'b
(a'b')'(a'b)
(a'b')(a'b)'
(a'b')'(a'b)'
ab'a'b'
(ab')'(a'b')
(ab')(a'b')'
(ab')'(a'b')'
a'b'ab'
(a'b')'(ab')
(a'b')(ab')'
(a'b')'(ab')'
a'ba'b'
(a'b)'(a'b')
(a'b)(a'b')'
(a'b)'(a'b')'
a'b'a'b'
(a'b')'(a'b')
(a'b')(a'b')'
(a'b')'(a'b')'

abac
(ab)'(ac)
(ab)(ac)'
(ab)'(ac)'
a'bac
(a'b)'(ac)
(a'b)(ac)'
(a'b)'(ac)'
ab'ac
(ab')'(ac)
(ab')(ac)'
(ab')'(ac)'
aba'c
(ab)'(a'c)
(ab)(a'c)'
(ab)'(a'c)'
abac'
(ab)'(ac')
(ab)(ac')'
(ab)'(ac')'
a'b'ac
(a'b')'(ac)
(a'b')(ac)'
(a'b')'(ac)'
a'ba'c
(a'b)'(a'c)
(a'b)(a'c)'
(a'b)'(a'c)'
a'bac'
(a'b)'(ac')
(a'b)(ac')'
(a'b)'(ac')'
ab'a'c
(ab')'(a'c)
(ab')(a'c)'
(ab')'(a'c)'
ab'ac'
(ab')'(ac')
(ab')(ac')'
(ab')'(ac')'
aba'c'
(ab)'(a'c')
(ab)(a'c')'
(ab)'(a'c')'
a'b'a'c
(a'b')'(a'c)
(a'b')(a'c)'
(a'b')'(a'c)'
ab'a'c'
(ab')'(a'c')
(ab')(a'c')'
(ab')'(a'c')'
a'b'ac'
(a'b')'(ac')
(a'b')(ac')'
(a'b')'(ac')'
a'ba'c'
(a'b)'(a'c')
(a'b)(a'c')'
(a'b)'(a'c')'
a'b'a'c'
(a'b')'(a'c')
(a'b')(a'c')'
(a'b')'(a'c')'

abcd
(ab)'(cd)
(ab)(cd)'
(ab)'(cd)'
a'bcd
(a'b)'(cd)
(a'b)(cd)'
(a'b)'(cd)'
ab'cd
(ab')'(cd)
(ab')(cd)'
(ab')'(cd)'
abc'd
(ab)'(c'd)
(ab)(c'd)'
(ab)'(c'd)'
abcd'
(ab)'(cd')
(ab)(cd')'
(ab)'(cd')'
a'b'cd
(a'b')'(cd)
(a'b')(cd)'
(a'b')'(cd)'
a'bc'd
(a'b)'(c'd)
(a'b)(c'd)'
(a'b)'(c'd)'
a'bcd'
(a'b)'(cd')
(a'b)(cd')'
(a'b)'(cd')'
ab'c'd
(ab')'(c'd)
(ab')(c'd)'
(ab')'(c'd)'
ab'cd'
(ab')'(cd')
(ab')(cd')'
(ab')'(cd')'
abc'd'
(ab)'(c'd')
(ab)(c'd')'
(ab)'(c'd')'
a'b'c'd
(a'b')'(c'd)
(a'b')(c'd)'
(a'b')'(c'd)'
ab'c'd'
(ab')'(c'd')
(ab')(c'd')'
(ab')'(c'd')'
a'b'cd'
(a'b')'(cd')
(a'b')(cd')'
(a'b')'(cd')'
a'bc'd'
(a'b)'(c'd')
(a'b)(c'd')'
(a'b)'(c'd')'
a'b'c'd'
(a'b')'(c'd')
(a'b')(c'd')'
(a'b')'(c'd')'

aaa
(a)'(aa)
(a)(aa)'
(a)'(aa)'
a'aa
(a')'(aa)
(a')(aa)'
(a')'(aa)'
aa'a
(a)'(a'a)
(a)(a'a)'
(a)'(a'a)'
aaa'
(a)'(aa')
(a)(aa')'
(a)'(aa')'
a'a'a
(a')'(a'a)
(a')(a'a)'
(a')'(a'a)'
a'aa'
(a')'(aa')
(a')(aa')'
(a')'(aa')'
aa'a'
(a)'(a'a')
(a)(a'a')'
(a)'(a'a')'
a'a'a'
(a')'(a'a')
(a')(a'a')'
(a')'(a'a')'

aab
(a)'(ab)
(a)(ab)'
(a)'(ab)'
a'ab
(a')'(ab)
(a')(ab)'
(a')'(ab)'
aa'b
(a)'(a'b)
(a)(a'b)'
(a)'(a'b)'
aab'
(a)'(ab')
(a)(ab')'
(a)'(ab')'
a'a'b
(a')'(a'b)
(a')(a'b)'
(a')'(a'b)'
a'ab'
(a')'(ab')
(a')(ab')'
(a')'(ab')'
aa'b'
(a)'(a'b')
(a)(a'b')'
(a)'(a'b')'
a'a'b'
(a')'(a'b')
(a')(a'b')'
(a')'(a'b')'

abc
(a)'(bc)
(a)(bc)'
(a)'(bc)'
a'bc
(a')'(bc)
(a')(bc)'
(a')'(bc)'
ab'c
(a)'(b'c)
(a)(b'c)'
(a)'(b'c)'
abc'
(a)'(bc')
(a)(bc')'
(a)'(bc')'
a'b'c
(a')'(b'c)
(a')(b'c)'
(a')'(b'c)'
a'bc'
(a')'(bc')
(a')(bc')'
(a')'(bc')'
ab'c'
(a)'(b'c')
(a)(b'c')'
(a)'(b'c')'
a'b'c'
(a')'(b'c')
(a')(b'c')'
(a')'(b'c')'
/;

foreach my $func (@functions) {
	my $tts = GenerateTruthTableString($func);
	push ( @{$hash{$tts}}, $func);
}

foreach my $k (keys %hash){
	print "($k) -> ";
	foreach my $e (@{$hash{$k}}) {
		print "$e, ";
	}
	print "\n";
}


