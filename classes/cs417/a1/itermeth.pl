#!/usr/bin/perl
sub IterMath {
	($a, $x, $es, $maxit) = @_;
	$iter = 1;
	$sol = 1;
	$ea = 100;
	while (( $ea <= $es ) || ($iter >= $maxit)) {
	 $solold = $sol;
	 $sol = ($x + ( $a + $x)) / 2;
	 $iter ++;
	 if ($sol !=0){
	  $ea = abs(($sol - $solold)/$sol) * 100;
	 }
	}
	$IterMeth = $sol;
	print $IterMeth;
}

IterMath(1,1,.05,100);
