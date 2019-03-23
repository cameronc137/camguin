#!/bin/bash
#
initial=$1
final=$2
for i in `seq $initial $final`;
do
#			script name.C(flag,"draw options",add n runs, online = 1)
	root -b -l -q ./target_stability_macro.C\(7,\"\",${i},0,0\) # T5/T2 avg value per run
	root -b -l -q ./target_stability_macro.C\(6,\"\",${i},0,0\) # T6 avg value per run
	root -b -l -q ./target_stability_macro.C\(5,\"\",${i},0,0\) # T5 avg value per run
	root -b -l -q ./target_stability_macro.C\(4,\"\",${i},0,0\) # T2 avg value per run
	root -b -l -q ./target_stability_macro.C\(3,\"\",${i},0,0\) # T2/uA stability
	root -b -l -q ./target_stability_macro.C\(2,\"\",${i},0,0\) # T6/uA stability
	root -b -l -q ./target_stability_macro.C\(1,\"\",${i},0,0\) # integrated charge
#	root -b -l -q ./delta_correlation_macro.C\(1,\"\",${i},0,0\) # integrated charge
#	root -b -l -q ./delta_correlation_macro.C\(2,\"\",${i},0,0\) # integrated charge
#	root -b -l -q ./delta_correlation_macro.C\(3,\"\",${i},0,0\) # integrated charge
#	root -b -l -q ./Bob_coinc_check_macro.C\(1,\"\",${i},0,0\) # integrated charge
#	root -b -l -q ./Bob_coinc_check_macro.C\(2,\"\",${i},0,0\) # integrated charge
#	root -b -l -q ./Bob_coinc_check_macro.C\(3,\"\",${i},0,0\) # integrated charge
done
